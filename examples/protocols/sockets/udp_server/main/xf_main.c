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
 * @example{lineno} examples/protocols/sockets/udp_server/main/xf_main.c
 * sockets udp 服务端 示例。
 */

/* ==================== [Includes] ========================================== */

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/inet.h"

#include "xf_utils.h"
#include "xf_wifi.h"
#include "xf_netif.h"
#include "xf_osal.h"

#include "ex_easy_wifi.h"

/* ==================== [Defines] =========================================== */

#define PORT                CONFIG_EXAMPLE_PORT
#define EXAMPLE_DELAY_MS    100
#define CLIENT_IP_ADDR      INADDR_ANY

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void _example_thread(void *argument);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "ap";
static const char *payload = "This is a message from server.";

static xf_osal_thread_t s_thread_hdl = NULL;
#define EX_THREAD_NAME          "ex_thread"
#define EX_THREAD_PRIORITY      XF_OSAL_PRIORITY_NORMOL
#define EX_THREAD_STACK_SIZE    (1024 * 4)
static const xf_osal_thread_attr_t s_thread_attr = {
    .name       = EX_THREAD_NAME,
    .priority   = EX_THREAD_PRIORITY,
    .stack_size = EX_THREAD_STACK_SIZE,
};

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

    ex_easy_wifi_ap();

    char rx_buffer[128];
    int addr_family = 0;
    int ip_protocol = 0;
    int sockfd = 0;

    struct sockaddr_in dest_addr = {0};
    struct sockaddr_in source_addr = {0};
    struct timeval timeout = {0};

    dest_addr.sin_addr.s_addr   = htonl(CLIENT_IP_ADDR);
    dest_addr.sin_family        = AF_INET;
    dest_addr.sin_port          = htons(PORT);
    addr_family                 = AF_INET;
    ip_protocol                 = IPPROTO_IP;

    sockfd = socket(addr_family, SOCK_DGRAM, ip_protocol);
    if (sockfd < 0) {
        XF_LOGE(TAG, "Unable to create socket: errno %d", errno);
        goto l_err;
    }
    XF_LOGI(TAG, "Socket created");

    /* 设置接收超时时间 */
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    /* 绑定端口号 */
    int err = bind(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        XF_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        goto l_err;
    }
    XF_LOGI(TAG, "Socket bound, port %d", PORT);

    for (;;) {
        xf_osal_delay_ms(EXAMPLE_DELAY_MS);

        XF_LOGI(TAG, "Waiting for data.");

        socklen_t socklen = sizeof(source_addr);
        int len = recvfrom(sockfd,
                           rx_buffer, sizeof(rx_buffer) - 1, 0,
                           (struct sockaddr *)&source_addr, &socklen);
        if (len < 0) {
            XF_LOGE(TAG, "recvfrom failed: errno %d", errno);
            continue;
        }

        /* 获取来源地址 */
        xf_ip4_addr_t src_ip4 = {0};
        src_ip4.addr = source_addr.sin_addr.s_addr;

        rx_buffer[len] = 0;
        XF_LOGI(TAG, "Received %d bytes from " XF_IPSTR ":",
                len, XF_IP2STR(&src_ip4));
        XF_LOGI(TAG, "%s", rx_buffer);

        /* 发送 */
        int err = sendto(sockfd,
                         payload, strlen(payload), 0,
                         (struct sockaddr *)&source_addr, sizeof(source_addr));
        if (err < 0) {
            XF_LOGE(TAG, "Error occurred during sending: errno %d", errno);
            break;
        }
    }

l_err:;
    if (sockfd != -1) {
        XF_LOGE(TAG, "Shutting down socket...");
        shutdown(sockfd, 0);
        closesocket(sockfd);
    }
    xf_osal_thread_delete(NULL);
}