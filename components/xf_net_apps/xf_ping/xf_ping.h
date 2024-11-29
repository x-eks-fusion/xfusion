/**
 * @file xf_ping.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-10-12
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/**
 * @cond (XFAPI_USER || XFAPI_PORT || XFAPI_INTERNAL)
 * @ingroup group_xf_net_apps
 * @defgroup group_xf_ping xf_ping
 * @brief ping 功能实现。
 * @endcond
 */

#ifndef __XF_PING_H__
#define __XF_PING_H__

/* ==================== [Includes] ========================================== */

#include "lwip/ip_addr.h"
#include "lwip/icmp.h"

#include "xf_osal.h"
#include "xf_utils.h"

/**
 * @cond XFAPI_USER
 * @addtogroup group_xf_ping
 * @endcond
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_PING_THREAD_NAME     "xf_ping"

/* ==================== [Typedefs] ========================================== */

/**
 * @brief ping 会话上下文的预声明。
 */
struct _xf_ping_ctx_t;

/**
 * @brief ping 会话句柄。
 */
typedef struct _xf_ping_ctx_t *xf_ping_t;

/**
 * @brief ping 配置类型。
 */
typedef struct _xf_ping_cfg_t {
    uint32_t count;             /*!< ping 次数，0xFFFFFFFF 为无限循环。 */
    uint32_t interval_ms;       /*!< 每两个 ping 的间隔时间(单位 ms)。 */
    uint32_t timeout_ms;        /*!< 每次 ping 的超时时间(单位 ms)。 */
    uint32_t data_size;         /*!< ICMP 数据包标头旁边的数据大小。 */
    uint8_t tos;                /*!< 服务类型，IP 标头中指定的字段。 */
    uint8_t ttl;                /*!< 生存时间，IP 标头中指定的字段。 */
    ip_addr_t target_addr;      /*!< 目标 IP 地址，IPv4 或 IPv6。 */
    uint32_t task_stack_size;   /*!< 内部 ping 任务的堆栈大小。 */
    uint32_t task_prio;         /*!< 内部 ping 任务的优先级。 */
    uint32_t interface;         /*!< Netif 索引，interface=0 表示 NETIF_NO_INDEX。 */
    uint8_t auto_delete_flag;   /*!< XF_PING_EVENT_END 事件发生后，是否自动删除会话。 */
} xf_ping_cfg_t;

/**
 * @brief ping 更新配置标志。
 */
typedef struct _xf_ping_cfg_update_flags_t {
    uint32_t b_count            : 1;    /*!< 更新 ping 次数。 */
    uint32_t b_interval_ms      : 1;    /*!< 更新间隔时间。 */
    uint32_t b_timeout_ms       : 1;    /*!< 更新超时时间。 */
    uint32_t b_tos              : 1;    /*!< 更新服务类型。 */
    uint32_t b_ttl              : 1;    /*!< 更新生存时间。 */
    uint32_t b_target_addr      : 1;    /*!< 更新目标 IP 地址。 */
    uint32_t b_interface        : 1;    /*!< 更新 Netif 索引。 */
    uint32_t b_auto_delete_flag : 1;    /*!< 更新是否自动删除会话。 */
    uint32_t reserve            : 24;   /*!< 保留位。 */
} xf_ping_cfg_update_flags_t;

/**
 * @brief 默认 ping 配置
 */
#define XF_PING_DEFAULT_CONFIG() (xf_ping_cfg_t) { \
        .count = 5, \
        .interval_ms = 1000, \
        .timeout_ms = 1000, \
        .data_size = 64, \
        .tos = 0, \
        .ttl = IP_DEFAULT_TTL, \
        .target_addr = *(IP_ANY_TYPE), \
        .task_stack_size = (4 * 1024), \
        .task_prio = XF_OSAL_PRIORITY_NORMOL, \
        .interface = 0, \
        .auto_delete_flag = 0, \
    }

/**
 * @brief Ping 事件声明。
 *
 * @attention
 *      1.  注意，出于安全性考虑，禁止在回调中删除会话。
 *      2.  如果需要一次性 ping 会话，请设置 `<xf_ping_cfg_t>.auto_delete_flag = true`.
 *          将会在运行一次结束后，内部 task 自动删除 ping 会话。
 *          此时注意通过 `XF_PING_EVENT_DELETE` 事件将 `xf_ping_new_session()` 传出的句柄
 *          置为空指针，防止野指针。
 *          设置了 `<xf_ping_cfg_t>.auto_delete_flag = true` 后，不需要再调用
 *          `xf_ping_delete_session()`。
 *      3.  如果 `<xf_ping_cfg_t>.auto_delete_flag = false`，
 *          当一个 ping 会话不再需要后，请通过 `xf_ping_delete_session()` 删除会话，
 *          并将外部保存的句柄置为 NULL。
 */
typedef enum _xf_ping_event_code_t {
    XF_PING_EVENT_SUCC = 0x00,          /*!< 收到 ICMP 应答数据包。 */
    XF_PING_EVENT_TIMEOUT,              /*!< ICMP 应答数据包超时。 */
    XF_PING_EVENT_END,                  /*!< ping 会话完成。 */
    XF_PING_EVENT_DELETE,               /*!< ping 会话已删除。 */

    XF_PING_EVENT_MAX,                  /*!< ping 事件 ID 最大值，无效 ID。 */
} xf_ping_event_code_t;

