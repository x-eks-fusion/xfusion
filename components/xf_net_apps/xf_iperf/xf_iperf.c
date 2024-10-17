/**
 * @file xf_iperf.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-09-26
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* Iperf Example - iperf implementation

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/* ==================== [Includes] ========================================== */

#include <stdio.h>
#include <string.h>
#include <sys/param.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"

#include "xf_utils.h"
#include "xf_netif.h"
#include "xf_osal.h"
#include "xf_sys.h"

#include "xf_iperf.h"

/* ==================== [Defines] =========================================== */

#define IPERF_HUNDRED          100

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static inline bool iperf_is_udp_client(void);
static inline bool iperf_is_udp_server(void);
static inline bool iperf_is_tcp_client(void);
static inline bool iperf_is_tcp_server(void);
static int iperf_get_socket_error_code(int sockfd);
static int iperf_show_socket_error_reason(const char *str, int sockfd);
static void iperf_report_task(void *arg);
static xf_err_t iperf_start_report(void);
static void socket_recv(int recv_socket, struct sockaddr *listen_addr, uint8_t type);
static void socket_send(int send_socket, struct sockaddr *dest_addr, uint8_t type, int bw_lim);
static xf_err_t iperf_run_tcp_server(void);
static xf_err_t iperf_run_tcp_client(void);
static xf_err_t iperf_run_udp_server(void);
static xf_err_t iperf_run_udp_client(void);
static void iperf_task_traffic(void *arg);
static uint32_t iperf_get_buffer_len(void);
static uint32_t iperf_get_float_int(float in);
static uint32_t iperf_get_float_dec(float in);

/* ==================== [Static Variables] ================================== */

static xf_iperf_ctx_t s_ctx;
static const char *TAG = "iperf";

/* ==================== [Macros] ============================================ */

