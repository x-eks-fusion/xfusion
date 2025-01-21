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

#if XF_SLE_IS_ENABLE || defined(__DOXYGEN__)

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_wal_sle_ssap
 * @endcond
 * @{
 */

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
    xf_sle_ssaps_desc_t
    *desc_set;          /*!< 特征描述符集合，如无，则填 NULL，见 @ref xf_sle_ssaps_desc_t */
} xf_sle_ssaps_property_t;

/**
 * @brief SLE SSAPS 服务信息
 */
typedef struct _xf_sle_ssaps_service_t xf_sle_ssaps_service_t;
typedef struct _xf_sle_ssaps_service_t {
    xf_sle_attr_handle_t service_handle;    /*!< 服务句柄，见 @ref xf_sle_attr_handle_t */
    xf_sle_ssap_service_type_t service_type;/*!< 服务类型，见 @ref xf_sle_ssap_service_type_t */
    xf_sle_uuid_info_t *service_uuid;       /*!< 服务UUID ，见 @ref xf_sle_uuid_info_t */
    xf_sle_ssaps_service_t
    **include_set;   /*!< 包含服务 (include service) 集合信息，见 @ref xf_sle_ssaps_service_t */
    xf_sle_ssaps_property_t *prop_set;      /*!< 特征集合 ，见 @ref xf_sle_ssaps_property_t */
} xf_sle_ssaps_service_t;

/**
 * @brief SLE SSAPS 事件
 */
typedef enum {
    XF_SLE_SSAPS_EVT_EXCHANGE_INFO = _XF_SLE_COMMON_EVT_ENUM_END,   /*!< INFO (MTU) 协商事件 */
    XF_SLE_SSAPS_EVT_READ_REQ,                                      /*!< 接收到读请求事件 */
    XF_SLE_SSAPS_EVT_WRITE_REQ,                                     /*!< 接收到写请求事件 */
} xf_sle_ssaps_event_t;

/**
 * @brief SLE SSAPS INFO 协商事件的参数
 */
typedef struct {
    uint8_t app_id;         /*!< 服务端 (应用) ID */
    uint16_t conn_id;       /*!< 链接(连接) ID */
    uint32_t mtu_size;      /*!< MTU 大小 */
    uint16_t version;       /*!< 版本 */
} xf_sle_ssaps_evt_param_exchange_info_t;

/**
 * @brief SLE SSAPS 读请求事件的参数
 */
typedef struct {
    uint16_t conn_id;                   /*!< 链接 (连接) ID */
    uint16_t trans_id;                  /*!< 传输 ID */
    uint16_t handle;                    /*!< 属性句柄 */
    xf_sle_ssap_property_type_t type;   /*!< 类型，见 @ref xf_sle_ssap_property_type_t */
    bool need_rsp;                      /*!< 是否需要发送响应 */
    bool need_auth;                     /*!< 是否需要授权 */
} xf_sle_ssaps_evt_param_read_req_t;

/**
 * @brief SLE SSAPS 写请求事件的参数
 */
typedef struct {
    uint16_t conn_id;                   /*!< 链接 (连接) ID */
    uint16_t trans_id;                  /*!< 传输 ID */
    uint16_t handle;                    /*!< 属性句柄 */
    xf_sle_ssap_property_type_t type;   /*!< 类型，见 @ref xf_sle_ssap_property_type_t */
    bool need_rsp;                      /*!< 是否需要发送响应 */
    bool need_auth;                     /*!< 是否需要授权 */
    uint8_t *value;                     /*!< 值 */
    uint16_t value_len;                 /*!< 值长度 */
} xf_sle_ssaps_evt_param_write_req_t;

/**
 * @brief SLE SSAPS 服务端事件回调参数
 */
typedef union _xf_sle_ssaps_evt_cb_param_t {
   xf_sle_common_evt_param_connect_t connect;   /*!< 连接事件的参数，
                                                 *  @ref xf_sle_common_evt_param_connect_t
                                                 *  XF_SLE_COMMON_EVT_CONNECT
                                                 */
    xf_sle_common_evt_param_disconnect_t disconnect;
                                                /*!< 断连事件的参数，
                                                 *  @ref xf_sle_common_evt_param_disconnect_t
                                                 *  XF_SLE_COMMON_EVT_DISCONNECT
                                                 */
    xf_sle_common_evt_param_conn_param_update_t conn_param_update;
                                                /*!< 连接参数更新事件的参数，
                                                 *  @ref xf_sle_common_evt_param_conn_param_update_t
                                                 *  XF_SLE_COMMON_EVT_CONN_PARAMS_UPDATE
                                                 */
    xf_sle_ssaps_evt_param_exchange_info_t info;/*!< 接收到 INFO 协商事件的参数，
                                                 *  @ref xf_sle_ssaps_evt_param_exchange_info_t
                                                 *  XF_SLE_SSAPS_EVT_EXCHANGE_INFO
                                                 */
    xf_sle_ssaps_evt_param_read_req_t read_req;                    
                                    /*!< 接收到读请求事件的参数，
                                     *  @ref xf_sle_ssaps_evt_param_read_req_t
                                     *  XF_SLE_SSAPS_EVT_READ_REQ
                                     */
    xf_sle_ssaps_evt_param_write_req_t write_req;                    
                                    /*!< 接收到写请求事件的参数，
                                     *  @ref xf_sle_ssaps_evt_param_write_req_t
                                     *  XF_SLE_SSAPS_EVT_WRITE_REQ
                                     */
} xf_sle_ssaps_evt_cb_param_t;

/**
 * @brief SLE SSAPS 事件回调函数原型
 *
 * @param event 事件，见 @ref xf_sle_ssaps_event_t
 * @param param 事件回调参数，见 @ref xf_sle_ssaps_evt_cb_param_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
typedef xf_err_t (*xf_sle_ssaps_event_cb_t)(
    xf_sle_ssaps_event_t event,
    xf_sle_ssaps_evt_cb_param_t *param);

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

/**
 * End of addtogroup group_xf_wal_sle_ssap
 * @}
 */

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_SSAP_SERVER_TYPES_H__ */
