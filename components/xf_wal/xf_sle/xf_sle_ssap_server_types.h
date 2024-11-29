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
 * 对属性集合中表示结束的最后一个属性成员结构直接置 0，如：
 *      ([attr_set_type])
 *      {
 *          {...},
 *          {...},
 *          {0}
 *      }
 */
#define XF_SLE_ATTR_SET_END_FLAG    NULL

/**
 * @brief SLE SSAPS 特征描述符信息
 */
typedef struct {
    xf_sle_uuid_info_t *desc_uuid;          /*!< 特征描述符 UUID，见 @ref xf_sle_uuid_info_t */
    xf_sle_ssap_permission_t permissions;   /*!< 特征描述符权限，见 @ref xf_sle_ssap_permission_t */
    xf_sle_ssap_operate_indication_t operate_indication;
                                            /*!< 操作指示，见 @ref xf_sle_ssap_operate_indication_t */
    xf_sle_ssap_desc_type_t desc_type;      /*!< 特征描述符类型，见 @ref xf_sle_ssap_desc_type_t */
    uint8_t *value;                         /*!< 属性值 */
    uint16_t value_len;                     /*!< 属性值长度 */
} xf_sle_ssaps_desc_t;

/**
 * @brief SLE SSAPS 特征值信息。
 */
typedef struct {
    xf_sle_attr_handle_t prop_handle;       /*!< 特征句柄 ，见 @ref xf_sle_attr_handle_t */
    xf_sle_uuid_info_t *prop_uuid;          /*!< 特征 UUID ，见 @ref xf_sle_uuid_info_t */
    xf_sle_ssap_permission_t permissions;   /*!< 特征权限，见 @ref xf_sle_ssap_permission_t */
    xf_sle_ssap_operate_indication_t
    operate_indication;                     /*!< 操作指示，见 @ref xf_sle_ssap_operate_indication_t */
    uint16_t value_len;                     /*!< 响应的数据长度 */
    uint8_t *value;                         /*!< 响应的数据 */
    xf_sle_ssaps_desc_t *desc_set;          /*!< 特征描述符集合，如无，则填 NULL，见 @ref xf_sle_ssaps_desc_t */
} xf_sle_ssaps_property_t;

/**
 * @brief SLE SSAPS 服务信息
 */
typedef struct _xf_sle_ssaps_service_t xf_sle_ssaps_service_t;
typedef struct _xf_sle_ssaps_service_t {
    xf_sle_attr_handle_t service_handle;    /*!< 服务句柄，见 @ref xf_sle_attr_handle_t */
    xf_sle_ssap_service_type_t service_type;/*!< 服务类型，见 @ref xf_sle_ssap_service_type_t */
    xf_sle_uuid_info_t *service_uuid;       /*!< 服务UUID ，见 @ref xf_sle_uuid_info_t */
    xf_sle_ssaps_service_t **include_set;   /*!< 包含服务 (include service) 集合信息，见 @ref xf_sle_ssaps_service_t */
    xf_sle_ssaps_property_t *prop_set;      /*!< 特征集合 ，见 @ref xf_sle_ssaps_property_t */
} xf_sle_ssaps_service_t;

/**
 * @brief SLE SSAPS 事件
 */
typedef enum {
    XF_SLE_ADV_EVT_ENABLE = _XF_SLE_SSAP_COMMON_EVT_MAX,/*!< 广播使能事件 */  
    XF_SLE_ADV_EVT_DISABLE,                             /*!< 广播失能事件 */
    XF_SLE_ADV_EVT_TERMINAL,                            
    XF_SLE_SSAPS_EVT_ADD_SERVICE,                       /*!< 服务添加事件 */       
    XF_SLE_SSAPS_EVT_ADD_PROPERTY,                      /*!< 特征添加事件 */
    XF_SLE_SSAPS_EVT_ADD_DESC,                          /*!< 特征描述符事件 */
    XF_SLE_SSAPS_EVT_SERVICE_START,                     /*!< 服务开启事件 */
    XF_SLE_SSAPS_EVT_SERVICE_DEL_ALL,                   /*!< 删除所有服务事件 */
    XF_SLE_SSAPS_EVT_REQ_READ,                          /*!< 接收到读请求事件 */
    XF_SLE_SSAPS_EVT_REQ_WRITE,                         /*!< 接收到写请求事件 */
} xf_sle_ssaps_event_t;


/**
 * @brief SLE SSAPS 服务端事件回调参数
 */