#define XF_ASSERT_RET_GOTO(condition, err_code, label, tag, format, ...) \
    do { \
        ret = err_code; \
        XF_ASSERT_GOTO(condition, label, tag, format, ##__VA_ARGS__); \
    } while (0)

/* ==================== [Global Functions] ================================== */

xf_err_t xf_iperf_start(
    const xf_iperf_cfg_t *p_cfg, xf_iperf_cb_t cb_func, void *user_args)
{
    if (!p_cfg) {
        return XF_FAIL;
    }

    if (s_ctx.traffic_is_running) {
        XF_LOGW(TAG, "iperf is running");
        return XF_FAIL;
    }

    xf_memset(&s_ctx, 0, sizeof(s_ctx));
    xf_memcpy(&s_ctx.cfg, p_cfg, sizeof(*p_cfg));
    s_ctx.traffic_is_running    = true;
    s_ctx.finish                = false;
    s_ctx.buffer_len            = iperf_get_buffer_len();
    s_ctx.buffer                = (uint8_t *)xf_malloc(s_ctx.buffer_len);
    if (!s_ctx.buffer) {
        XF_LOGE(TAG, "create buffer: not enough memory");
        return XF_FAIL;
    }
    xf_memset(s_ctx.buffer, 0, s_ctx.buffer_len);

    s_ctx.cb_func         = cb_func;
    s_ctx.user_args       = user_args;

    /* socket 成功通信后再创建 report_task */

    xf_osal_thread_attr_t thread_attr = {
        .name       = IPERF_TRAFFIC_TASK_NAME,
        .priority   = p_cfg->traffic_task_prio,
        .stack_size = p_cfg->traffic_task_stack_size,
    };
    s_ctx.traffic_task_hdl =
        xf_osal_thread_create(iperf_task_traffic, NULL, &thread_attr);
    if (s_ctx.traffic_task_hdl == NULL) {
        XF_LOGE(TAG, "create task %s failed", IPERF_TRAFFIC_TASK_NAME);
        xf_free(s_ctx.buffer);
        s_ctx.buffer = NULL;
        return XF_FAIL;
    }
    return XF_OK;
}

xf_err_t xf_iperf_stop(void)
{
    if (s_ctx.traffic_is_running) {
        s_ctx.finish = true;
    }

    while (s_ctx.traffic_is_running || s_ctx.report_is_running) {
        XF_LOGI(TAG, "wait current iperf to stop ...");
        xf_osal_delay_ms(100);
    }

    return XF_OK;
}

bool xf_iperf_is_running(void)
{
    return s_ctx.traffic_is_running;
}

/* ==================== [Static Functions] ================================== */

static inline bool iperf_is_udp_client(void)
{
    return ((s_ctx.cfg.flag & IPERF_FLAG_CLIENT)
            && (s_ctx.cfg.flag & IPERF_FLAG_UDP));
}

static inline bool iperf_is_udp_server(void)
{
    return ((s_ctx.cfg.flag & IPERF_FLAG_SERVER)
            && (s_ctx.cfg.flag & IPERF_FLAG_UDP));
}

static inline bool iperf_is_tcp_client(void)
{
    return ((s_ctx.cfg.flag & IPERF_FLAG_CLIENT)
            && (s_ctx.cfg.flag & IPERF_FLAG_TCP));
}

static inline bool iperf_is_tcp_server(void)
{
    return ((s_ctx.cfg.flag & IPERF_FLAG_SERVER)
            && (s_ctx.cfg.flag & IPERF_FLAG_TCP));
}

static int iperf_get_socket_error_code(int sockfd)
{
    return errno;
}

static int iperf_show_socket_error_reason(const char *str, int sockfd)
{
    int err = errno;
    if (err != 0) {
        XF_LOGW(TAG, "%s error, error code: %d, reason: %s",
                str, err, strerror(err));
    }

    return err;
}

static void iperf_report_task(void *arg)
{
    UNUSED(arg);
    s_ctx.report_is_running = true;

    uint32_t interval = s_ctx.cfg.interval;
    uint32_t time = s_ctx.cfg.time;
    uint32_t delay_interval_ms = interval * 1000;
    uint32_t cur = 0;
    float average = 0;
    float actual_bandwidth = 0;
    int k = 1;

    if (s_ctx.cb_func) {
        s_ctx.cb_func(XF_IPERF_EVENT_REPORT, &s_ctx, s_ctx.user_args);
    }
    if (s_ctx.cfg.report_enabled) {
        xf_printf("\n%16s %s\n", "Interval", "Bandwidth");
    }

    while (!s_ctx.finish) {
        xf_osal_delay_ms(delay_interval_ms);

        /* 更新本次带宽 */
        s_ctx.actual_len = s_ctx.actual_len_internal;
        actual_bandwidth = ((float)(s_ctx.actual_len * 8) / 1e6f) / (float)interval;
        s_ctx.actual_bandwidth = actual_bandwidth;

        /* 更新平均带宽 */
        average = ((average * (k - 1) / k) + (actual_bandwidth / k));
        s_ctx.average_bandwidth = average;

        /* 更新报告时刻 */
        s_ctx.curr_time = cur;

        if (s_ctx.cb_func) {
            s_ctx.cb_func(XF_IPERF_EVENT_REPORT, &s_ctx, s_ctx.user_args);
        }
        if (s_ctx.cfg.report_enabled) {
            xf_printf("%4d-%4d sec       %d.%02d Mbits/sec\n",
                      cur, cur + interval,
                      iperf_get_float_int(actual_bandwidth),
                      iperf_get_float_dec(actual_bandwidth));
        }

        cur += interval;
        k++;
        s_ctx.actual_len_internal = 0;

        if (cur >= time) {
            s_ctx.finish = true;
            break;
        }
    }

    s_ctx.finish = true;

    if (s_ctx.cb_func) {
        s_ctx.cb_func(XF_IPERF_EVENT_END, &s_ctx, s_ctx.user_args);
    }
    if (s_ctx.cfg.report_enabled) {
        xf_printf("%4d-%4d sec       %d.%02d Mbits/sec\n",
                  0, time,
                  iperf_get_float_int(average),
                  iperf_get_float_dec(average));
    }

    s_ctx.report_is_running = false;
    xf_osal_thread_delete(NULL);
}

static xf_err_t iperf_start_report(void)
{
    xf_osal_thread_attr_t thread_attr = {
        .name       = IPERF_REPORT_TASK_NAME,
        .priority   = s_ctx.cfg.report_task_prio,
        .stack_size = s_ctx.cfg.report_task_stack_size,
    };
    s_ctx.report_task_hdl = xf_osal_thread_create(iperf_report_task, NULL, &thread_attr);
    if (s_ctx.report_task_hdl == NULL) {
        XF_LOGE(TAG, "create task %s failed", IPERF_REPORT_TASK_NAME);
        s_ctx.finish = true;
        return XF_FAIL;
    }
    return XF_OK;
}

static void socket_recv(int recv_socket, struct sockaddr *listen_addr, uint8_t type)
{
    bool iperf_recv_start = true;
    uint8_t *buffer;
    int want_recv = 0;
    int actual_recv = 0;
    socklen_t socklen = (s_ctx.cfg.type == IPERF_IP_TYPE_IPV6)
                        ? sizeof(struct sockaddr_in6)
                        : sizeof(struct sockaddr_in);
    const char *error_log = (type == IPERF_TRANS_TYPE_TCP)
                            ? "tcp server recv"
                            : "udp server recv";

    s_ctx.actual_len_internal = 0;

    buffer      = s_ctx.buffer;
    want_recv   = s_ctx.buffer_len;
    while (!s_ctx.finish) {
        actual_recv = recvfrom(recv_socket, buffer, want_recv, 0, listen_addr, &socklen);
        if (actual_recv >= 0) {
            if (iperf_recv_start) {
                iperf_start_report();
                iperf_recv_start = false;
            }
            s_ctx.actual_len_internal += actual_recv;
        } else if (type == IPERF_TRANS_TYPE_TCP) { /* UDP 时不判断接收错误 */
            iperf_show_socket_error_reason(error_log, recv_socket);
            s_ctx.finish = true;
            break;
        }

    }
}

static void socket_send(int send_socket, struct sockaddr *dest_addr, uint8_t type, int bw_lim)
{
    uint8_t *buffer;
    uint32_t *pkt_id_p;
    uint32_t pkt_cnt = 0;
    int actual_send = 0;
    int want_send = 0;
    int period_us = -1;
    int delay_us = 0;
    xf_us_t prev_time = 0;
    xf_us_t send_time = 0;
    int err = 0;
    const socklen_t socklen = (s_ctx.cfg.type == IPERF_IP_TYPE_IPV6)
                              ? sizeof(struct sockaddr_in6)
                              : sizeof(struct sockaddr_in);
    const char *error_log = (type == IPERF_TRANS_TYPE_TCP)
                            ? "tcp client send"
                            : "udp client send";

    buffer = s_ctx.buffer;
    pkt_id_p = (uint32_t *)s_ctx.buffer;
    want_send = s_ctx.buffer_len;
    iperf_start_report();

    if (bw_lim > 0) {
        period_us = want_send * 8 / bw_lim;
    }

    while (!s_ctx.finish) {
        if (period_us > 0) {
            send_time = xf_sys_time_get_us();
            if (actual_send > 0) {
                // Last packet "send" was successful, check how much off the previous loop duration was to the ideal send period. Result will adjust the
                // next send delay.
                delay_us += period_us + (int32_t)(prev_time - send_time);
            } else {
                // Last packet "send" was not successful. Ideally we should try to catch up the whole previous loop duration (e.g. prev_time - send_time).
                // However, that's not possible since the most probable reason why the send was unsuccessful is the HW was not able to process the packet.
                // Hence, we cannot queue more packets with shorter (or no) delay to catch up since we are already at the performance edge. The best we
                // can do is to reset the send delay (which is probably big negative number) and start all over again.
                delay_us = 0;
            }
            prev_time = send_time;
        }
        *pkt_id_p = htonl(pkt_cnt++); // datagrams need to be sequentially numbered
        actual_send = sendto(send_socket, buffer, want_send, 0, dest_addr, socklen);
        if (actual_send != want_send) {
            if (type == IPERF_TRANS_TYPE_UDP) {
                err = iperf_get_socket_error_code(send_socket);
                // ENOMEM is expected under heavy load => do not print it
                if (err != ENOMEM) {
                    iperf_show_socket_error_reason(error_log, send_socket);
                }
            } else if (type == IPERF_TRANS_TYPE_TCP) {
                iperf_show_socket_error_reason(error_log, send_socket);
                break;
            }
        } else {
            s_ctx.actual_len_internal += actual_send;
        }
        // The send delay may be negative, it indicates we are trying to catch up and hence to not delay the loop at all.
        if (delay_us > 0) {
            xf_delay_us(delay_us);
        }
    }
}

static xf_err_t iperf_run_tcp_server(void)
{
    int listen_socket = -1;
    int client_socket = -1;
    int opt = 1;
    int err = 0;
    xf_err_t ret = XF_OK;
    struct sockaddr_in remote_addr;
    struct timeval timeout = { 0 };
    socklen_t addr_len = sizeof(struct sockaddr);
    struct sockaddr *listen_addr = NULL;
    struct sockaddr_in6 listen_addr6 = { 0 };
    struct sockaddr_in listen_addr4 = { 0 };

    XF_ASSERT_RET_GOTO((s_ctx.cfg.type == IPERF_IP_TYPE_IPV6 || s_ctx.cfg.type == IPERF_IP_TYPE_IPV4),
                       XF_FAIL, l_exit, TAG, "Ivalid AF types");

    if (s_ctx.cfg.type == IPERF_IP_TYPE_IPV6) {
        // The TCP server listen at the address "::", which means all addresses can be listened to.
        inet6_aton("::", &listen_addr6.sin6_addr);
        listen_addr6.sin6_family = AF_INET6;
        listen_addr6.sin6_port = htons(s_ctx.cfg.sport);

        listen_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_IPV6);
        XF_ASSERT_RET_GOTO((listen_socket >= 0), XF_FAIL, l_exit, TAG, "Unable to create socket: errno %d", errno);

        setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        setsockopt(listen_socket, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));

        XF_LOGI(TAG, "Socket created");

        err = bind(listen_socket, (struct sockaddr *)&listen_addr6, sizeof(listen_addr6));
        XF_ASSERT_RET_GOTO((err == 0), XF_FAIL, l_exit, TAG, "Socket unable to bind: errno %d, IPPROTO: %d", errno, AF_INET6);
        err = listen(listen_socket, 1);
        XF_ASSERT_RET_GOTO((err == 0), XF_FAIL, l_exit, TAG, "Error occurred during listen: errno %d", errno);

        timeout.tv_sec  = IPERF_SOCKET_RX_TIMEOUT_MS / 1000;
        timeout.tv_usec = (IPERF_SOCKET_RX_TIMEOUT_MS % 1000) * 1000;
        setsockopt(listen_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        listen_addr = (struct sockaddr *)&listen_addr6;
    } else if (s_ctx.cfg.type == IPERF_IP_TYPE_IPV4) {
        listen_addr4.sin_family = AF_INET;
        listen_addr4.sin_port = htons(s_ctx.cfg.sport);
        listen_addr4.sin_addr.s_addr = s_ctx.cfg.source_ip4;

        listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        XF_ASSERT_RET_GOTO((listen_socket >= 0), XF_FAIL, l_exit, TAG, "Unable to create socket: errno %d", errno);

        setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        XF_LOGI(TAG, "Socket created");

        err = bind(listen_socket, (struct sockaddr *)&listen_addr4, sizeof(listen_addr4));
        XF_ASSERT_RET_GOTO((err == 0), XF_FAIL, l_exit, TAG, "Socket unable to bind: errno %d, IPPROTO: %d", errno, AF_INET);

        err = listen(listen_socket, 5);
        XF_ASSERT_RET_GOTO((err == 0), XF_FAIL, l_exit, TAG, "Error occurred during listen: errno %d", errno);

        listen_addr = (struct sockaddr *)&listen_addr4;
    }

    client_socket = accept(listen_socket, (struct sockaddr *)&remote_addr, &addr_len);
    XF_ASSERT_RET_GOTO((client_socket >= 0), XF_FAIL, l_exit, TAG, "Unable to accept connection: errno %d", errno);
    XF_LOGI(TAG, "accept: %s,%d\n", inet_ntoa(remote_addr.sin_addr), htons(remote_addr.sin_port));

    timeout.tv_sec  = IPERF_SOCKET_RX_TIMEOUT_MS / 1000;
    timeout.tv_usec = (IPERF_SOCKET_RX_TIMEOUT_MS % 1000) * 1000;
    setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    socket_recv(client_socket, listen_addr, IPERF_TRANS_TYPE_TCP);
l_exit:
    if (client_socket != -1) {
        shutdown(client_socket, SHUT_RDWR);
        closesocket(client_socket);
    }

    if (listen_socket != -1) {
        shutdown(listen_socket, SHUT_RDWR);
        closesocket(listen_socket);
        XF_LOGI(TAG, "TCP Socket server is closed.");
    }
    s_ctx.finish = true;
    return ret;
}

