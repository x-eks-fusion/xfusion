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
 * @brief SLE SSAP 搜寻类型
*/
typedef enum {
    XF_SLE_SSAP_FIND_TYPE_SERVICE_STRUCTURE = 0x00, /*!< 服务结构 */
    XF_SLE_SSAP_FIND_TYPE_PRIMARY_SERVICE   = 0x01, /*!< 首要服务 */
    XF_SLE_SSAP_FIND_TYPE_REFERENCE_SERVICE = 0x02, /*!< 引用服务 */
    XF_SLE_SSAP_FIND_TYPE_PROPERTY          = 0x03, /*!< 属性 (property) */
    XF_SLE_SSAP_FIND_TYPE_METHOD            = 0x04, /*!< 方法 (method) */
    XF_SLE_SSAP_FIND_TYPE_EVENT             = 0x05, /*!< 事件 (event) */
} xf_sle_ssap_find_type_t;

/**
 * @brief 
 * 
 */
typedef struct {
    xf_sle_ssap_find_type_t type;   /*!< 搜寻类型，见 @ref xf_sle_ssap_find_type_t */
    uint16_t start_hdl;             /*!< 起始句柄 */
    uint16_t end_hdl;               /*!< 结束句柄 */
    xf_sle_uuid_info_t
    uuid;        /*!< UUID，按照 UUID 搜寻时生效，其余情况不生效，见 @ref xf_sle_uuid_info_t */
    uint8_t    reserve;             /*!< 预留，默认值写0 */
} xf_sle_ssapc_find_struct_param_t;

/**
 * @brief SLE SSAPC 搜寻到的特征描述符
 */
typedef struct {
    uint16_t handle;                /*!< 特征描述符句柄 */
    xf_sle_uuid_info_t uuid;        /*!< 特征描述符 UUID ，见 @ref xf_sle_uuid_info_t */
} xf_sle_ssapc_desc_found_t;

/**
 * @brief SLE SSAPC 搜寻到的特征描述符集合信息
 */
typedef struct {
    xf_sle_ssapc_desc_found_t *set; /*!< 描述符集合 ，见 @ref xf_sle_ssapc_desc_found_t */
    uint16_t cnt;                   /*!< 搜寻到的个数 */
} xf_sle_ssapc_desc_found_set_t;

/**
 * @brief SLE SSAPC 搜寻到的特征
 */
typedef struct {
    xf_sle_attr_handle_t handle;    /*!< 特征句柄 */
    xf_sle_uuid_info_t uuid;        /*!< 特征 UUID ，见 @ref xf_sle_uuid_info_t */
    xf_sle_ssap_operate_indication_t operate_indication;
    /*!< 操作指示 ，见 @ref xf_sle_ssap_operate_indication_t */
    xf_sle_ssapc_desc_found_set_t *desc_set_info;
    /*!< 特征描述符集合信息，见 @ref xf_sle_ssapc_desc_found_set_t */
} xf_sle_ssapc_property_found_t;

/**
 * @brief SLE SSAPC 搜寻到的特征集合信息
 */
typedef struct {
    xf_sle_ssapc_property_found_t *set; /*!< 特征集合，见 @ref xf_sle_ssapc_property_found_t */
    uint16_t cnt;                       /*!< 搜寻到的个数 */
} xf_sle_ssapc_property_found_set_t;

/**
 * @brief SLE SSAPC 搜寻到的服务
 */
typedef struct {
    xf_sle_attr_handle_t start_hdl;     /*!< 服务起始句柄，见 @ref xf_sle_attr_handle_t */
    xf_sle_attr_handle_t end_hdl;       /*!< 服务结束句柄，见 @ref xf_sle_attr_handle_t */
    xf_sle_uuid_info_t uuid;            /*!< 服务 UUID ，见 @ref xf_sle_uuid_info_t */
    xf_sle_ssapc_property_found_set_t *chara_set_info;
    /*!< 特征集合信息 ，见 @ref xf_sle_ssapc_property_found_set_t */
} xf_sle_ssapc_service_found_t;

/**
 * @brief SLE SSAPC 搜寻到的服务集合信息
 */
typedef struct {
    xf_sle_ssapc_service_found_t *set;  /*!< 服务集合，见 @ref xf_sle_ssapc_service_found_t */
    uint16_t cnt;                       /*!< 搜寻到的个数 */
} xf_sle_ssapc_service_found_set_t;

/**
 * @brief SLE SSAPC 事件
 */
