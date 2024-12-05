/**
 * @file xf_ble_gatt_client_types.h
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-08-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_BLE_GATT_CLIENT_TYPES_H__
#define __XF_BLE_GATT_CLIENT_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#include "xf_ble_types.h"
#include "xf_ble_gap_types.h"
#include "xf_ble_gatt_common.h"

#if XF_BLE_IS_ENABLE || defined(__DOXYGEN__)

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_wal_ble_gatt
 * @endcond
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief BLE GATTC 写请求的类型
 */
typedef enum {
    XF_BLE_GATT_WRITE_TYPE_NO_RSP  =   1,   /*!< 无需 (对端) 响应 (写命令) */
    XF_BLE_GATT_WRITE_TYPE_WITH_RSP,        /*!< 需要 (对端) 回应 (写请求) */
} xf_ble_gattc_write_type_t;

/**
 * @brief BLE GATTC 搜寻到的特征描述符
 */
typedef struct {
    uint16_t handle;                                /*!< 特征描述符句柄 */
    xf_ble_uuid_info_t uuid;                         /*!< 特征描述符 UUID ，见 @ref xf_ble_gattc_desc_found_t */
} xf_ble_gattc_desc_found_t;

/**
 * @brief BLE GATTC 搜寻到的特征描述符集合信息
 */
typedef struct {
    xf_ble_gattc_desc_found_t *set;         /*!< 描述符集合 ，见 @ref xf_ble_gattc_desc_found_t */
    uint16_t cnt;                           /*!< 搜寻到的个数 */
} xf_ble_gattc_desc_found_set_t;

/**
 * @brief BLE GATTC 搜寻到的特征
 */
typedef struct {
    xf_ble_uuid_info_t uuid;                         /*!< 特征 UUID ，见 @ref xf_ble_uuid_info_t */
    xf_ble_attr_handle_t handle;                     /*!< 特征句柄，见 @ref xf_ble_attr_handle_t */
    xf_ble_attr_handle_t value_handle;               /*!< 特征值句柄，见 @ref xf_ble_attr_handle_t */
    xf_ble_gatt_chara_property_t properties;        /*!< 特征特性，见 @ref xf_ble_gatt_chara_property_t */
    xf_ble_gattc_desc_found_set_t
    *desc_set_info;   /*!< 特征描述符集合信息，见 @ref xf_ble_gattc_desc_found_set_t */
} xf_ble_gattc_chara_found_t;

/**
 * @brief BLE GATTC 搜寻到的特征集合信息
 */
typedef struct {
    xf_ble_gattc_chara_found_t *set;        /*!< 特征集合，见 @ref xf_ble_gattc_chara_found_t */
    uint16_t cnt;                           /*!< 搜寻到的个数 */
} xf_ble_gattc_chara_found_set_t;

/**
 * @brief BLE GATTC 搜寻到的服务
 */
typedef struct {
    xf_ble_attr_handle_t start_hdl;                  /*!< 服务起始句柄，见 @ref xf_ble_attr_handle_t */
    xf_ble_attr_handle_t end_hdl;                    /*!< 服务结束句柄，见 @ref xf_ble_attr_handle_t */
    xf_ble_uuid_info_t uuid;                         /*!< 服务 UUID ，见 @ref xf_ble_uuid_info_t */
    xf_ble_gattc_chara_found_set_t *chara_set_info; /*!< 特征集合信息 ，见 @ref xf_ble_uuid_info_t */
} xf_ble_gattc_service_found_t;

/**
 * @brief BLE GATTC 搜寻到的服务集合信息
 */
typedef struct {
    xf_ble_gattc_service_found_t *set;      /*!< 服务集合，见 @ref xf_ble_gattc_service_found_t */
    uint16_t cnt;                           /*!< 搜寻到的个数 */
} xf_ble_gattc_service_found_set_t;

/**
 * @brief BLE GATTC MTU 协商事件的参数
 */
typedef struct {
    uint8_t app_id;                         /*!< 应用 ID */
    uint16_t conn_id;                       /*!< 链接(连接) ID */
    uint16_t mtu;                           /*!< MTU 大小 */
} xf_ble_gattc_evt_param_exchange_mtu_t;

