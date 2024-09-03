/**
 * @file xf_sle_ssap_client_types.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_SLE_SSAP_CLIENT_TYPES_H__
#define __XF_SLE_SSAP_CLIENT_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sle_types.h"
#include "xf_sle_ssap_types.h"

#if XF_SLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */


/**
 * @if Eng
 * @brief  SSAP find type.
 * @else
 * @brief  SSAP 查找类型
 * @endif
 */
typedef enum {
    XF_SLE_SSAP_FIND_TYPE_SERVICE_STRUCTURE = 0x00, /*!< @if Eng service structure
                                                  @else   服务结构 @endif */
    XF_SLE_SSAP_FIND_TYPE_PRIMARY_SERVICE   = 0x01, /*!< @if Eng primary service
                                                  @else   首要服务 @endif */
    XF_SLE_SSAP_FIND_TYPE_REFERENCE_SERVICE = 0x02, /*!< @if Eng reference service
                                                  @else   引用服务 @endif */
    XF_SLE_SSAP_FIND_TYPE_PROPERTY          = 0x03, /*!< @if Eng property
                                                  @else   属性 @endif */
    XF_SLE_SSAP_FIND_TYPE_METHOD            = 0x04, /*!< @if Eng method
                                                  @else   方法 @endif */
    XF_SLE_SSAP_FIND_TYPE_EVENT             = 0x05, /*!< @if Eng event
                                                  @else   事件 @endif */
} xf_sle_ssap_find_type_t;

/**
 * @if Eng
 * @brief SSAP find operation parameter.
 * @else
 * @brief SSAP 查找参数。
 * @endif
 */
typedef struct {
    xf_sle_ssap_find_type_t    type;      /*!< 查找类型 */
    uint16_t   start_hdl; /*!< @if Eng start handle.
                               @else   起始句柄 @endif */
    uint16_t   end_hdl;   /*!< @if Eng end handle.
                               @else   结束句柄 @endif */
    xf_sle_uuid_info_t uuid;      /*!< uuid，按照uuid查找时生效，其余不生效 */
    uint8_t    reserve;   /*!< @if Eng reserve, "0" for default value.
                               @else   预留，默认值写0 @endif */
} xf_sle_ssapc_find_struct_param_t;

/**
 * @brief 搜寻到的特征描述符
 */
typedef struct {
    uint16_t handle;            // 特征描述符句柄
    xf_sle_uuid_info_t uuid;             // 特征描述符uuid
} xf_sle_ssapc_desc_found_t;

/**
 * @brief 搜寻到的特征描述符集合
 */
typedef struct {
    xf_sle_ssapc_desc_found_t *set;        // 描述符集合
    uint16_t cnt;
} xf_sle_ssapc_desc_found_set_t;

/**
 * @brief 搜寻到的特征
 */
typedef struct {
    xf_sle_attr_handle_t handle;                 // 特征句柄
    xf_sle_uuid_info_t uuid;                     // 特征uuid

    xf_sle_ssap_operate_indication_t operate_indication;

    xf_sle_ssapc_desc_found_set_t *desc_set_info;
} xf_ble_ssapc_property_found_t;

/**
 * @brief 搜寻到的特征集合
 */
typedef struct {
    xf_ble_ssapc_property_found_t *set;      // 特征集合
    uint16_t cnt;
} xf_ble_ssapc_property_found_set_t;

/**
 * @brief 搜寻到的服务
 */
typedef struct {
    xf_sle_attr_handle_t start_hdl;  // 服务起始句柄
    xf_sle_attr_handle_t end_hdl;    // 服务结束句柄
    xf_sle_uuid_info_t uuid;        // 服务uuid
    xf_ble_ssapc_property_found_set_t *chara_set_info;
} xf_ble_ssapc_service_found_t;

/**
 * @brief 搜寻到的服务集合
 */
typedef struct {
    xf_ble_ssapc_service_found_t *set;
    uint16_t cnt;
} xf_ble_ssapc_service_found_set_t;


typedef enum {
    XF_SLE_SEEK_EVT_START = _XF_SLE_SSAP_COMMON_EVT_MAX,
    XF_SLE_SEEK_EVT_STOP,
    XF_SLE_SEEK_EVT_RESULT,


    XF_SLE_SSAPS_EVT_FIND_STRUCT,
    XF_SLE_SSAPS_EVT_FIND_STRUCT_CMPL,
    XF_SLE_SSAPS_EVT_FIND_PROPERTY,
    XF_SLE_SSAPS_EVT_RECV_WRITE_CFM,
    XF_SLE_SSAPS_EVT_RECV_READ_CFM,
    XF_SLE_SSAPS_EVT_NOTIFICATION,
    XF_SLE_SSAPS_EVT_INDICATION,
} xf_sle_ssapc_event_t;


typedef struct {
    uint16_t conn_id;
    xf_sle_addr_t peer_addr;
    xf_sle_pair_state_t pair_state;
} xf_sle_evt_param_connect_t;