typedef enum {
    XF_SLE_SSAPC_EVT_EXCHANGE_INFO = _XF_SLE_COMMON_EVT_ENUM_END,   /*!< INFO (MTU) 协商事件 */
    XF_SLE_SSAPC_EVT_WRITE_CFM = _XF_SLE_COMMON_EVT_ENUM_END,       /*!< 接收到写确认事件 */
    XF_SLE_SSAPC_EVT_READ_CFM,                                      /*!< 接收到读确认事件 */
    XF_SLE_SSAPC_EVT_NOTIFICATION,                                  /*!< 接接收到通知事件 */
    XF_SLE_SSAPC_EVT_INDICATION,                                    /*!< 接接收到指示事件 */
} xf_sle_ssapc_event_t;

/**
 * @brief SLE SSAPC INFO 协商事件的参数
 */
typedef struct {
    uint8_t app_id;         /*!< 服务端 (应用) ID */
    uint16_t conn_id;       /*!< 链接(连接) ID */
    uint32_t mtu_size;      /*!< MTU 大小 */
    uint16_t version;       /*!< 版本 */
} xf_sle_ssapc_evt_param_exchange_info_t;
/**
 * @brief SLE 接收到读确认事件的参数
 */
typedef struct {
    uint16_t conn_id;       /*!< 链接(连接) ID */
    uint16_t handle;        /*!< 属性句柄 (property) */
    uint8_t  type;          /*!< 属性类型 (property) */
    uint16_t data_len;      /*!< 数据长度 */
    uint8_t  *data;         /*!< 数据内容 */
} xf_sle_ssapc_evt_param_read_cfm_t;

/**
 * @brief SLE 接收到写确认事件的参数
 */
typedef struct {
    uint16_t conn_id;       /*!< 链接(连接) ID */
    uint16_t handle;        /*!< 属性句柄 (property) */
    uint8_t  type;          /*!< 属性类型 (property) */
} xf_sle_ssapc_evt_param_write_cfm_t;

/**
 * @brief SLE 接收到通知或指示事件的参数
 */
typedef struct {
    uint16_t conn_id;       /*!< 链接(连接) ID */
    uint16_t handle;        /*!< 属性句柄 (property) */
    uint8_t  type;          /*!< 属性类型 (property) */
    uint16_t data_len;      /*!< 数据长度 */
    uint8_t  *data;         /*!< 数据内容 */
} xf_sle_ssapc_evt_param_ntf_t, xf_sle_ssapc_evt_param_ind_t;

/**
 * @brief SLE SSAPC 客户端事件回调参数
 */
typedef union _xf_sle_ssapc_evt_cb_param_t {
    xf_sle_common_evt_param_connect_t connect;  /*!< 连接事件的参数，
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
    xf_sle_common_evt_param_seek_result_t seek_result; 
                                                /*!< 收到扫描结果事件的参数，
                                                 *  @ref xf_sle_common_evt_param_seek_result_t
                                                 *  XF_SLE_COMMON_EVT_SEEK_RESULT
                                                 */
    xf_sle_ssapc_evt_param_exchange_info_t info;
                                                /*!< 接收到 INFO 协商事件的参数，
                                                 *  @ref xf_sle_ssapc_evt_param_exchange_info_t
                                                 *  XF_SLE_SSAPC_EVT_EXCHANGE_INFO
                                                 */
    xf_sle_ssapc_evt_param_read_cfm_t read_cfm; /*!< 接收到读确认事件的参数，
                                                 *  @ref xf_sle_ssapc_evt_param_read_cfm_t
                                                 *  XF_SLE_SSAPC_EVT_READ_CFM
                                                 */
    xf_sle_ssapc_evt_param_write_cfm_t write_cfm;     
                                                /*!< 接收到写确认事件的参数，
                                                 *  @ref xf_sle_ssapc_evt_param_write_cfm_t
                                                 *  XF_SLE_SSAPC_EVT_WRITE_CFM
                                                 */
    xf_sle_ssapc_evt_param_ntf_t ntf;           /*!< 接收到通知事件的参数，
                                                 *  @ref xf_sle_ssapc_evt_param_ntf_t
                                                 *  XF_SLE_SSAPC_EVT_NOTIFICATION
                                                 */
    xf_sle_ssapc_evt_param_ind_t ind;           /*!< 接收到通知事件的参数，
                                                 *  @ref xf_sle_ssapc_evt_param_ind_t
                                                 *  XF_SLE_SSAPC_EVT_INDICATION
                                                 */
} xf_sle_ssapc_evt_cb_param_t;

/**
 * @brief SLE SSAPC 事件回调函数原型
 *
 * @param event 事件，见 @ref xf_sle_ssapc_event_t
 * @param param 事件回调参数，见 @ref xf_sle_ssapc_evt_cb_param_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
typedef xf_err_t (*xf_sle_ssapc_event_cb_t)(
    xf_sle_ssapc_event_t event,
    xf_sle_ssapc_evt_cb_param_t *param);
    
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

#endif /* __XF_SLE_SSAP_CLIENT_TYPES_H__ */