/**
 * @brief ping 事件 id。见 @ref xf_ping_event_code_t.
 */
typedef int32_t xf_ping_event_id_t;

/**
 * @brief ping 回调函数原型。
 *
 * @param event_id 事件 id. 根据事件类型, 见 @ref xf_ping_event_code_t.
 * @param hdl 事件数据. 需要根据事件 id 强转为对应的类型.
 * @param user_args 用户数据. `xf_ping_new_session()` 时传入.
 */
typedef void (*xf_ping_cb_t)(
    xf_ping_event_id_t event_id, xf_ping_t hdl, void *user_args);

/**
 * @brief ping 会话的上下文。
 * @attention 只有 `public:` 部分是在回调中可读的，
 *            用户 **禁止** 修改其中任何内容。
 */
typedef struct _xf_ping_ctx_t {
    /* public: */
    /* 只读数据 */
    struct icmp_echo_hdr *packet_hdr;   /*!< ping 序列号见 icmp_echo_hdr.seqno. */
    ip_addr_t recv_addr;                /*!< 回复的 IP 地址。 */
    uint32_t recv_len;                  /*!< 接收到的数据包的大小。 */
    uint32_t elapsed_time_ms;           /*!< 请求和回复数据包之间经过的时间。 */
    uint32_t total_time_ms;             /*!< 整个 ping 会话所用的时间。 */
    uint32_t interval_ms;               /*!< 每两个 ping 的间隔时间(单位 ms)。 */
    uint32_t count;                     /*!< ping 会话当前计数。 */
    uint32_t transmitted;               /*!< 发出的请求数据包数量。 */
    uint32_t received;                  /*!< 收到的回复数据包数量。 */
    uint8_t ttl;                        /*!< ping 生存时间。 */
    uint8_t tos;                        /*!< ping 服务类型。 */
    uint8_t auto_delete_flag;           /*!< XF_PING_EVENT_END 事件发生后，是否自动删除会话。 */

    /* privte: */
    int sock;                           /*!< socket 文件描述符。 */
    /*
        struct sockaddr_storage 太大，在某些芯片上(ws63)无法正常收发，不使用。
        struct sockaddr_storage target_addr;
     */
    union sockaddr_in_in6_u {
        struct sockaddr_in in;
        struct sockaddr_in6 in6;
    } target_addr;                      /*!< 目标 sockaddr. */
    uint8_t target_addr_type;           /*!< 目标 sockaddr 的类型。 */

    uint32_t flags;                     /*!< 内部标志。 */
    uint32_t icmp_pkt_size;             /*!< ICMP 包长。 */

    xf_osal_thread_t task_hdl;          /*!< 内部任务句柄。 */
    uint8_t task_exit_flag;             /*!< 内部任务结束标志。 */
    uint8_t ping_running_flag;          /*!< ping 是否正在进行。 */

    xf_ping_cb_t cb_func;               /*!< 传入的回调函数 。*/
    void *user_args;                    /*!< 传入的回调函数的用户参数。 */
} xf_ping_ctx_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 创建 ping 会话。
 *
 * @param p_cfg ping 配置。
 * @param cb_func ping 事件回调。
 * @param user_args ping 事件回调中的用户参数。
 * @param[out] hdl_out ping 会话句柄。
 * @return
 *      - XF_ERR_INVALID_ARG 无效参数（例如配置为空等）
 *      - XF_ERR_NO_MEM 内存不足
 *      - XF_FAIL 其他内部错误（例如套接字错误）
 *      - XF_OK 成功
 */
xf_err_t xf_ping_new_session(
    const xf_ping_cfg_t *p_cfg, xf_ping_cb_t cb_func, void *user_args,
    xf_ping_t *hdl_out);

/**
 * @brief 删除 ping 会话。
 *
 * @param hdl ping 会话句柄。
 * @return
 *      - XF_ERR_INVALID_ARG 无效参数
 *      - XF_OK 成功
 */
xf_err_t xf_ping_delete_session(xf_ping_t hdl);

/**
 * @brief 启动 ping 会话。
 *
 * @param hdl ping 会话句柄
 * @return
 *      - XF_ERR_INVALID_ARG 无效参数
 *      - XF_OK 成功
 */
xf_err_t xf_ping_start(xf_ping_t hdl);

/**
 * @brief 重新启动 ping 会话。
 *
 * @param hdl ping 会话句柄
 * @param p_cfg 需要更新的 ping 配置，为 NULL 时跳过更新配置。
 * @param p_flags ping 配置中需要更新的部分。
 * @return
 *      - XF_ERR_INVALID_ARG 无效参数
 *      - XF_ERR_BUSY ping 会话正在运行，无法重新开始
 *      - XF_OK 成功
 */
xf_err_t xf_ping_restart(
    xf_ping_t hdl, const xf_ping_cfg_t *p_cfg, xf_ping_cfg_update_flags_t *p_flags);

/**
 * @brief 检查 ping 是否正在运行。
 *
 * @param hdl ping 会话句柄
 * @return
 *      - true 是
 *      - false 否
 */
bool xf_ping_is_running(xf_ping_t hdl);

/**
 * @brief 停止 ping 会话。
 *
 * @param hdl ping 会话句柄
 * @return
 *      - XF_ERR_INVALID_ARG 无效参数
 *      - XF_OK 成功
 */
xf_err_t xf_ping_stop(xf_ping_t hdl);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_ping
 * @}
 */

#endif /* __XF_PING_H__ */