typedef union _xf_sle_ssaps_evt_cb_param_t {
    // XF_SLE_CONN_EVT_CONNECT,
    /**
     * @brief SLE SSAPS 连接事件的参数
     */
    struct {
        uint16_t conn_id;               /*!< 链接(连接) ID */
        xf_sle_addr_t peer_addr;        /*!< 对端地址，见 @ref xf_sle_addr_t */
        xf_sle_pair_state_t pair_state; /*!< 配对状态，见 @ref xf_sle_pair_state_t */
    } connect;

    // XF_SLE_CONN_EVT_DISCONNECT,
    /**
     * @brief SLE 断连事件的参数
     */
    struct {
        uint16_t conn_id;                   /*!< 链接(连接) ID */
        xf_sle_addr_t peer_addr;            /*!< 对端地址，见 @ref xf_sle_addr_t */
        xf_sle_pair_state_t pair_state;     /*!< 配对状态，见 @ref xf_sle_pair_state_t */
        xf_sle_disconnect_reason_t reason;  /*!< 断连原因，见 @ref xf_sle_disconnect_reason_t */
    } disconnect;

    // XF_SLE_CONN_EVT_CONN_PARAMS_UPDATE,
    /**
     * @brief SLE 断连事件的参数
     */
    struct {
        uint16_t interval;              /*!< 链路调度间隔，单位slot */
        uint16_t latency;               /*!< 延迟周期，单位slot */
        uint16_t supervision_timeout;   /*!< 超时时间，单位10ms */
    } conn_param_update;

    // XF_SLE_CONN_EVT_REQ_CONN_PARAMS_UPDATE
    /**
     * @brief SLE 连接参数更新事件的参数
     */
    struct {
        uint16_t conn_id;               /*!< 链接(连接) ID */
        uint16_t interval_min;          /*!< 链路调度间隔，单位slot */
        uint16_t interval_max;          /*!< 链路调度间隔，单位slot */
        uint16_t max_latency;           /*!< 延迟周期，单位slot */
        uint16_t supervision_timeout;   /*!< 超时时间，单位10ms */
    } req_conn_param_update;

    // XF_SLE_ADV_EVT_ENABLE,
    /**
     * @brief SLE 广播开启事件的参数
     */
    struct {
        uint32_t announce_id;       /*!< 广播 ID */
    } adv_enable;

    // XF_SLE_ADV_EVT_DISABLE,
    /**
     * @brief SLE 广播停止事件的参数
     */
    struct {
        uint32_t announce_id;       /*!< 广播 ID */
    } adv_disable;

    // XF_SLE_ADV_EVT_TERMINAL,
    /**
     * @brief SLE 广播 TERMINAL 事件的参数
     */
    struct {
        uint32_t announce_id;       /*!< 广播 ID */
    } adv_termial;

    /**
     * @brief SLE SSAPS 接收到读请求事件的参数
     */
    struct {
        uint16_t conn_id;                   /*!< 链接 (连接) ID */
        uint16_t trans_id;                  /*!< 传输 ID */
        uint16_t handle;                    /*!< 属性句柄 */
        xf_sle_ssap_property_type_t type;   /*!< 类型，见 @ref xf_sle_ssap_property_type_t */
        bool need_rsp;                      /*!< 是否需要发送响应 */
        bool need_auth;                     /*!< 是否需要授权 */
    } req_read;

    // XF_SLE_SSAPS_EVT_REQ_WRITE,
    /**
     * @brief SLE SSAPS 接收到写请求事件的参数
     */
    struct {
        uint16_t conn_id;                   /*!< 链接 (连接) ID */
        uint16_t trans_id;                  /*!< 传输 ID */
        uint16_t handle;                    /*!< 属性句柄 */
        xf_sle_ssap_property_type_t type;   /*!< 类型，见 @ref xf_sle_ssap_property_type_t */
        bool need_rsp;                      /*!< 是否需要发送响应 */
        bool need_auth;                     /*!< 是否需要授权 */
        uint8_t *value;                     /*!< 值 */
        uint16_t value_len;                 /*!< 值长度 */
    } req_write;    
} xf_sle_ssaps_evt_cb_param_t;

/**
 * @brief SLE SSAPS 响应 (回应) 值信息
 */
typedef struct {
    uint16_t value_len;         /*!< 响应的值长度 */
    uint8_t *value;             /*!< 响应的值 */
} xf_sle_ssaps_response_value_t;

/**
 * @brief SLE SSAPS 发送通知或指示的信息
 */
typedef struct {
    uint16_t handle;                    /*!< 属性句柄 */
    xf_sle_ssap_property_type_t type;   /*!< 属性类型，见 @ref xf_sle_ssap_property_type_t */
    uint16_t value_len;                 /*!< 通知/指示的值长度 */
    uint8_t *value;                     /*!< 发送的通知/指示的值 */
} xf_sle_ssaps_ntf_ind_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_SSAP_SERVER_TYPES_H__ */
