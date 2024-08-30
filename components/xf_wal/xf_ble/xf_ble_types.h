/**
 * @file xf_ble_types.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_BLE_TYPES_H__
#define __XF_BLE_TYPES_H__

/* ==================== [Includes] ========================================== */

typedef enum {
    XF_BLE_GAP_EVT_CONNECT,
    XF_BLE_GAP_EVT_DISCONNECT,
    XF_BLE_GAP_EVT_PAIR_END,
    XF_BLE_GAP_EVT_CONN_PARAMS_UPDATE,

    XF_BLE_GAP_EVT_SCAN_PARAM_SET,
    XF_BLE_GAP_EVT_SCAN_RESULT,

    XF_BLE_GAP_ADV_STARTED,
    XF_BLE_GAP_ADV_STOPPED,
    
    XF_BLE_GAP_EVT_ENUM_END,
} xf_ble_gap_event_t;

#include "xf_utils.h"
/* FIXME gap 与 gatt 事件以及事件回调分离 */
typedef enum {

    // XF_BLE_GAP_EVT_CONNECT,
    // XF_BLE_GAP_EVT_DISCONNECT,
    // XF_BLE_GAP_ADV_STARTED,
    // XF_BLE_GAP_ADV_STOPPED,
    // XF_BLE_GAP_EVT_CONN_PARAMS_UPDATE,

    XF_BLE_GATTS_EVT_ADD_SERVICE = XF_BLE_GAP_EVT_ENUM_END,
    XF_BLE_GATTS_EVT_DEL_SERVICE,
    XF_BLE_GATTS_EVT_ADD_CHARA,
    XF_BLE_GATTS_EVT_ADD_DESC,
    XF_BLE_GATTS_EVT_SERVICE_START,
    XF_BLE_GATTS_EVT_SERVICE_STOP,
    XF_BLE_GATTS_EVT_REQ_READ,
    XF_BLE_GATTS_EVT_REQ_WRITE,
    XF_BLE_GATTS_EVT_MTU_CHANGED,
} xf_ble_gatts_event_t;


typedef enum {

    // XF_BLE_GAP_EVT_SCAN_PARAM_SET,
    // XF_BLE_GAP_EVT_SCAN_RESULT,

    // XF_BLE_GAP_EVT_CONNECT,
    // XF_BLE_GAP_EVT_DISCONNECT,
    // XF_BLE_GAP_EVT_CONN_PARAMS_UPDATE,

    XF_BLE_GATTC_EVT_DISCOVER_SERVICE_COMPLETE = XF_BLE_GAP_EVT_ENUM_END,
    XF_BLE_GATTC_EVT_MTU_CHANGED,
    XF_BLE_GATTC_EVT_WRITE_COMPLETE,
    XF_BLE_GATTC_EVT_READ_COMPLETE,
    XF_BLE_GATTC_EVT_RECV_NOTIFICATION_OR_INDICATION,
} xf_ble_gattc_event_t;

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_BLE_IS_ENABLE 1

/* ==================== [Typedefs] ========================================== */


/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_BLE_TYPES_H__ */
