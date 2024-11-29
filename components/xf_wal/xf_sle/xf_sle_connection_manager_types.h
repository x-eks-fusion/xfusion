/**
 * @file xf_sle_connection_manager_types.h
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-08-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_SLE_XF_SLE_CONNECTION_MANAGER_TYPES_H__
#define __XF_SLE_XF_SLE_CONNECTION_MANAGER_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sle_types.h"

#if XF_SLE_IS_ENABLE || defined(__DOXYGEN__)

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_wal_sle
 * @endcond
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* TODO xf_sle_conn_param_update_t conn_id 移至函数参数处，
                supervision_timeout 改 timeout */
/**
 * @brief SLE 连接参数更新的信息
 */
typedef struct {
    uint16_t conn_id;             /*!< 链接 (连接) ID */
    uint16_t interval_min;        /*!< 链路调度最小间隔，单位slot */
    uint16_t interval_max;        /*!< 链路调度最大间隔，单位slot */
    uint16_t max_latency;         /*!< 延迟周期，单位slot */
    uint16_t supervision_timeout; /*!< 超时时间，单位10ms */
} xf_sle_conn_param_update_t;

/**
 * @brief SLE 配对状态
 */
typedef enum {
    XF_SLE_PAIR_STATE_NONE    = 0x01,    /*!< 未配对状态 */
    XF_SLE_PAIR_STATE_PAIRING = 0x02,    /*!< 正在配对 */
    XF_SLE_PAIR_STATE_PAIRED  = 0x03     /*!< 已完成配对 */
} xf_sle_pair_state_t;

/**
 * @brief SLE 断连原因
 */
typedef enum {
    XF_SLE_DISCONNECT_BY_REMOTE = 0x10,    /*!< 远端断链 */
    XF_SLE_DISCONNECT_BY_LOCAL  = 0x11,    /*!< 本端断链 */
} xf_sle_disconnect_reason_t;

/**
 * @brief SLE PHY 参数
 */
typedef struct {
    uint8_t tx_format;          /*!< 发送无线帧类型 */
    uint8_t rx_format;          /*!< 接收无线帧类型 */
    uint8_t tx_phy;             /*!< 发送 PHY */
    uint8_t rx_phy;             /*!< 接收 PHY */
    uint8_t tx_pilot_density;   /*!< 发送导频密度指示 */
    uint8_t rx_pilot_density;   /*!< 接收导频密度指示 */
    uint8_t g_feedback;         /*!< 先发链路反馈类型指示，取值范围 0-63 */
    uint8_t t_feedback;         /*!< 后发链路反馈类型指示，取值范围 0-7 */
} xf_sle_set_phy_t;

/**
 * @brief SLE 默认连接参数
 */
typedef struct {
    uint8_t  en_filter;         /*!< 链路是否打开过滤功能 */
    uint8_t  scan_phy;          /*!< 链路扫描通信带宽： 1:1M, 2:2M */
    uint8_t  en_gt_negotiate;   /*!< 链路建立时是否进行 G 和 T 交互 */
    uint16_t scan_interval;     /*!< 链路建立时扫描对端设备的 interval */
    uint16_t scan_window;       /*!< 链路建立时扫描对端设备的 windows */
    uint16_t interval_min;      /*!< 链路调度最小 interval */
    uint16_t interval_max;      /*!< 链路调度最大 interval */
    uint16_t timeout;           /*!< 链路超时时间 */
} xf_sle_conn_param_def_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_wal_sle
 * @}
 */

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_XF_SLE_CONNECTION_MANAGER_TYPES_H__ */
