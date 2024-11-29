/**
 * @file xf_iperf.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-09-26
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* Iperf Example - iperf declaration

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/**
 * @cond (XFAPI_USER || XFAPI_PORT || XFAPI_INTERNAL)
 * @ingroup group_xf_net_apps
 * @defgroup group_xf_iperf xf_iperf
 * @brief 吞吐量测试。
 * @endcond
 */

#ifndef __XF_IPERF_H__
#define __XF_IPERF_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_osal.h"
#include "xf_netif.h"

/**
 * @cond XFAPI_USER
 * @addtogroup group_xf_iperf
 * @endcond
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define IPERF_IP_TYPE_IPV4              0
#define IPERF_IP_TYPE_IPV6              1
#define IPERF_TRANS_TYPE_TCP            0
#define IPERF_TRANS_TYPE_UDP            1

#define IPERF_FLAG_SET(cfg, flag)       ((cfg) |= (flag))
#define IPERF_FLAG_CLR(cfg, flag)       ((cfg) &= (~(flag)))

#define IPERF_FLAG_CLIENT               (1)
#define IPERF_FLAG_SERVER               (1 << 1)
#define IPERF_FLAG_TCP                  (1 << 2)
#define IPERF_FLAG_UDP                  (1 << 3)

#define IPERF_DEFAULT_PORT              5001
#define IPERF_DEFAULT_INTERVAL          1
#define IPERF_DEFAULT_TIME              30
#define IPERF_DEFAULT_NO_BW_LIMIT       -1

#define IPERF_TRAFFIC_TASK_NAME         "iperf_traffic"
#define IPERF_TRAFFIC_TASK_PRIORITY     XF_OSAL_PRIORITY_NORMOL
#define IPERF_TRAFFIC_TASK_STACK        4096
#define IPERF_REPORT_TASK_NAME          "iperf_report"
#define IPERF_REPORT_TASK_PRIORITY      XF_OSAL_PRIORITY_NORMOL
#define IPERF_REPORT_TASK_STACK         4096

#define IPERF_DEFAULT_IPV4_UDP_TX_LEN   (1470)
#define IPERF_DEFAULT_IPV6_UDP_TX_LEN   (1450)
#define IPERF_DEFAULT_UDP_RX_LEN        (16 << 10)
#define IPERF_DEFAULT_TCP_TX_LEN        (16 << 10)
#define IPERF_DEFAULT_TCP_RX_LEN        (16 << 10)

#define IPERF_MAX_DELAY                 64

#define IPERF_SOCKET_RX_TIMEOUT_MS      3000

/* ==================== [Typedefs] ========================================== */

/**
 * @brief iperf 上下文的预声明。
 */
struct _xf_iperf_ctx_t;

/**
 * @brief iperf 句柄。
 */
typedef struct _xf_iperf_ctx_t *xf_iperf_t;

/**
 * @brief iperf 事件声明。
 */
typedef enum _xf_iperf_event_code_t {
    XF_IPERF_EVENT_START = 0x00,        /*!< iperf 开始。 */
    XF_IPERF_EVENT_REPORT,              /*!< iperf 报告带宽。
                                         *   事件数据见
                                         *   @ref xf_iperf_t.curr_time,
                                         *   @ref xf_iperf_t.actual_len,
                                         *   和 @ref xf_iperf_t.actual_bandwidth.
                                         */
    XF_IPERF_EVENT_END,                 /*!< iperf 完成。
                                         *   事件数据见 @ref xf_iperf_t.average_bandwidth.
                                         *   不要在回调内重启 iperf.
                                         */

    XF_IPERF_EVENT_MAX,                 /*!< iperf 事件 ID 最大值，无效 ID。 */
} xf_iperf_event_code_t;

/**
 * @brief iperf 事件 id。见 @ref xf_iperf_event_code_t.
 */
typedef int32_t xf_iperf_event_id_t;

/**
 * @brief iperf 回调函数原型。
 *
 * 不要在回调内重启 iperf.
 *
 * @param event_id 事件 id. 根据事件类型, 见 @ref xf_iperf_event_code_t.
 * @param hdl 事件数据. 见 @ref xf_iperf_t.
 * @param user_args 用户数据. `xf_iperf_new_session()` 时传入.
 */
typedef void (*xf_iperf_cb_t)(
    xf_iperf_event_id_t event_id, xf_iperf_t hdl, void *user_args);

/**
 * @brief iperf 配置类型。
 */
typedef struct _xf_iperf_cfg_t {
    uint32_t flag;                  /*!< 设定客户端或服务端/协议的标志，
                                     *   见 @ref IPERF_FLAG_CLIENT, @ref IPERF_FLAG_TCP.
                                     */
    xf_ip_addr_t dip;               /*!< 目标地址。 */
    xf_ip_addr_t sip;               /*!< 源地址。 TODO 不支持 IPv6 */
    uint8_t type;                   /*!< 源和目标的地址类型。 */
    uint16_t dport;                 /*!< 目标端口。 */
    uint16_t sport;                 /*!< 源端口。 */
    uint32_t interval;              /*!< 报告间隔，单位 s. */
    uint32_t time;                  /*!< iperf 运行总时间，单位 s. */
    uint16_t len_send_buf;          /*!< iperf 每包缓冲区的长度，填入 0 时使用
                                     *   @ref IPERF_DEFAULT_IPV4_UDP_TX_LEN
                                     *   或 @ref IPERF_DEFAULT_IPV6_UDP_TX_LEN.
                                     */
    int32_t bw_lim;                 /*!< 带宽限制，单位 Mbits/sec. */

    uint32_t traffic_task_stack_size;   /*!< 内部 traffic 任务的堆栈大小。 */
    uint32_t traffic_task_prio;         /*!< 内部 traffic 任务的优先级。 */

    bool report_enabled;                /*!< 是否使能内部 report 任务报告带宽。 */
    uint32_t report_task_stack_size;    /*!< 内部 report 任务的堆栈大小。 */
    uint32_t report_task_prio;          /*!< 内部 report 任务的优先级。 */
} xf_iperf_cfg_t;