typedef struct {
    uint16_t conn_id;
    xf_sle_addr_t peer_addr;
    xf_sle_pair_state_t pair_state;
    xf_sle_disconnect_reason_t reason;
} xf_sle_evt_param_disconnect_t;

typedef struct {
    uint16_t conn_id;
    uint16_t interval;              /*!< 链路调度间隔，单位slot */
    uint16_t latency;               /*!< 延迟周期，单位slot */
    uint16_t supervision_timeout;           /*!< 超时时间，单位10ms */
} xf_sle_evt_param_conn_param_update;

/* TODO supervision_timeout 改 timeout */
typedef struct {
    uint16_t conn_id;
    uint16_t interval_min;          /*!< 链路调度间隔，单位slot */
    uint16_t interval_max;          /*!< 链路调度间隔，单位slot */
    uint16_t max_latency;           /*!< 延迟周期，单位slot */
    uint16_t supervision_timeout;   /*!< 超时时间，单位10ms */
} xf_sle_evt_param_req_conn_param_update;

typedef struct {
    uint8_t evt_type;             /*!< 上报事件类型 */
    xf_sle_addr_t peer_addr;
    xf_sle_addr_t direct_addr;     /*!< 定向发现地址 */
    /* FIXME 这里不知道为啥官方用的是 uint8_t */
    uint8_t rssi;                   /*!< 信号强度指示，取值范围[-127dBm, 20dBm]，0x7F表示不提供信号强度指示 */
    uint8_t data_status;            // 数据状态
    uint8_t data_len;
    uint8_t *data;
} xf_sle_evt_param_seek_result;

typedef struct {
    uint16_t conn_id;
    uint16_t handle;    /*!< @if Eng property handle.
                             @else   属性句柄。 @endif */
    uint8_t  type;      /*!< @if Eng property type.
                             @else   属性类型。 @endif */
    uint16_t data_len;  /*!< @if Eng Data Length.
                             @else   数据长度。 @endif */
    uint8_t  *data;     /*!< @if Eng Data.
                             @else   数据内容。 @endif */
} xf_sle_evt_param_req_read_cfm;

typedef struct {
    uint16_t conn_id;
    uint16_t handle;    /*!< @if Eng property handle.
                             @else   属性句柄。 @endif */
    uint8_t  type;      /*!< @if Eng property type.
                             @else   属性类型。 @endif */
} xf_sle_evt_param_req_write_cfm;

typedef struct {
    uint16_t conn_id;
    uint16_t handle;    /*!< @if Eng property handle.
                             @else   属性句柄。 @endif */
    uint8_t  type;      /*!< @if Eng property type.
                             @else   属性类型。 @endif */
    uint16_t data_len;  /*!< @if Eng Data Length.
                             @else   数据长度。 @endif */
    uint8_t  *data;     /*!< @if Eng Data. */
} xf_sle_evt_param_ntf_t, xf_sle_evt_param_ind_t;

typedef union _xf_sle_ssapc_evt_cb_param_t {
    // XF_SLE_CONN_EVT_CONNECT,
    xf_sle_evt_param_connect_t connect;

    // XF_SLE_CONN_EVT_DISCONNECT,
    xf_sle_evt_param_disconnect_t disconnect;

    // XF_SLE_CONN_EVT_CONN_PARAMS_UPDATE,
    xf_sle_evt_param_conn_param_update conn_param_update;

    // XF_SLE_CONN_EVT_REQ_CONN_PARAMS_UPDATE
    xf_sle_evt_param_req_conn_param_update req_conn_param_update;

    // // XF_SLE_SEEK_EVT_START,
    // struct
    // {
    // }seek_enable;

    // // XF_SLE_SEEK_EVT_STOP,
    // struct
    // {
    // }seek_disable;

    // XF_SLE_SEEK_EVT_RESULT,
    xf_sle_evt_param_seek_result seek_result;

    /* TODO 暂未支持 */
    // XF_SLE_CONN_EVT_AUTH_CMPL,
    // XF_SLE_CONN_EVT_PAIR_CMPL,

    // XF_SLE_SSAPS_EVT_ADD_SERVICE,
    // XF_SLE_SSAPS_EVT_ADD_PROPERTY,
    // XF_SLE_SSAPS_EVT_ADD_DESC,
    // XF_SLE_SSAPS_EVT_SERVICE_START,
    // // XF_SLE_SSAPS_EVT_SERVICE_STOP,
    // XF_SLE_SSAPS_EVT_SERVICE_DEL_ALL,

    // XF_SLE_SSAPS_EVT_REQ_READ,
    xf_sle_evt_param_req_read_cfm req_read;
    // XF_SLE_SSAPS_EVT_REQ_WRITE,
    xf_sle_evt_param_req_write_cfm req_write;
    // XF_SLE_SSAPS_EVT_MTU_CHANGED

    xf_sle_evt_param_ntf_t ntf;
    xf_sle_evt_param_ind_t ind;
} xf_sle_ssapc_evt_cb_param_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_SSAP_CLIENT_TYPES_H__ */
