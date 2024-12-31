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

/**
 * @example{lineno} examples/protocols/iperf/softap/main/xf_main.c
 * wifi iperf 示例(softap)。
 */

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/inet.h"

#include "xf_utils.h"
#include "xf_wifi.h"
#include "xf_netif.h"
#include "xf_osal.h"
#include "xf_sys.h"

#include "xf_iperf.h"

#include "ex_easy_wifi.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void _example_thread(void *argument);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "xf_main";

static xf_osal_thread_t s_thread_hdl = NULL;
#define EX_THREAD_NAME          "ex_thread"
#define EX_THREAD_PRIORITY      XF_OSAL_PRIORITY_NORMOL
#define EX_THREAD_STACK_SIZE    (1024 * 4)
static const xf_osal_thread_attr_t s_thread_attr = {
    .name       = EX_THREAD_NAME,
    .priority   = EX_THREAD_PRIORITY,
    .stack_size = EX_THREAD_STACK_SIZE,
};

#ifdef CONFIG_EXAMPLE_IPERF_AUTO_TEST
static const uint32_t s_iperf_test_item[] = {
    IPERF_FLAG_SERVER | IPERF_FLAG_UDP,
    IPERF_FLAG_SERVER | IPERF_FLAG_TCP,
    IPERF_FLAG_CLIENT | IPERF_FLAG_UDP,
    IPERF_FLAG_CLIENT | IPERF_FLAG_TCP,
};
#endif /* CONFIG_EXAMPLE_IPERF_AUTO_TEST */

/* ==================== [Macros] ============================================ */

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

static void _example_thread(void *argument)
{
    UNUSED(argument);

    uint32_t test_idx = 0;

    xf_ip4_addr_t dest_ip4 = {0};
    xf_ip4_addr_t src_ip4 = {0};

    ex_easy_wifi_ap();
    src_ip4 = ex_easy_wifi_ap_get_onw_ip();
    dest_ip4 = ex_easy_wifi_ap_get_last_sta_ip();

    XF_LOGI(TAG, "\n"
            "src_ip4:       " XF_IPSTR "\n"
            "dest_ip4:      " XF_IPSTR,
            XF_IP2STR(&src_ip4),
            XF_IP2STR(&dest_ip4));

    xf_sys_watchdog_disable();

    for (test_idx = 0; test_idx < ARRAY_SIZE(s_iperf_test_item); ++test_idx) {
        xf_osal_delay_ms(IPERF_SOCKET_RX_TIMEOUT_MS);
        xf_iperf_stop();

        xf_iperf_cfg_t cfg  = XF_IPERF_DEFAULT_CONFIG();
        cfg.type            = IPERF_IP_TYPE_IPV4;
        cfg.time            = CONFIG_EXAMPLE_IPERF_TIME;
        cfg.dip.u_addr.ip4.addr = dest_ip4.addr;
        cfg.sip.u_addr.ip4.addr = src_ip4.addr;

#ifdef CONFIG_EXAMPLE_IPERF_AUTO_TEST
        if (s_iperf_test_item[test_idx] & IPERF_FLAG_CLIENT) {
            /* 延迟一会，避免服务端还没建立 */
            xf_osal_delay_ms(100);
        }
        cfg.flag = s_iperf_test_item[test_idx];
#else
#   if defined(CONFIG_EXAMPLE_IPERF_SELECT_SERVER)
        cfg.flag |= IPERF_FLAG_SERVER;
#   else
        cfg.flag |= IPERF_FLAG_CLIENT;
#   endif
#   if defined(CONFIG_EXAMPLE_IPERF_SELECT_UDP)
        cfg.flag |= IPERF_FLAG_UDP;
#   else
        cfg.flag |= IPERF_FLAG_TCP;
#   endif
#endif /* CONFIG_EXAMPLE_IPERF_AUTO_TEST */

        XF_LOGI(TAG,
                "mode=%s-%s "
                "sip=" XF_IPSTR ":%d,"
                "dip=" XF_IPSTR ":%d,"
                "interval=%" PRId32 ", time=%" PRId32 "",
                cfg.flag & IPERF_FLAG_TCP ? "tcp" : "udp",
                cfg.flag & IPERF_FLAG_SERVER ? "server" : "client",
                XF_IP2STR(&cfg.sip.u_addr.ip4), (int)cfg.sport,
                XF_IP2STR(&cfg.dip.u_addr.ip4), (int)cfg.dport,
                cfg.interval, cfg.time);

        xf_iperf_start(&cfg, NULL, NULL);

        while (xf_iperf_is_running()) {
            xf_osal_delay_ms(100);
        }
    } /* for */

    XF_LOGI(TAG, "test done!");

    xf_osal_thread_delete(NULL);
}

// /**
//  * @brief iperf 回调函数。
//  *
//  * 这是 iperf 回调示例，如果不需要 iperf 内部报告，
//  * 则需要修改 `xf_iperf_cfg_t.report_enabled = false;
//  * 并使用该回调 `xf_iperf_start(&cfg, _xf_iperf_cb, NULL);`
//  *
//  * @param event_id iperf 事件 id.
//  * @param hdl iperf 句柄。
//  * @param user_args 用户数据。
//  */
// static void _xf_iperf_cb(
//     xf_iperf_event_id_t event_id, xf_iperf_t hdl, void *user_args)
// {
//     UNUSED(user_args);
//     switch (event_id) {
//     case XF_IPERF_EVENT_START: {
//         XF_LOGI(TAG, "%16s %s", "Interval", "Bandwidth");
//     } break;
//     case XF_IPERF_EVENT_REPORT: {
//         XF_LOGI(TAG, "%4d-%4d sec       %.2f Mbits/sec",
//                 hdl->curr_time, hdl->curr_time + hdl->cfg.interval,
//                 hdl->actual_bandwidth);
//     } break;
//     case XF_IPERF_EVENT_END: {
//         XF_LOGI(TAG, "%4d-%4d sec       %.2f Mbits/sec",
//                 0, hdl->cfg.time, hdl->average_bandwidth);
//     } break;
//     default:
//         break;
//     }
// }