/**
 * @brief 默认 iperf 配置
 */
#define XF_IPERF_DEFAULT_CONFIG() (xf_iperf_cfg_t) { \
        .flag = 0, \
        .dip = {}, \
        .sip = {}, \
        .type = IPERF_IP_TYPE_IPV4, \
        .dport = IPERF_DEFAULT_PORT, \
        .sport = IPERF_DEFAULT_PORT, \
        .interval = IPERF_DEFAULT_INTERVAL, \
        .time = IPERF_DEFAULT_TIME, \
        .len_send_buf = 0, \
        .bw_lim = IPERF_DEFAULT_NO_BW_LIMIT, \
        .traffic_task_stack_size = IPERF_TRAFFIC_TASK_STACK, \
        .traffic_task_prio = IPERF_TRAFFIC_TASK_PRIORITY, \
        .report_enabled = true, \
        .report_task_stack_size = IPERF_REPORT_TASK_STACK, \
        .report_task_prio = IPERF_REPORT_TASK_PRIORITY, \
    }

/**
 * @brief iperf 上下文.
 * @attention 只有 `public:` 部分是在回调中可读的，
 *            用户 **禁止** 修改其中任何内容。
 */
typedef struct _xf_iperf_ctx_t {
    /* public: */
    /* 只读数据 */
    xf_iperf_cfg_t cfg;                 /*!< xf_iperf_start() 传入的配置备份。 */
    uint32_t curr_time;                 /*!< 当前报告时刻，单位 s.
                                         *   注意，每次报告时间差见当前结构体内的 cfg:
                                         *   @ref xf_iperf_cfg_t.interval.
                                         */
    uint32_t actual_len;                /*!< 本次报告周期内，
                                         *   (xf_iperf_t.curr_time
                                         *      ~ xf_iperf_t.curr_time + xf_iperf_cfg_t.interval)
                                         *   实际发送或者接收到的数据长度(单位字节)。
                                         */
    float actual_bandwidth;             /*!< 本次报告周期内的实际带宽(单位 Mbits/sec)。 */
    float average_bandwidth;            /*!< 总的平均带宽(单位 Mbits/sec)。 */

    /* privte: */
    bool finish;                        /*!< 传输完毕标志或强制结束标志。 */
    uint32_t sockfd;                    /*!< socket 文件描述符。 */
    uint8_t *buffer;                    /*!< 内部测速缓冲区 */
    uint32_t buffer_len;                /*!< 每 tcp/udp 包长度。 */
    uint32_t actual_len_internal;       /*!< 本次报告周期内，
                                         *   实际发送或者接收到的数据长度(单位字节)。
                                         *   内部使用。
                                         */

    xf_iperf_cb_t cb_func;              /*!< 传入的回调函数 。*/
    void *user_args;                    /*!< 传入的回调函数的用户参数。 */

    xf_osal_thread_t traffic_task_hdl;  /*!< 内部 traffic 任务句柄。 */
    bool traffic_is_running;            /*!< 内部 traffic 任务是否正在运行。 */

    xf_osal_thread_t report_task_hdl;   /*!< 内部 report 任务句柄。 */
    bool report_is_running;             /*!< 内部 report 任务否正在运行。 */
} xf_iperf_ctx_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 启动 iperf.
 *
 * @note iperf 通常用于测试网络极限性能，因此不打算支持多实例。
 *
 * @param p_cfg iperf 配置。
 * @param cb_func iperf 回调函数。
 * @param user_args iperf 回调函数用户参数。
 * @return xf_err_t
 *      - XF_ERR_INVALID_ARG 无效参数（例如配置为空等）
 *      - XF_ERR_NO_MEM 内存不足
 *      - XF_FAIL 其他内部错误（例如套接字错误）
 *      - XF_OK 成功
 */
xf_err_t xf_iperf_start(
    const xf_iperf_cfg_t *p_cfg, xf_iperf_cb_t cb_func, void *user_args);

/**
 * @brief 获取 iperf 句柄。
 *
 * @return xf_iperf_t
 *      - NULL 失败
 *      - (OTHER) 成功
 */
xf_iperf_t xf_iperf_get_handle(void);

/**
 * @brief 检查 iperf 是否正在运行。
 *
 * @return
 *      - true 是
 *      - false 否
 */
bool xf_iperf_is_running(void);

/**
 * @brief 停止 iperf.
 *
 * @return
 *      - XF_OK 成功
 *      - XF_FAIL 其他内部错误（例如套接字错误）
 */
xf_err_t xf_iperf_stop(void);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_iperf
 * @}
 */

#endif /* __XF_IPERF_H__ */
