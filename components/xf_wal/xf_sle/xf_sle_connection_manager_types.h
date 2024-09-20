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

#if XF_SLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* TODO xf_sle_conn_param_update_t conn_id 移至函数参数处，
			 	supervision_timeout 改 timeout */
/**
 * @if Eng
 * @brief Enum of sle logical link update parameters.
 * @else
 * @brief 星闪逻辑链路更新参数
 * @endif
 */
typedef struct {
    uint16_t conn_id;             /*!< @if Eng connection ID
                                       @else   连接ID @endif */
    uint16_t interval_min;        /*!< @if Eng minimum interval
                                       @else   链路调度最小间隔，单位slot @endif */
    uint16_t interval_max;        /*!< @if Eng maximum interval
                                       @else   链路调度最大间隔，单位slot @endif */
    uint16_t max_latency;         /*!< @if Eng maximum latency
                                       @else   延迟周期，单位slot @endif */
    uint16_t supervision_timeout; /*!< @if Eng timeout
                                       @else   超时时间，单位10ms @endif */
} xf_sle_conn_param_update_t;

/**
 * @if Eng
 * @brief Enum of sle pairing state.
 * @else
 * @brief 星闪配对状态。
 * @endif
 */
typedef enum {
    XF_SLE_PAIR_STATE_NONE    = 0x01,    /*!< @if Eng Pair state of none
                                     @else   未配对状态 @endif */
    XF_SLE_PAIR_STATE_PAIRING = 0x02,    /*!< @if Eng Pair state of pairing
                                     @else   正在配对 @endif */
    XF_SLE_PAIR_STATE_PAIRED  = 0x03     /*!< @if Eng Pair state of paired
                                     @else   已完成配对 @endif */
} xf_sle_pair_state_t;

/**
 * @if Eng
 * @brief Enum of sle pairing state.
 * @else
 * @brief 星闪断链原因。
 * @endif
 */
typedef enum {
    XF_SLE_DISCONNECT_BY_REMOTE = 0x10,    /*!< @if Eng disconnect by remote
                                             @else   远端断链 @endif */
    XF_SLE_DISCONNECT_BY_LOCAL  = 0x11,    /*!< @if Eng disconnect by local
                                             @else   本端断链 @endif */
} xf_sle_disconnect_reason_t;

/**
 * @if Eng
 * @brief Struct of sle phy parameter.
 * @else
 * @brief 星闪phy参数
 * @endif
 */
typedef struct {
    uint8_t tx_format;          /*!< @if Eng Transmitted radio frame type, @ref sle_radio_frame_t
                                     @else 发送无线帧类型，参考 { @ref sle_radio_frame_t }。 @endif */
    uint8_t rx_format;          /*!< @if Eng Received radio frame type, @ref sle_radio_frame_t
                                     @else 接收无线帧类型，参考 { @ref sle_radio_frame_t }。 @endif */
    uint8_t tx_phy;             /*!< @if Eng Transmitted PHY, @ref sle_phy_tx_rx_t
                                     @else 发送PHY，参考 { @ref sle_phy_tx_rx_t }。 @endif */
    uint8_t rx_phy;             /*!< @if Eng Received PHY, @ref sle_phy_tx_rx_t
                                     @else 接收PHY，参考 { @ref sle_phy_tx_rx_t }。 @endif */
    uint8_t tx_pilot_density;   /*!< @if Eng Transmitted pilot density indicator, @ref sle_phy_tx_rx_pilot_density_t
                                     @else 发送导频密度指示，参考 { @ref sle_phy_tx_rx_pilot_density_t }。 @endif */
    uint8_t rx_pilot_density;   /*!< @if Eng Received pilot density indicator, @ref sle_phy_tx_rx_pilot_density_t
                                     @else 接收导频密度指示，参考 { @ref sle_phy_tx_rx_pilot_density_t }。 @endif */
    uint8_t g_feedback;         /*!< @if Eng Indicates the feedback type of the pre-transmitted link.
                                             The value range is 0 to 63.
                                     @else 先发链路反馈类型指示，取值范围0-63。 @endif */
    uint8_t t_feedback;         /*!< @if Eng Indicates the feedback type of the post-transmit link.
                                             The value range is 0-7.
                                     @else 后发链路反馈类型指示，取值范围0-7。 @endif */
} xf_sle_set_phy_t;

/**
 * @if Eng
 * @brief Struct of Default sle connection parameter.
 * @else
 * @brief 星闪默认连接参数
 * @endif
 */
typedef struct {
    uint8_t  en_filter;      /*!< @if Eng Whether the filtering function is enabled on the link
                                             @else 链路是否打开过滤功能 @endif */
    uint8_t  scan_phy;             /*!< @if Eng Link scanning communication bandwidth: 1:1M, 2:2M
                                             @else 链路扫描通信带宽： 1:1M, 2:2M @endif */
    uint8_t  en_gt_negotiate;              /*!< @if Eng Whether G-T interaction is performed during link establishment
                                             @else 链路建立时是否进行G和T交互 @endif */
    uint16_t scan_interval;             /*!< @if Eng Interval for scanning the peer
                                                     device during link establishment
                                             @else 链路建立时扫描对端设备的interval @endif */
    uint16_t scan_window;               /*!< @if Eng Scans the Windows operating system of the
                                                     peer device during link establishment.
                                             @else 链路建立时扫描对端设备的windows @endif */
    uint16_t interval_min;              /*!< @if Eng Minimum link scheduling interval
                                             @else 链路调度最小interval @endif */
    uint16_t interval_max;              /*!< @if Eng Maximum link scheduling interval
                                             @else 链路调度最大interval @endif */
    uint16_t timeout;                   /*!< @if Eng Link Timeout Interval
                                             @else 链路超时时间 @endif */
} xf_sle_conn_param_def_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_XF_SLE_CONNECTION_MANAGER_TYPES_H__ */
