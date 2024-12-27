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
 * @example{lineno} examples/protocols/sockets/tcp_server/main/xf_main.c
 * sockets tcp 服务端 示例。
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
#include "xf_sys.h"

#include "ex_easy_wifi.h"

/* ==================== [Defines] =========================================== */

#define PORT                CONFIG_EXAMPLE_PORT
#define KEEPALIVE_IDLE      CONFIG_EXAMPLE_KEEPALIVE_IDLE
#define KEEPALIVE_INTERVAL  CONFIG_EXAMPLE_KEEPALIVE_INTERVAL
#define KEEPALIVE_COUNT     CONFIG_EXAMPLE_KEEPALIVE_COUNT
#define EXAMPLE_DELAY_MS    1000
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

    int err = 0;
    char rx_buffer[128];
    int addr_family = 0;
    int ip_protocol = 0;
    int sockfd_listen = 0;
    int sockfd_conn = 0;
    int keepAlive = 1;
    int keepIdle = KEEPALIVE_IDLE;
    int keepInterval = KEEPALIVE_INTERVAL;
    int keepCount = KEEPALIVE_COUNT;
    xf_ip4_addr_t src_ip4;

    struct sockaddr_in dest_addr = {0};
    struct sockaddr_in source_addr = {0};
    struct timeval timeout = {0};

    dest_addr.sin_addr.s_addr   = htonl(CLIENT_IP_ADDR);
    dest_addr.sin_family        = AF_INET;
    dest_addr.sin_port          = htons(PORT);
    addr_family                 = AF_INET;
    ip_protocol                 = IPPROTO_IP;

    sockfd_listen = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (sockfd_listen < 0) {
        XF_LOGE(TAG, "Unable to create socket: errno %d", errno);
        goto l_err;
    }
    XF_LOGI(TAG, "Socket created");

    /* 绑定端口号 */
    err = bind(sockfd_listen, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        XF_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        goto l_err;
    }
    XF_LOGI(TAG, "Socket bound, port %d", PORT);

    /* 监听 */
    err = listen(sockfd_listen, 1);
    if (err != 0) {
        XF_LOGE(TAG, "Error occurred during listen: errno %d", errno);
        goto l_err;
    }

    XF_LOGI(TAG, "Socket listening");

    socklen_t addr_len = sizeof(source_addr);
    sockfd_conn = accept(sockfd_listen, (struct sockaddr *)&source_addr, &addr_len);
    if (sockfd_conn < 0) {
        XF_LOGE(TAG, "Unable to accept connection: errno %d", errno);
        goto l_err;
    }

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    setsockopt(sockfd_conn, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    /* 设置 tcp keepalive 选项 */
    setsockopt(sockfd_conn, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
    setsockopt(sockfd_conn, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
    setsockopt(sockfd_conn, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
    setsockopt(sockfd_conn, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));

    for (;;) {
        socklen_t socklen = sizeof(source_addr);
        int len = recvfrom(sockfd_conn,
                           rx_buffer, sizeof(rx_buffer) - 1, 0,
                           (struct sockaddr *)&source_addr, &socklen);
        if (len < 0) {
            XF_LOGE(TAG, "recvfrom failed: errno %d", errno);
            continue;
        } else if (len == 0) {
            XF_LOGW(TAG, "Connection closed");
        }

        rx_buffer[len] = 0;

        src_ip4.addr = source_addr.sin_addr.s_addr;
        XF_LOGI(TAG, "Received %d bytes from " XF_IPSTR ": \"%s\"",
                len, XF_IP2STR(&src_ip4), rx_buffer);

        /* 发送 */
        int payload_len = strlen(payload);
        err = sendto(sockfd_conn,
                     payload, payload_len, 0,
                     (struct sockaddr *)&source_addr, sizeof(source_addr));
        if (err < 0) {
            XF_LOGE(TAG, "Error occurred during sending: errno %d", errno);
            break;
        }
        XF_LOGI(TAG, "send     %d bytes to   " XF_IPSTR ": \"%s\"",
                payload_len, XF_IP2STR(&src_ip4), payload);

        xf_osal_delay_ms(EXAMPLE_DELAY_MS);
    }

l_err:;
    if (sockfd_conn != -1) {
        shutdown(sockfd_conn, 0);
        closesocket(sockfd_conn);
    }
    if (sockfd_listen != -1) {
        XF_LOGE(TAG, "Shutting down socket...");
        shutdown(sockfd_listen, 0);
        closesocket(sockfd_listen);
    }
    xf_osal_thread_delete(NULL);
}