static xf_err_t iperf_run_tcp_client(void)
{
    int client_socket = -1;
    int err = 0;
    xf_err_t ret = XF_OK;
    struct sockaddr *dest_addr = NULL;
    struct sockaddr_in6 dest_addr6 = { 0 };
    struct sockaddr_in dest_addr4 = { 0 };

    XF_ASSERT_RET_GOTO((s_ctx.cfg.type == IPERF_IP_TYPE_IPV6 || s_ctx.cfg.type == IPERF_IP_TYPE_IPV4),
                       XF_FAIL, l_exit, TAG, "Ivalid AF types");

    if (s_ctx.cfg.type == IPERF_IP_TYPE_IPV6) {
        client_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_IPV6);
        XF_ASSERT_RET_GOTO((client_socket >= 0), XF_FAIL, l_exit, TAG, "Unable to create socket: errno %d", errno);

        inet6_aton(s_ctx.cfg.destination_ip6, &dest_addr6.sin6_addr);
        dest_addr6.sin6_family = AF_INET6;
        dest_addr6.sin6_port = htons(s_ctx.cfg.dport);

        err = connect(client_socket, (struct sockaddr *)&dest_addr6, sizeof(struct sockaddr_in6));
        XF_ASSERT_RET_GOTO((err == 0), XF_FAIL, l_exit, TAG, "Socket unable to connect: errno %d", errno);
        XF_LOGI(TAG, "Successfully connected");

        dest_addr = (struct sockaddr *)&dest_addr6;
    } else if (s_ctx.cfg.type == IPERF_IP_TYPE_IPV4) {
        client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        XF_ASSERT_RET_GOTO((client_socket >= 0), XF_FAIL, l_exit, TAG, "Unable to create socket: errno %d", errno);

        dest_addr4.sin_family = AF_INET;
        dest_addr4.sin_port = htons(s_ctx.cfg.dport);
        dest_addr4.sin_addr.s_addr = s_ctx.cfg.destination_ip4;

        err = connect(client_socket, (struct sockaddr *)&dest_addr4, sizeof(struct sockaddr_in));
        XF_ASSERT_RET_GOTO((err == 0), XF_FAIL, l_exit, TAG, "Socket unable to connect: errno %d", errno);
        XF_LOGI(TAG, "Successfully connected");

        dest_addr = (struct sockaddr *)&dest_addr4;
    }

    socket_send(client_socket, dest_addr, IPERF_TRANS_TYPE_TCP, s_ctx.cfg.bw_lim);
