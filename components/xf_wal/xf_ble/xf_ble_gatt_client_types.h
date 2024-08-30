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
  * @brief Gatt write type
  */
typedef enum {
    XF_BLE_GATT_WRITE_TYPE_NO_RSP  =   1,                      /*!< Gatt write attribute need no response */
    XF_BLE_GATT_WRITE_TYPE_WITH_RSP,                                /*!< Gatt write attribute need remote response */
} xf_ble_gattc_write_type_t;

/**
 * @brief 搜寻到的特征描述符
 */
typedef struct {
    uint16_t handle;            // 特征描述符句柄
    xf_bt_uuid_info_t uuid;             // 特征描述符uuid
} xf_ble_gattc_desc_found_t;

/**
 * @brief 搜寻到的特征描述符集合
 */
typedef struct {
    xf_ble_gattc_desc_found_t *set;        // 描述符集合
    uint16_t cnt;
} xf_ble_gattc_desc_found_set_t;

/**
 * @brief 搜寻到的特征
 */
typedef struct {
    xf_bt_uuid_info_t uuid;                          // 特征uuid
    xf_bt_attr_handle_t handle;                 // 特征句柄
    xf_bt_attr_handle_t value_handle;           // 特征值句柄
    xf_ble_gatt_chara_property_t properties;    //特征特性
    xf_ble_gattc_desc_found_set_t *desc_set_info;
} xf_ble_gattc_chara_found_t;

/**
 * @brief 搜寻到的特征集合
 */
typedef struct {
    xf_ble_gattc_chara_found_t *set;      // 特征集合
    uint16_t cnt;
} xf_ble_gattc_chara_found_set_t;

/**
 * @brief 搜寻到的服务
 */
typedef struct {
    xf_bt_attr_handle_t start_hdl;  // 服务起始句柄
    xf_bt_attr_handle_t end_hdl;    // 服务结束句柄
    xf_bt_uuid_info_t uuid;              // 服务uuid
    xf_ble_gattc_chara_found_set_t *chara_set_info;
} xf_ble_gattc_service_found_t;

/**
 * @brief 搜寻到的服务集合
 */
typedef struct {
    xf_ble_gattc_service_found_t *set;
    uint16_t cnt;
} xf_ble_gattc_service_found_set_t;


typedef enum {
    XF_BLE_GAP_LINK_ROLE_MASTER = 0,
    XF_BLE_GAP_LINK_ROLE_SLAVE  = 1
} xf_ble_gap_link_role_type_t;

/**
 * @brief Ble scan result event type, to indicate the
 *        result is scan response or advertising data or other
 */
typedef enum {
    XF_BLE_GAP_SCANNED_ADV_TYPE_CONN            = 0x00,        /*!< Connectable undirected advertising (ADV_IND) */
    XF_BLE_GAP_SCANNED_ADV_TYPE_CONN_DIR        = 0x01,        /*!< Connectable directed advertising (ADV_DIRECT_IND) */
    XF_BLE_GAP_SCANNED_ADV_TYPE_SCAN            = 0x02,        /*!< Scannable undirected advertising (ADV_SCAN_IND) */
    XF_BLE_GAP_SCANNED_ADV_TYPE_NONCONN         = 0x03,         /*!< Non connectable undirected advertising (ADV_NONCONN_IND) */
    XF_BLE_GAP_SCANNED_ADV_TYPE_SCAN_RSP        = 0x04,        /*!< Scan Response (SCAN_RSP) */
} xf_ble_gap_scanned_adv_type_t;

typedef struct {
    xf_err_t status;
    // esp_bt_status_t status;                     /*!< Indicate the set scan param operation success status */
} xf_ble_evt_param_scan_t;            

typedef struct {
    int rssi;                                   /*!< Searched device's RSSI */
    xf_bt_dev_addr_t addr_scanned;              /*!< Bluetooth device address which has been searched */
    xf_ble_gap_scanned_adv_type_t adv_type;     /*!< Ble scanned adv type */
    uint8_t adv_data_len;                       /*!< Adv data length */
    uint8_t *adv_data;                          /*!< Advertising data */
    // search_evt;                              /*!< Search event type */
    // int num_resps;                              /*!< Scan result number */
    // uint8_t scan_rsp_len;                       /*!< Scan response length */
    // uint8_t  ble_adv[];     /*!< Received EIR */
} xf_ble_evt_param_scan_result_t;

