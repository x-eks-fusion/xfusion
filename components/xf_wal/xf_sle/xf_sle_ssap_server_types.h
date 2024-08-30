/**
 * @file xf_sle_ssap_server_types.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_SLE_SSAP_SERVER_TYPES_H__
#define __XF_SLE_SSAP_SERVER_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sle_types.h"
#include "xf_sle_ssap_types.h"
#include "xf_sle_connection_manager_types.h"

#if XF_SLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief 属性集合的结束标记值
 *
 * @note 一般出现在构造服务结构时，
 * 多个服务、特征、描述符等属性的集合的结尾部分，
 * 一般是标记属性的 UUID 项（关键项）为
 * 'XF_SLE_ATTR_SET_END_FLAG' 表示结束。
 *
 * @note 由于结束标记的特殊性（NULL），用户构造时也可以
 * 对属性集合中表示结束的最后一个属性成员结构直接置 0，
 * 如： ([attr_set_type])
 *      {
 *          {...},
 *          {...},
 *          {0}
 *      }
 */
#define XF_SLE_ATTR_SET_END_FLAG    NULL
/**
 * @brief 特征描述符信息。
 *  特征描述符（不知道是不是属性）定义
 *  |标识：特征 UUID（特征声明中的）.                               【O】
 *  |权限：由高层 profile 指定（如服务）                            【O】
 */
typedef struct {
    xf_sle_uuid_info_t *desc_uuid; /*!< @if Eng UUID of GATT descriptor.
                                @else   GATT 描述符 UUID。 @endif */

    xf_sle_ssap_permission_t permissions;


    xf_sle_ssap_operate_indication_t operate_indication;

    xf_sle_ssap_desc_type_t desc_type;   // 描述符类型

    uint8_t *value;        // chara value Attr - 属性值
    uint16_t value_len;    // chara value Attr - 属性值长度
} xf_sle_ssaps_desc_t;

/**
 * @if Eng
 * @brief Struct of add property information.
 * @else
 * @brief 添加特征信息。
 * @endif
 */
typedef struct {
    xf_sle_attr_handle_t prop_handle;
    xf_sle_uuid_info_t *prop_uuid;    /*!< SSAP 特征 UUID */
    xf_sle_ssap_permission_t permissions;   /*!< 特征权限 */
    xf_sle_ssap_operate_indication_t
    operate_indication;         /*!< 操作指示 */
    uint16_t value_len;             /*!< 响应的数据长度 */
    uint8_t *value;                 /*!< 响应的数据 */

    xf_sle_ssaps_desc_t *desc_set;
} xf_sle_ssaps_property_t;

typedef struct _xf_sle_ssaps_service_t xf_sle_ssaps_service_t;
typedef struct _xf_sle_ssaps_service_t {
    xf_sle_attr_handle_t service_handle;         /*<<<<<<<<<<<<<<<<<<<<<<<<*/

    // 主要服务、次要服务  服务集结束标志
    xf_sle_ssap_service_type_t service_type; // attr type

    /**
     * 指向服务 UUID 的指针；使用BLE_UUIDxx_DECLARE宏声明
     */
    xf_sle_uuid_info_t *service_uuid; // attr value


    /* 以下为官方标准上的可选项，如需则填入 */

    // 包含服务 集合（数组）
    xf_sle_ssaps_service_t **include_set;

    // 特征 集合（数组）
    xf_sle_ssaps_property_t *prop_set;
} xf_sle_ssaps_service_t;

typedef enum {
    XF_SLE_ADV_EVT_ENABLE,
    XF_SLE_ADV_EVT_DISABLE,
    XF_SLE_ADV_EVT_TERMINAL,


    XF_SLE_SSAPS_EVT_ADD_SERVICE,
    XF_SLE_SSAPS_EVT_ADD_PROPERTY,
    XF_SLE_SSAPS_EVT_ADD_DESC,
    XF_SLE_SSAPS_EVT_SERVICE_START,
    // XF_SLE_SSAPS_EVT_SERVICE_STOP,
    XF_SLE_SSAPS_EVT_SERVICE_DEL_ALL,
    XF_SLE_SSAPS_EVT_REQ_READ,
    XF_SLE_SSAPS_EVT_REQ_WRITE,
    XF_SLE_SSAPS_EVT_MTU_CHANGED
} xf_sle_ssaps_event_t;