/**
 * @brief BLE GATTC 写确认事件的参数
 */
typedef struct {
    uint8_t app_id;                         /*!< 应用 ID */
    uint16_t conn_id;                       /*!< 链接(连接) ID */
    uint16_t handle;                        /*!< 特征或描述符的句柄 */
    uint16_t offset;                        /*!< The prepare write offset, this value is valid only when prepare write */
} xf_ble_gattc_evt_param_write_cfm_t;

/**
 * @brief BLE GATTC 读确认事件的参数
 */
typedef struct {
    uint8_t app_id;                         /*!< 应用 ID */
    uint16_t conn_id;                       /*!< 链接(连接) ID */
    uint16_t handle;                        /*!< 句柄 */
    uint8_t *value;                         /*!< 值 */
    uint16_t value_len;                     /*!< 值长度 */
} xf_ble_gattc_evt_param_read_cfm_t;

/**
 * @brief BLE GATTC 接收到通知或指示事件的参数
 */
typedef struct {
    uint8_t app_id;
    uint16_t conn_id;                       /*!< 链接(连接) ID */
    xf_ble_addr_t peer_addr;                /*!< 对端地址，见 @ref xf_ble_addr_t */
    uint16_t handle;                        /*!< 特征或描述符的句柄 */
    uint16_t value_len;                     /*!< 通知或指示的属性值长度 */
    uint8_t *value;                         /*!< 通知或指示的属性值 */
} xf_ble_gattc_evt_param_ntf_t, xf_ble_gattc_evt_param_ind_t;

/**
 * @brief BLE GATTC 客户端事件回调参数
 */
typedef union {
    xf_ble_common_evt_param_connect_t connect;  /*!< 连接事件的参数，
                                                 *  @ref xf_ble_common_evt_param_connect_t
                                                 *  XF_BLE_COMMON_EVT_CONNECT
                                                 */
    xf_ble_common_evt_param_disconnect_t disconnect;   
                                                /*!< 断连事件的参数，
                                                 *  @ref xf_ble_common_evt_param_disconnect_t
                                                 *  XF_BLE_COMMON_EVT_DISCONNECT
                                                 */
    xf_ble_common_evt_param_scan_result_t scan_result; 
                                                /*!< 收到扫描结果事件的参数，
                                                 *  @ref xf_ble_common_evt_param_scan_result_t
                                                 *  XF_BLE_COMMON_EVT_SCAN_RESULT
                                                 */
    xf_ble_gattc_evt_param_exchange_mtu_t mtu;      /*!< MTU 协商事件的参数，
                                                 *  @ref xf_ble_gattc_evt_param_exchange_mtu_t
                                                 *  XF_BLE_GATTC_EVT_EXCHANGE_MTU,
                                                 */
    xf_ble_gattc_evt_param_read_cfm_t read_cfm; /*!< 读确认事件的参数，
                                                 *  @ref xf_ble_gattc_evt_param_read_cfm_t
                                                 *  XF_BLE_GATTC_EVT_READ_CFM
                                                 */
    xf_ble_gattc_evt_param_write_cfm_t write_cfm;
                                                /*!< 写确认事件的参数，
                                                 *  @ref xf_ble_gattc_evt_param_write_cfm_t
                                                 *  XF_BLE_GATTC_EVT_WRITE_CFM
                                                 */
    xf_ble_gattc_evt_param_ntf_t ntf;           /*!< 接收到通知事件的参数，
                                                 *  @ref xf_ble_gattc_evt_param_ntf_t
                                                 *  XF_BLE_GATTC_EVT_NOTIFICATION
                                                 */
    xf_ble_gattc_evt_param_ind_t ind;           /*!< 接收到指示事件的参数，
                                                 *  @ref xf_ble_gattc_evt_param_ind_t
                                                 *  XF_BLE_GATTC_EVT_INDICATION
                                                 */
} xf_ble_gattc_evt_cb_param_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_wal_ble_gatt
 * @}
 */

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GATT_CLIENT_TYPES_H__ */
