/**
 * @file xf_main.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include <string.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"

#include "xf_utils.h"
#include "xf_wifi.h"
#include "xf_netif.h"
#include "xf_osal.h"

#include "xf_ping.h"

#include "ex_easy_wifi.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void _example_thread(void *argument);
static void xf_ping_cb(
    xf_ping_event_id_t event_id, xf_ping_t hdl, void *user_args);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "sta";

static xf_osal_thread_t s_thread_hdl = NULL;
#define EX_THREAD_NAME          "ex_thread"
#define EX_THREAD_PRIORITY      XF_OSAL_PRIORITY_NORMOL
#define EX_THREAD_STACK_SIZE    (1024 * 4)
static const xf_osal_thread_attr_t s_thread_attr = {
    .name       = EX_THREAD_NAME,
    .priority   = EX_THREAD_PRIORITY,
    .stack_size = EX_THREAD_STACK_SIZE,
};

static uint8_t s_ping_done = false;
static const char *const s_hostname_arr[] = {
    "example.com",
    "www.baidu.com",
    "www.qq.com",
    "www.360.com",
};

/* ==================== [Macros] ============================================ */

#define ERROR_CHECK(e) \
    do { \
        if (unlikely((e) != XF_OK)) { \
            XF_LOGE(TAG, "An error occurred: %s", XSTR(e != XF_OK)); \
            while(1); \
        } \
    } while (0)
#define TEST_XF_OK(e) ERROR_CHECK(e)

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    s_thread_hdl = xf_osal_thread_create(_example_thread, NULL, &s_thread_attr);
    if (s_thread_hdl == NULL) {
        XF_LOGE(TAG, "xf_osal_thread_create error");
        return;
    }
}

/* ==================== [Static Functions] ================================== */

static void xf_ping_cb(
    xf_ping_event_id_t event_id, xf_ping_t hdl, void *user_args)
{
    UNUSED(user_args);
    switch (event_id) {
    case XF_PING_EVENT_SUCC: {
        xf_printf("%d bytes from " XF_IPSTR " icmp_seq=%d ttl=%d time=%d ms" "\n",
                  hdl->recv_len, XF_IP2STR(&hdl->recv_addr.u_addr.ip4),
                  hdl->packet_hdr->seqno, hdl->ttl, hdl->elapsed_time_ms);
    } break;
    case XF_PING_EVENT_TIMEOUT: {
        xf_printf("From " XF_IPSTR " icmp_seq=%d timeout" "\n",
                  XF_IP2STR(&hdl->recv_addr.u_addr.ip4), hdl->packet_hdr->seqno);
    } break;
    case XF_PING_EVENT_END: {
        uint32_t loss = (uint32_t)((1 - ((float)hdl->received) / hdl->transmitted) * 100);
        if (IP_IS_V4(&hdl->recv_addr)) {
            struct in_addr ip4;
            ip4.s_addr = hdl->recv_addr.u_addr.ip4.addr;
            xf_printf("\n--- %s ping statistics ---\n", inet_ntoa(ip4));
        } else {
            xf_printf("\n--- %s ping statistics ---\n", inet6_ntoa(*ip_2_ip6(&hdl->recv_addr)));
        }
        xf_printf("%d packets transmitted, %d received, %d%% packet loss, time %dms" "\n",
                  hdl->transmitted, hdl->received, loss, hdl->total_time_ms);
        s_ping_done = true;
    } break;
    default:
        break;
    }
}

static void _example_thread(void *argument)
{
    UNUSED(argument);

    ex_easy_wifi_sta();

    xf_netif_t netif_hdl = NULL;
    xf_wifi_sta_get_netif(&netif_hdl);

    xf_netif_dns_info_t dns_info = {0};
    xf_netif_get_dns_info(netif_hdl, XF_NETIF_DNS_MAIN, &dns_info);
    XF_LOGI(TAG, "dns main:     " XF_IPSTR, XF_IP2STR(&dns_info.ip.u_addr.ip4));
    xf_netif_get_dns_info(netif_hdl, XF_NETIF_DNS_BACKUP, &dns_info);
    XF_LOGI(TAG, "dns backup:   " XF_IPSTR, XF_IP2STR(&dns_info.ip.u_addr.ip4));

    for (size_t hostname_idx = 0;
            hostname_idx < ARRAY_SIZE(s_hostname_arr);
            hostname_idx++) {

        xf_ip4_addr_t dest_ip4 = {0};
        struct hostent *host;
        ip_addr_t target_addr = {0};

        /* 解析域名 */
        if ((host = gethostbyname(s_hostname_arr[hostname_idx])) == NULL) {
            XF_LOGE(TAG, "gethostbyname failed");
            dest_ip4 = ex_easy_wifi_sta_get_gw_ip();
            *(uint32_t *)(host->h_addr) = dest_ip4.addr;
        }
        dest_ip4.addr = *(uint32_t *)(host->h_addr);

        target_addr.u_addr.ip4.addr = dest_ip4.addr;
        target_addr.type = IPADDR_TYPE_V4;
        XF_LOGI(TAG, "target_addr: " XF_IPSTR,
                XF_IP2STR(&target_addr.u_addr.ip4));

        xf_ping_cfg_t ping_cfg = XF_PING_DEFAULT_CONFIG();
        ping_cfg.timeout_ms  = 2000;
        ping_cfg.target_addr = target_addr;
        ping_cfg.count       = 5;
        ping_cfg.interval_ms = 1000;

        xf_ping_t ping_hdl = NULL;
        TEST_XF_OK(xf_ping_new_session(
                       &ping_cfg, xf_ping_cb, NULL,
                       &ping_hdl));
        TEST_XF_OK(xf_ping_start(ping_hdl));

        while (s_ping_done == false) {
            xf_osal_delay_ms(100);
        }
        s_ping_done = false;

        TEST_XF_OK(xf_ping_delete_session(ping_hdl));

        xf_osal_delay_ms(1000);
    } /* for 0 ~ ARRAY_SIZE(s_hostname_arr) */

    xf_osal_thread_delete(NULL);
}
