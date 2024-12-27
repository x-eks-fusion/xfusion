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
 * @example{lineno} examples/protocols/http_request/main/xf_main.c
 * http 请求示例。
 */

/* ==================== [Includes] ========================================== */

#include <string.h>
#include <sys/param.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"

#include <unistd.h>
#include <arpa/inet.h>

#include "xf_utils.h"
#include "xf_wifi.h"
#include "xf_netif.h"
#include "xf_osal.h"
#include "xf_task.h"

#include "ex_easy_wifi.h"

/* ==================== [Defines] =========================================== */

#define HTTP_BUFFER_SIZE    (1024 * 2)

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void _example_thread(void *argument);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "sta";
static char s_http_buf[HTTP_BUFFER_SIZE];

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

    ex_easy_wifi_sta();

    xf_netif_t netif_hdl = NULL;
    xf_wifi_sta_get_netif(&netif_hdl);

    xf_netif_dns_info_t dns_info = {0};
    xf_netif_get_dns_info(netif_hdl, XF_NETIF_DNS_MAIN, &dns_info);
    XF_LOGI(TAG, "dns main:     " XF_IPSTR, XF_IP2STR(&dns_info.ip.u_addr.ip4));
    xf_netif_get_dns_info(netif_hdl, XF_NETIF_DNS_BACKUP, &dns_info);
    XF_LOGI(TAG, "dns backup:   " XF_IPSTR, XF_IP2STR(&dns_info.ip.u_addr.ip4));

    int sockfd = 0;
    struct sockaddr_in server_addr;
    struct hostent *host;
    char *hostname = "example.com";
    char *request = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";

    /* 解析域名 */
    if ((host = gethostbyname(hostname)) == NULL) {
        XF_LOGE(TAG, "gethostbyname failed");
        goto l_err;
    }

    /* 创建 socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        XF_LOGE(TAG, "Socket creation failed");
        goto l_err;
    }

    /* 设置服务器地址 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    server_addr.sin_addr.s_addr = *(long *)(host->h_addr);

    /* 连接服务器 */
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        XF_LOGE(TAG, "Connection failed");
        closesocket(sockfd);
        goto l_err;
    }

    /* 发送 HTTP 请求 */
    send(sockfd, request, strlen(request), 0);

    /* 接收服务器响应 */
    int bytes_received = recv(sockfd, s_http_buf, HTTP_BUFFER_SIZE - 1, 0);
    if (bytes_received < 0) {
        XF_LOGE(TAG, "Receive failed");
    } else {
        s_http_buf[bytes_received] = '\0';
        XF_LOGI(TAG, "Response:\n%s\n", s_http_buf);
    }

l_err:;
    /* 关闭 socket */
    closesocket(sockfd);
    xf_osal_thread_delete(NULL);
}