typedef union _xf_sle_ssaps_evt_cb_param_t {
    // XF_SLE_CONN_EVT_CONNECT,
    struct {
        uint16_t conn_id;
        xf_sle_addr_t peer_addr;
        xf_sle_pair_state_t pair_state;
    } connect;

    // XF_SLE_CONN_EVT_DISCONNECT,
    struct {
        uint16_t conn_id;
        xf_sle_addr_t peer_addr;
        xf_sle_pair_state_t pair_state;
        xf_sle_disconnect_reason_t reason;
    } disconnect;

    // XF_SLE_CONN_EVT_CONN_PARAMS_UPDATE,

    struct {
        uint16_t interval;              /*!< 链路调度间隔，单位slot */
        uint16_t latency;               /*!< 延迟周期，单位slot */
        uint16_t supervision_timeout;           /*!< 超时时间，单位10ms */
    } conn_param_update;

    // XF_SLE_CONN_EVT_REQ_CONN_PARAMS_UPDATE
    struct {
        uint16_t conn_id;
        uint16_t interval_min;          /*!< 链路调度间隔，单位slot */
        uint16_t interval_max;          /*!< 链路调度间隔，单位slot */
        uint16_t max_latency;           /*!< 延迟周期，单位slot */
        uint16_t supervision_timeout;   /*!< 超时时间，单位10ms */
    } req_conn_param_update;

    // XF_SLE_ADV_EVT_ENABLE,
    struct {
        uint32_t announce_id;
    } adv_enable;

    // XF_SLE_ADV_EVT_DISABLE,
    struct {
        uint32_t announce_id;
    } adv_disable;

    // XF_SLE_ADV_EVT_TERMINAL,
    struct {
        uint32_t announce_id;
    } adv_termial;

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
    struct {
        uint16_t conn_id;
        uint16_t trans_id;
        uint16_t handle;
        xf_sle_ssap_property_type_t type;
        bool need_rsp;  // 是否需要发送响应
        bool need_auth; // 是否需要授权
    } req_read;

    // XF_SLE_SSAPS_EVT_REQ_WRITE,
    struct {
        uint16_t conn_id;
        uint16_t trans_id;
        uint16_t handle;
        xf_sle_ssap_property_type_t type;
        bool need_rsp;  // 是否需要发送响应
        bool need_auth; // 是否需要授权
        uint8_t *value;
        uint16_t value_len;
    } req_write;

    // XF_SLE_SSAPS_EVT_MTU_CHANGED

} xf_sle_ssaps_evt_cb_param_t;

typedef struct {
    uint16_t value_len;  /*!< @if Eng Length of reponse data.
                              @else   响应的数据长度。 @endif */
    uint8_t *value;      /*!< @if Eng Reponse data.
                              @else   响应的数据。 @endif */
} xf_sle_ssaps_response_value_t;

/**
 * @if Eng
 * @brief Struct of send notification/indication information.
 * @else
 * @brief 发送通知/指示信息。
 * @endif
 */
typedef struct {
    uint16_t handle;      /*!< @if Eng Properity handle.
                               @else   属性句柄。 @endif */
    xf_sle_ssap_property_type_t type; /*!< 属性类型 */
    uint16_t value_len;   /*!< @if Eng Length of notification/indication data.
                               @else   通知/指示数据长度。 @endif */
    uint8_t *value;       /*!< @if Eng Notification/indication data.
                               @else   发送的通知/指示数据。 @endif */
} xf_sle_ssaps_ntf_ind_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_SSAP_SERVER_TYPES_H__ */