l_exit:
    if (client_socket != -1) {
        shutdown(client_socket, SHUT_RDWR);
        closesocket(client_socket);
        XF_LOGI(TAG, "TCP Socket client is closed.");
    }
    s_ctx.finish = true;
    return ret;
}

static xf_err_t iperf_run_udp_server(void)
{
    int listen_socket = -1;
    int opt = 1;
    int err = 0;
    xf_err_t ret = XF_OK;
    struct timeval timeout = { 0 };
    struct sockaddr *listen_addr = NULL;
    struct sockaddr_in6 listen_addr6 = { 0 };
    struct sockaddr_in listen_addr4 = { 0 };

    XF_ASSERT_RET_GOTO((s_ctx.cfg.type == IPERF_IP_TYPE_IPV6 || s_ctx.cfg.type == IPERF_IP_TYPE_IPV4),
                       XF_FAIL, l_exit, TAG, "Ivalid AF types");

    if (s_ctx.cfg.type == IPERF_IP_TYPE_IPV6) {
        // The UDP server listen at the address "::", which means all addresses can be listened to.
        inet6_aton("::", &listen_addr6.sin6_addr);
        listen_addr6.sin6_family = AF_INET6;
        listen_addr6.sin6_port = htons(s_ctx.cfg.sport);

        listen_socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
        XF_ASSERT_RET_GOTO((listen_socket >= 0), XF_FAIL, l_exit, TAG, "Unable to create socket: errno %d", errno);
        XF_LOGI(TAG, "Socket created");

        setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        err = bind(listen_socket, (struct sockaddr *)&listen_addr6, sizeof(struct sockaddr_in6));
        XF_ASSERT_RET_GOTO((err == 0), XF_FAIL, l_exit, TAG, "Socket unable to bind: errno %d", errno);
        XF_LOGI(TAG, "Socket bound, port %d", listen_addr6.sin6_port);

        listen_addr = (struct sockaddr *)&listen_addr6;
    } else if (s_ctx.cfg.type == IPERF_IP_TYPE_IPV4) {
        listen_addr4.sin_family = AF_INET;
        listen_addr4.sin_port = htons(s_ctx.cfg.sport);
        listen_addr4.sin_addr.s_addr = s_ctx.cfg.source_ip4;

        listen_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        XF_ASSERT_RET_GOTO((listen_socket >= 0), XF_FAIL, l_exit, TAG, "Unable to create socket: errno %d", errno);
        XF_LOGI(TAG, "Socket created");

        setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        err = bind(listen_socket, (struct sockaddr *)&listen_addr4, sizeof(struct sockaddr_in));
        XF_ASSERT_RET_GOTO((err == 0), XF_FAIL, l_exit, TAG, "Socket unable to bind: errno %d", errno);
        XF_LOGI(TAG, "Socket bound, port %d", listen_addr4.sin_port);

        listen_addr = (struct sockaddr *)&listen_addr4;
    }

    /* UDP 接收一直轮询，避免长时间阻塞影响 s_ctx.finish 的检测，任务无法及时退出  */
    timeout.tv_sec  = 0;
    timeout.tv_usec = 1000;
    setsockopt(listen_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    socket_recv(listen_socket, listen_addr, IPERF_TRANS_TYPE_UDP);
l_exit:
    if (listen_socket != -1) {
        shutdown(listen_socket, SHUT_RDWR);
        closesocket(listen_socket);
    }
    XF_LOGI(TAG, "Udp socket server is closed.");
    s_ctx.finish = true;
    return ret;
}

static xf_err_t iperf_run_udp_client(void)
{
    int client_socket = -1;
    int opt = 1;
    xf_err_t ret = XF_OK;
    struct sockaddr *dest_addr = NULL;
    struct sockaddr_in6 dest_addr6 = { 0 };
    struct sockaddr_in dest_addr4 = { 0 };

    XF_ASSERT_RET_GOTO((s_ctx.cfg.type == IPERF_IP_TYPE_IPV6 || s_ctx.cfg.type == IPERF_IP_TYPE_IPV4),
                       XF_FAIL, l_exit, TAG, "Ivalid AF types");

    if (s_ctx.cfg.type == IPERF_IP_TYPE_IPV6) {
        inet6_aton(s_ctx.cfg.destination_ip6, &dest_addr6.sin6_addr);
        dest_addr6.sin6_family = AF_INET6;
        dest_addr6.sin6_port = htons(s_ctx.cfg.dport);

        client_socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_IPV6);
        XF_ASSERT_RET_GOTO((client_socket >= 0), XF_FAIL, l_exit, TAG, "Unable to create socket: errno %d", errno);
        XF_LOGI(TAG, "Socket created, sending to %s:%d", s_ctx.cfg.destination_ip6, s_ctx.cfg.dport);

        setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        dest_addr = (struct sockaddr *)&dest_addr6;
    } else if (s_ctx.cfg.type == IPERF_IP_TYPE_IPV4) {
        dest_addr4.sin_family = AF_INET;
        dest_addr4.sin_port = htons(s_ctx.cfg.dport);
        dest_addr4.sin_addr.s_addr = s_ctx.cfg.destination_ip4;

        client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        XF_ASSERT_RET_GOTO((client_socket >= 0), XF_FAIL, l_exit, TAG, "Unable to create socket: errno %d", errno);
        XF_LOGI(TAG, "Socket created, sending to " XF_IPSTR ":%d",
                XF_IP2STR((xf_ip4_addr_t *)&s_ctx.cfg.destination_ip4), s_ctx.cfg.dport);

        setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        dest_addr = (struct sockaddr *)&dest_addr4;
    }

    socket_send(client_socket, dest_addr, IPERF_TRANS_TYPE_UDP, s_ctx.cfg.bw_lim);
l_exit:
    if (client_socket != -1) {
        shutdown(client_socket, SHUT_RDWR);
        closesocket(client_socket);
    }
    s_ctx.finish = true;
    XF_LOGI(TAG, "UDP Socket client is closed");
    return ret;
}

