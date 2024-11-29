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

#if XF_BLE_IS_ENABLE

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
 * @brief Ble scan result event type, to indicate the
 *        result is scan response or advertising data or other
 */
/**
 * @brief BLE GAP 扫描结果中的事件类型，扫描响应、广播数据或其他类型
 */
typedef enum {
    XF_BLE_GAP_SCANNED_ADV_TYPE_CONN        = 0x00, /*!< Connectable undirected advertising (ADV_IND) */
    XF_BLE_GAP_SCANNED_ADV_TYPE_CONN_DIR    = 0x01, /*!< Connectable directed advertising (ADV_DIRECT_IND) */
    XF_BLE_GAP_SCANNED_ADV_TYPE_SCAN        = 0x02, /*!< Scannable undirected advertising (ADV_SCAN_IND) */
    XF_BLE_GAP_SCANNED_ADV_TYPE_NONCONN     = 0x03, /*!< Non connectable undirected advertising (ADV_NONCONN_IND) */
    XF_BLE_GAP_SCANNED_ADV_TYPE_SCAN_RSP    = 0x04, /*!< Scan Response (SCAN_RSP) */
} xf_ble_gap_scanned_adv_type_t;

/**
 * @brief BLE GAP 扫描参数设置事件的参数
 */
typedef struct {
    xf_err_t status;
} xf_ble_evt_param_scan_t;

/**
 * @brief BLE GAP 收到扫描结果事件的参数
 */
typedef struct {
    int rssi;                                   /*!< 扫到的设备的 RSSI 值 */
    xf_ble_addr_t addr_scanned;              /*!< 扫到的设备的地址，见 @ref xf_ble_addr_t */
    xf_ble_gap_scanned_adv_type_t adv_type;     /*!< 扫到的设备广播类型，见 @ref xf_ble_gap_scanned_adv_type_t */
    uint8_t adv_data_len;                       /*!< 广播数据的长度 (指整个广播数据 AdvData ) */
    uint8_t *adv_data;                          /*!< 广播数据 (指整个广播数据 AdvData ) */
} xf_ble_evt_param_scan_result_t;

/**
 * @brief BLE GAP 连接事件的参数
 */
typedef struct {
    uint16_t conn_id;                       /*!< 链接(连接) ID */
    uint16_t conn_handle;                   /*!< 链接的句柄 */
    xf_ble_gap_link_role_type_t link_role;  /*!< 链路角色，见 @ref xf_ble_gap_link_role_type_t */
    xf_ble_addr_t peer_addr;             /*!< 对端地址，见 @ref xf_ble_addr_t */
} xf_ble_evt_param_connect_t;

/**
 * @brief BLE GAP 断连事件的参数
 */
typedef struct {
    uint16_t conn_id;                       /*!< 链接(连接) ID */
    xf_ble_addr_t peer_addr;             /*!< 对端地址，见 @ref xf_ble_addr_t */
    xf_ble_gap_disconnect_reason_t reason;  /*!< 断连原因，见 @ref xf_ble_gap_disconnect_reason_t */
} xf_ble_evt_param_disconnect_t;

/**
 * @brief BLE GATTC 搜寻到服务结构事件的参数
 */
typedef struct {
    uint8_t app_id;                         /*!< 应用 ID */
    uint16_t conn_id;                       /*!< 链接(连接) ID */
} xf_ble_evt_param_gattc_service_found_t;

/**
 * @brief BLE GATTC MTU 改变事件的参数
 */
typedef struct {
    uint8_t app_id;                         /*!< 应用 ID */
    uint16_t conn_id;                       /*!< 链接(连接) ID */
    uint16_t mtu;                           /*!< MTU 大小 */
} xf_ble_evt_param_gattc_mtu_changed_t;

/**
 * @brief BLE GATTC 写完成事件的参数
 */
