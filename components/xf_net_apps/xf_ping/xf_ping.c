/**
 * @file xf_ping.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-10-12
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include <stdlib.h>
#include <stdbool.h>

#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/mem.h"
#include "lwip/icmp.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"
#include "lwip/inet.h"
#include "lwip/inet_chksum.h"
#include "lwip/ip.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include "xf_sys.h"
#include "xf_ping.h"

/* ==================== [Defines] =========================================== */

#define PING_TIME_DIFF_MS(_end, _start) \
    ((uint32_t)((_end - _start) / 1000))

#define PING_CHECK_START_TIMEOUT_MS (100)

#define PING_FLAGS_INIT         (1 << 0)
#define PING_FLAGS_RUN          (1 << 1)

#define THREAD_NOTIFY_BIT       0x01

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static xf_err_t xf_ping_send(xf_ping_ctx_t *ctx);
static int xf_ping_receive(xf_ping_ctx_t *ctx);
static void xf_ping_thread(void *args);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "xf_ping";

/* ==================== [Macros] ============================================ */

#define XF_GOTO_ON_FALSE(a, err_code, goto_tag, log_tag, format, ...) \
    do { \
        if (unlikely(!(a))) { \
            XF_LOGE(log_tag, format, ##__VA_ARGS__); \
            ret = err_code; \
            goto goto_tag; \
        } \
    } while (0)

/* ==================== [Global Functions] ================================== */

xf_err_t xf_ping_new_session(
    const xf_ping_cfg_t *p_cfg, xf_ping_cb_t cb_func, void *user_args,
    xf_ping_t *hdl_out)
{
    xf_err_t ret = XF_FAIL;
    xf_ping_ctx_t *ctx = NULL;

    XF_GOTO_ON_FALSE(p_cfg, XF_ERR_INVALID_ARG, l_err,
                     TAG, "p_cfg-%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    XF_GOTO_ON_FALSE(hdl_out, XF_ERR_INVALID_ARG, l_err,
                     TAG, "hdl_out-%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    ctx = xf_malloc(sizeof(xf_ping_ctx_t));
    XF_GOTO_ON_FALSE(ctx, XF_ERR_NO_MEM, l_err,
                     TAG, "ping_ctx-%s", xf_err_to_name(XF_ERR_NO_MEM));
    xf_memset(ctx, 0x00, sizeof(xf_ping_ctx_t));

    /* 防止 ping 任务退出 */
    BITS_SET1(ctx->flags, PING_FLAGS_INIT);

    ctx->task_exit_flag = false;
    xf_osal_thread_attr_t thread_attr = {
        .name       = XF_PING_THREAD_NAME,
        .priority   = p_cfg->task_prio,
        .stack_size = p_cfg->task_stack_size,
    };
    ctx->task_hdl = xf_osal_thread_create(xf_ping_thread, ctx, &thread_attr);
    XF_GOTO_ON_FALSE(ctx->task_hdl != NULL, XF_ERR_NO_MEM, l_err,
                     TAG, "ping_task-%s", xf_err_to_name(XF_ERR_NO_MEM));

    if (cb_func) {
        ctx->cb_func    = cb_func;
        ctx->user_args  = user_args;
    }
    ctx->recv_addr          = p_cfg->target_addr;
    ctx->target_addr_type   = p_cfg->target_addr.type;
    ctx->count              = p_cfg->count;
    ctx->interval_ms        = p_cfg->interval_ms;
    ctx->icmp_pkt_size      = sizeof(struct icmp_echo_hdr) + p_cfg->data_size;
    ctx->packet_hdr         = xf_malloc(ctx->icmp_pkt_size);
    XF_GOTO_ON_FALSE(ctx->packet_hdr, XF_ERR_NO_MEM, l_err,
                     TAG, "icmp_pkt-%s", xf_err_to_name(XF_ERR_NO_MEM));
    xf_memset(ctx->packet_hdr, 0x00, ctx->icmp_pkt_size);

    ctx->packet_hdr->code = 0;
    ctx->packet_hdr->id = ((uint32_t)ctx->task_hdl) & 0xFFFF; /*!< 任务句柄唯一，视为 ping ID  */

    char *d = (char *)(ctx->packet_hdr) + sizeof(struct icmp_echo_hdr);
    for (uint32_t i = 0; i < p_cfg->data_size; i++) {
        d[i] = 'A' + i;
    }

    /* 创建 socket */
    if (IP_IS_V4(&p_cfg->target_addr)
#if CONFIG_LWIP_IPV6
            || ip6_addr_isipv4mappedipv6(ip_2_ip6(&p_cfg->target_addr))
#endif
       ) {
        ctx->sock = socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP);
    }
#if CONFIG_LWIP_IPV6
    else {
        ctx->sock = socket(AF_INET6, SOCK_RAW, IP6_NEXTH_ICMP6);
    }
#endif
    XF_GOTO_ON_FALSE(ctx->sock >= 0, XF_FAIL, l_err,
                     TAG, "create socket failed: %d", ctx->sock);

    if (p_cfg->interface) {
        struct ifreq iface;
        if (netif_index_to_name(p_cfg->interface, iface.ifr_name) == NULL) {
            XF_LOGE(TAG, "fail to find interface name with netif index %d",
                    p_cfg->interface);
            goto l_err;
        }
        if (setsockopt(ctx->sock, SOL_SOCKET, SO_BINDTODEVICE, &iface, sizeof(iface)) != 0) {
            XF_LOGE(TAG, "fail to setsockopt SO_BINDTODEVICE");
            goto l_err;
        }
    }

    struct timeval timeout;
    timeout.tv_sec  = p_cfg->timeout_ms / 1000;
    timeout.tv_usec = (p_cfg->timeout_ms % 1000) * 1000;
    setsockopt(ctx->sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(ctx->sock, IPPROTO_IP, IP_TOS, &p_cfg->tos, sizeof(p_cfg->tos));
    setsockopt(ctx->sock, IPPROTO_IP, IP_TTL, &p_cfg->ttl, sizeof(p_cfg->ttl));

    if (IP_IS_V4(&p_cfg->target_addr)) {
        struct sockaddr_in *to4 = (struct sockaddr_in *)&ctx->target_addr;
        to4->sin_family = AF_INET;
        inet_addr_from_ip4addr(&to4->sin_addr, ip_2_ip4(&p_cfg->target_addr));
        ctx->packet_hdr->type = ICMP_ECHO;
    }
#if CONFIG_LWIP_IPV6
    if (IP_IS_V6(&p_cfg->target_addr)) {
        struct sockaddr_in6 *to6 = (struct sockaddr_in6 *)&ctx->target_addr;
        to6->sin6_family = AF_INET6;
        inet6_addr_from_ip6addr(&to6->sin6_addr, ip_2_ip6(&p_cfg->target_addr));
        ctx->packet_hdr->type = ICMP6_TYPE_EREQ;
    }
#endif

    *hdl_out = (xf_ping_t)ctx;

    return XF_OK;

l_err:
    if (ctx) {
        if (ctx->sock > 0) {
            closesocket(ctx->sock);
        }
        if (ctx->packet_hdr) {
            xf_free(ctx->packet_hdr);
        }
        if (ctx->task_hdl) {
            xf_osal_thread_delete(ctx->task_hdl);
        }
        xf_free(ctx);
    }
    return ret;
}

xf_err_t xf_ping_delete_session(xf_ping_t hdl)
{
    xf_err_t ret = XF_OK;
    xf_ping_ctx_t *ctx = (xf_ping_ctx_t *)hdl;
    XF_GOTO_ON_FALSE(ctx, XF_ERR_INVALID_ARG, l_err,
                     TAG, "hdl-%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    BITS_SET0(ctx->flags, PING_FLAGS_INIT);

    size_t wait_cnt = 0;
#define WAIT_CNT_MAX    1000
    while (ctx->task_exit_flag != true) {
        xf_osal_delay_ms(10);
        wait_cnt++;
        if (wait_cnt >= WAIT_CNT_MAX) {
            xf_osal_thread_delete(ctx->task_hdl);
            xf_osal_delay_ms(50);
            break;
        }
    }

    xf_free(ctx);

    return XF_OK;
l_err:
    return ret;
}

xf_err_t xf_ping_start(xf_ping_t hdl)
{
    xf_err_t ret = XF_OK;
    xf_ping_ctx_t *ctx = (xf_ping_ctx_t *)hdl;
    XF_GOTO_ON_FALSE(ctx, XF_ERR_INVALID_ARG, l_err,
                     TAG, "hdl-%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    BITS_SET1(ctx->flags, PING_FLAGS_RUN);
    xf_osal_thread_notify_set(ctx->task_hdl, THREAD_NOTIFY_BIT);

    return XF_OK;
l_err:
    return ret;
}

bool xf_ping_is_running(xf_ping_t hdl)
{
    bool ret = false;
    xf_ping_ctx_t *ctx = (xf_ping_ctx_t *)hdl;
    XF_GOTO_ON_FALSE(ctx, false, l_err,
                     TAG, "hdl-%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    return ctx->ping_running_flag;
l_err:
    return ret;
}

xf_err_t xf_ping_stop(xf_ping_t hdl)
{
    xf_err_t ret = XF_OK;
    xf_ping_ctx_t *ctx = (xf_ping_ctx_t *)hdl;
    XF_GOTO_ON_FALSE(ctx, XF_ERR_INVALID_ARG, l_err,
                     TAG, "hdl-%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    BITS_SET0(ctx->flags, PING_FLAGS_RUN);

    return XF_OK;
l_err:
    return ret;
}

/* ==================== [Static Functions] ================================== */

static xf_err_t xf_ping_send(xf_ping_ctx_t *ctx)
{
    xf_err_t ret = XF_OK;

    ctx->packet_hdr->seqno++;
    /* seqno 已更改，重新计算校验和 */
    ctx->packet_hdr->chksum = 0;
    if (ctx->packet_hdr->type == ICMP_ECHO) {
        ctx->packet_hdr->chksum = inet_chksum(ctx->packet_hdr, ctx->icmp_pkt_size);
    }

    socklen_t socklen = (ctx->target_addr_type == IPADDR_TYPE_V4)
                        ? sizeof(struct sockaddr_in)
                        : sizeof(struct sockaddr_in6);
    ssize_t sent = sendto(ctx->sock, ctx->packet_hdr, ctx->icmp_pkt_size, 0,
                          (struct sockaddr *)&ctx->target_addr, socklen);

    if (sent != (ssize_t)ctx->icmp_pkt_size) {
        int opt_val;
        socklen_t opt_len = sizeof(opt_val);
        getsockopt(ctx->sock, SOL_SOCKET, SO_ERROR, &opt_val, &opt_len);
        XF_LOGE(TAG, "send error=%d", opt_val);
        ret = XF_FAIL;
    } else {
        ctx->transmitted++;
    }
    return ret;
}

static int xf_ping_receive(xf_ping_ctx_t *ctx)
{
    char buf[64]; /*!< sock buffer, 64 字节足以容纳 IP 头和 ICMP 头 */
    int len = 0;
    struct sockaddr_storage from;

    int fromlen = (ctx->target_addr_type == IPADDR_TYPE_V4)
                  ? sizeof(struct sockaddr_in)
                  : sizeof(struct sockaddr_in6);
    uint16_t data_head = 0;

    while ((len = recvfrom(ctx->sock, buf, sizeof(buf), 0,
                           (struct sockaddr *)&from, (socklen_t *)&fromlen)) > 0) {
        if (from.ss_family == AF_INET) {
            // IPv4
            struct sockaddr_in *from4 = (struct sockaddr_in *)&from;
            inet_addr_to_ip4addr(ip_2_ip4(&ctx->recv_addr), &from4->sin_addr);
            IP_SET_TYPE_VAL(ctx->recv_addr, IPADDR_TYPE_V4);
            data_head = (uint16_t)(sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr));
        }
#if CONFIG_LWIP_IPV6
        else {
            // IPv6
            struct sockaddr_in6 *from6 = (struct sockaddr_in6 *)&from;
            inet6_addr_to_ip6addr(ip_2_ip6(&ctx->recv_addr), &from6->sin6_addr);
            IP_SET_TYPE_VAL(ctx->recv_addr, IPADDR_TYPE_V6);
            data_head = (uint16_t)(sizeof(struct ip6_hdr) + sizeof(struct icmp6_echo_hdr));
        }
#endif
        if (len >= data_head) {
            if (IP_IS_V4_VAL(ctx->recv_addr)) {
                struct ip_hdr *iphdr = (struct ip_hdr *)buf;
                struct icmp_echo_hdr *iecho = (struct icmp_echo_hdr *)(buf + (IPH_HL(iphdr) * 4));
                if ((iecho->id == ctx->packet_hdr->id) && (iecho->seqno == ctx->packet_hdr->seqno)) {
                    ctx->received++;
                    ctx->ttl = iphdr->_ttl;
                    ctx->tos = iphdr->_tos;
                    ctx->recv_len = lwip_ntohs(IPH_LEN(iphdr)) - data_head;  /*!< ICMP 的数据部分 */
                    return len;
                }
            }
#if CONFIG_LWIP_IPV6
            else if (IP_IS_V6_VAL(ctx->recv_addr)) {
                struct ip6_hdr *iphdr = (struct ip6_hdr *)buf;
                struct icmp6_echo_hdr *iecho6 = (struct icmp6_echo_hdr *)(buf + sizeof(struct ip6_hdr)); /*!< IPv6 头长度为 40 */
                if ((iecho6->id == ctx->packet_hdr->id) && (iecho6->seqno == ctx->packet_hdr->seqno)) {
                    ctx->received++;
                    ctx->recv_len = IP6H_PLEN(iphdr) - sizeof(struct icmp6_echo_hdr);  /*!< ICMP 的数据部分 */
                    return len;
                }
            }
#endif
        }
        fromlen = (ctx->target_addr_type == IPADDR_TYPE_V4)
                  ? sizeof(struct sockaddr_in)
                  : sizeof(struct sockaddr_in6);
    }
    /* 如果超时，len 将为 -1 */
    return len;
}

static void xf_ping_thread(void *args)
{
    xf_ping_ctx_t *ctx = (xf_ping_ctx_t *)(args);
    int recv_ret;
    xf_err_t xf_ret = XF_OK;
    uint32_t wait_ticks = xf_osal_kernel_ms_to_ticks(PING_CHECK_START_TIMEOUT_MS);
    uint32_t interval_ticks;  /*!< 在循环内更新，此处赋值有可能无法获得实际值 */
    uint32_t last_wake;
    xf_us_t start_time, end_time;

    for (;;) {
        if (!(ctx->flags & PING_FLAGS_INIT)) {
            break;
        }
        /* 此处 PING_FLAGS_RUN 避免长时间等待通知 */
        if (!(ctx->flags & PING_FLAGS_RUN)) {
            xf_osal_delay_ms(50);
            continue;
        }
        xf_ret = xf_osal_thread_notify_wait(THREAD_NOTIFY_BIT, XF_OSAL_WAIT_ANY, wait_ticks);
        if (xf_ret != XF_OK) {
            continue;
        }
        UNUSED(xf_osal_thread_notify_get());
        xf_osal_thread_notify_clear(THREAD_NOTIFY_BIT);

        ctx->packet_hdr->seqno  = 0;
        ctx->transmitted        = 0;
        ctx->received           = 0;
        ctx->total_time_ms      = 0;

        /* 此处 PING_FLAGS_RUN 用于在运行时暂停 */
        while ((ctx->flags & PING_FLAGS_RUN)
                && ((ctx->count == 0xFFFFFFFF) || (ctx->packet_hdr->seqno < ctx->count))) {
            last_wake = xf_osal_kernel_get_tick_count();

            /*
                ctx->flags 告诉 xf_ping_thread ping 能否运行；
                ctx->ping_running_flag 指示 ping 是否正在运行。
             */
            ctx->ping_running_flag = true;

            xf_ping_send(ctx);

            start_time  = xf_sys_time_get_us();
            recv_ret    = xf_ping_receive(ctx);
            end_time    = xf_sys_time_get_us();

            ctx->elapsed_time_ms    = PING_TIME_DIFF_MS(end_time, start_time);
            ctx->total_time_ms      += ctx->elapsed_time_ms;

            if (ctx->cb_func) {
                xf_ping_event_id_t event_id = (recv_ret >= 0) ? (XF_PING_EVENT_SUCC) : (XF_PING_EVENT_TIMEOUT);
                ctx->cb_func(event_id, (void *)ctx, ctx->user_args);
            }

            interval_ticks = xf_osal_kernel_ms_to_ticks(ctx->interval_ms);
            if (interval_ticks) {
                xf_osal_delay_until(last_wake + interval_ticks);
            }
        } /* while */

        BITS_SET0(ctx->flags, PING_FLAGS_RUN);
        ctx->ping_running_flag = false;

        if (ctx->cb_func) {
            ctx->cb_func(XF_PING_EVENT_END, (void *)ctx, ctx->user_args);
        }
    }
    if (ctx->packet_hdr) {
        xf_free(ctx->packet_hdr);
    }
    if (ctx->sock >= 0) {
        closesocket(ctx->sock);
    }
    ctx->task_exit_flag = true;
    /* ctx 在 xf_ping_delete_session() 内释放 */
    xf_osal_thread_delete(NULL);
}