static void iperf_task_traffic(void *arg)
{
    if (iperf_is_udp_client()) {
        iperf_run_udp_client();
    } else if (iperf_is_udp_server()) {
        iperf_run_udp_server();
    } else if (iperf_is_tcp_client()) {
        iperf_run_tcp_client();
    } else {
        iperf_run_tcp_server();
    }

    if (s_ctx.buffer) {
        xf_free(s_ctx.buffer);
        s_ctx.buffer = NULL;
    }
    s_ctx.traffic_is_running = false;
    XF_LOGI(TAG, "iperf done");
    xf_osal_thread_delete(NULL);
}

static uint32_t iperf_get_buffer_len(void)
{
    if (iperf_is_udp_client()) {
#ifdef CONFIG_LWIP_IPV6
        if (s_ctx.cfg.len_send_buf) {
            return s_ctx.cfg.len_send_buf;
        } else if (s_ctx.cfg.type == IPERF_IP_TYPE_IPV6) {
            return IPERF_DEFAULT_IPV6_UDP_TX_LEN;
        } else {
            return IPERF_DEFAULT_IPV4_UDP_TX_LEN;
        }
#else
        return (s_ctx.cfg.len_send_buf == 0
                ? IPERF_DEFAULT_IPV4_UDP_TX_LEN
                : s_ctx.cfg.len_send_buf);
#endif
    } else if (iperf_is_udp_server()) {
        return IPERF_DEFAULT_UDP_RX_LEN;
    } else if (iperf_is_tcp_client()) {
        return (s_ctx.cfg.len_send_buf == 0
                ? IPERF_DEFAULT_TCP_TX_LEN
                : s_ctx.cfg.len_send_buf);
    } else {
        return IPERF_DEFAULT_TCP_RX_LEN;
    }
    return 0;
}

static uint32_t iperf_get_float_int(float in)
{
    return (uint32_t)(((uint64_t)(in * IPERF_HUNDRED)) / IPERF_HUNDRED);
}

static uint32_t iperf_get_float_dec(float in)
{
    return (uint32_t)(((uint64_t)(in * IPERF_HUNDRED)) % IPERF_HUNDRED);
}