typedef struct {
    uint16_t conn_id;               /*!< Connection id */
    uint16_t conn_handle;           /*!< HCI connection handle */
    xf_ble_gap_link_role_type_t link_role; /*!< Link role : master role = 0  ; slave role = 1*/
    xf_bt_dev_addr_t peer_addr;       /*!< Remote bluetooth device address */
    // conn_params; /*!< current connection parameters */
} xf_ble_evt_param_connect_t;

typedef struct {
    uint16_t conn_id;               /*!< Connection id */
    xf_bt_dev_addr_t peer_addr;       /*!< Remote bluetooth device address */
    xf_ble_gap_disconnect_reason_t reason;  /*!< disconnection reason */
} xf_ble_evt_param_disconnect_t;

typedef struct {
    uint8_t app_id;
    // status;      /*!< Operation status */
    uint16_t conn_id;              /*!< Connection id */
} xf_ble_evt_param_gattc_service_found_t;

typedef struct {
    uint8_t app_id;
    // status;       /*!< Operation status */
    uint16_t conn_id;               /*!< Connection id */
    uint16_t mtu;                   /*!< MTU size */
} xf_ble_evt_param_gattc_mtu_changed_t;

typedef struct {
    uint8_t app_id;
    // status;       /*!< Operation status */
    uint16_t conn_id;               /*!< Connection id */
    uint16_t handle;                /*!< The Characteristic or descriptor handle */
    uint16_t offset;                /*!< The prepare write offset, this value is valid only when prepare write */
} xf_ble_evt_param_gattc_write_t;

typedef struct {
    uint8_t app_id;
    // status;       /*!< Operation status */
    uint16_t conn_id;               /*!< Connection id */
    uint16_t chara_handle;                /*!< Characteristic handle */
    uint8_t *chara_value;                 /*!< Characteristic value */
    uint16_t chara_value_len;             /*!< Characteristic value length */
} xf_ble_evt_param_gattc_read_cmpl_t;

typedef struct {
    uint8_t app_id;
    uint16_t conn_id;               /*!< Connection id */
    xf_bt_dev_addr_t peer_addr;       /*!< Remote bluetooth device address */
    uint16_t handle;                /*!< The Characteristic or descriptor handle */
    uint16_t value_len;             /*!< Notify attribute value */
    uint8_t *value;                 /*!< Notify attribute value */
    bool is_notify;                 /*!< True means notify, false means indicate */
} xf_ble_evt_param_gattc_notify_t;

/**
 * @brief Gatt client callback parameters union
 */
typedef union {

    /**
     * @brief XF_BLE_GAP_EVT_SCAN_PARAM_SET
     */
    xf_ble_evt_param_scan_t scan_param;
    /**
     * @brief XF_BLE_GAP_EVT_SCAN_RESULT
     */
    
    xf_ble_evt_param_scan_result_t scan_result;

    /* XF_BLE_GAP_EVT_CONNECT */
    xf_ble_evt_param_connect_t connect;

    /* XF_BLE_GAP_EVT_DISCONNECT */
    xf_ble_evt_param_disconnect_t disconnect;

    /* XF_BLE_GATTC_EVT_DISCOVER_SERVICE_COMPLETE */
    xf_ble_evt_param_gattc_service_found_t service_found;

    /* XF_BLE_GATTC_EVT_MTU_CHANGED */
    xf_ble_evt_param_gattc_mtu_changed_t mtu_changed;

    /* XF_BLE_GATTC_EVT_WRITE_COMPLETE */
    xf_ble_evt_param_gattc_write_t write;
    /* XF_BLE_GATTC_EVT_READ_COMPLETE */
    xf_ble_evt_param_gattc_read_cmpl_t read_cmpl;

    /* XF_BLE_GATTC_EVT_RECV_NOTIFICATION_OR_INDICATION */
    xf_ble_evt_param_gattc_notify_t notify;
} xf_ble_gattc_evt_cb_param_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GATT_CLIENT_TYPES_H__ */