typedef struct {
    uint8_t app_id;                         /*!< 应用 ID */
    uint16_t conn_id;                       /*!< 链接(连接) ID */
    uint16_t handle;                        /*!< 特征或描述符的句柄 */
    uint16_t offset;                        /*!< The prepare write offset, this value is valid only when prepare write */
} xf_ble_evt_param_gattc_write_t;

/**
 * @brief BLE GATTC 读完成事件的参数
 */
typedef struct {
    uint8_t app_id;                         /*!< 应用 ID */
    uint16_t conn_id;                       /*!< 链接(连接) ID */
    uint16_t chara_handle;                  /*!< 特征句柄 */
    uint8_t *chara_value;                   /*!< 特征值 */
    uint16_t chara_value_len;               /*!< 特征值长度 */
} xf_ble_evt_param_gattc_read_cmpl_t;

/**
 * @brief BLE GATTC 接收到通知或指示事件的参数
 */
typedef struct {
    uint8_t app_id;
    uint16_t conn_id;                       /*!< 链接(连接) ID */
    xf_ble_addr_t peer_addr;             /*!< 对端地址，见 @ref xf_ble_addr_t */
    uint16_t handle;                        /*!< 特征或描述符的句柄 */
    uint16_t value_len;                     /*!< 通知或指示的属性值长度 */
    uint8_t *value;                         /*!< 通知或指示的属性值 */
    bool is_ntf;                            /*!< 是否是通知，否则是指示 */
} xf_ble_evt_param_gattc_notify_t;

/**
 * @brief BLE GATTC 客户端事件回调参数
 */
typedef union {
    xf_ble_evt_param_scan_t scan_param;         /*!< 扫描参数设置事件的参数，
                                                 *  @ref xf_ble_evt_param_scan_t
                                                 *  XF_BLE_GAP_EVT_SCAN_PARAM_SET
                                                 */
    xf_ble_evt_param_scan_result_t scan_result; /*!< 收到扫描结果事件的参数，
                                                 *  @ref xf_ble_evt_param_scan_result_t
                                                 *  XF_BLE_GAP_EVT_SCAN_RESULT
                                                 */
    xf_ble_evt_param_connect_t connect;         /*!< 连接事件的参数，
                                                 *  @ref xf_ble_evt_param_connect_t
                                                 *  XF_BLE_GAP_EVT_CONNECT
                                                 */
    xf_ble_evt_param_disconnect_t disconnect;   /*!< 断连事件的参数，
                                                 *  @ref xf_ble_evt_param_disconnect_t
                                                 *  XF_BLE_GAP_EVT_DISCONNECT
                                                 */
    xf_ble_evt_param_gattc_service_found_t service_found;
    /*!< 搜寻到服务结构事件的参数，
     *  @ref xf_ble_evt_param_gattc_service_found_t
     *  XF_BLE_GATTC_EVT_DISCOVER_SERVICE_COMPLETE
     */
    xf_ble_evt_param_gattc_mtu_changed_t mtu_changed;
    /*!< MTU 变更事件的参数，
     *  @ref xf_ble_evt_param_gattc_mtu_changed_t
     *  XF_BLE_GATTC_EVT_MTU_CHANGED
     */
    xf_ble_evt_param_gattc_write_t write;       /*!< 写完成事件的参数，
                                                 *  @ref xf_ble_evt_param_gattc_write_t
                                                 *  XF_BLE_GATTC_EVT_WRITE_COMPLETE
                                                 */
    xf_ble_evt_param_gattc_read_cmpl_t read_cmpl;
    /*!< 读完成事件的参数，
     *  @ref xf_ble_evt_param_gattc_read_cmpl_t
     *  XF_BLE_GATTC_EVT_READ_COMPLETE
     */
    xf_ble_evt_param_gattc_notify_t notify;     /*!< 接收到通知或指示事件的参数，
                                                 *  @ref xf_ble_evt_param_gattc_notify_t
                                                 *  XF_BLE_GATTC_EVT_RECV_NOTIFICATION_OR_INDICATION
                                                 */
} xf_ble_gattc_evt_cb_param_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GATT_CLIENT_TYPES_H__ */
