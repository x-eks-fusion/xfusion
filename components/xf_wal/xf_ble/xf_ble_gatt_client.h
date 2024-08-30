/**
 * @file xf_ble_gatt_client.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_BLE_GATT_CLIENT_H__
#define __XF_BLE_GATT_CLIENT_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#include "xf_ble_types.h"
#include "xf_ble_gatt_client_types.h"

#if XF_BLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 
 * 
 * @param app_uuid  [in] app profile uuid
 * @param app_id    [out] app instance id （ APP 实例 ID ）
 * @return xf_err_t 
 */
xf_err_t xf_ble_gattc_register_app_profile(
    xf_bt_uuid_info_t *app_uuid, uint8_t *app_id);

xf_err_t xf_ble_gattc_unregister_app_profile(uint8_t app_id);


// 搜寻（发现）服务（所有或指定（ UUID? ））
// ？获取指定 UUID 的所有服务（本地）
// ？获取指定服务 handle 的服务（本地）

// 如果uuid长度为NULL，发现所有服务，否则按照uuid过滤。
xf_err_t xf_ble_gattc_discover_service(
    uint8_t app_id, uint16_t conn_id,
    xf_bt_uuid_info_t *service_uuid,
    xf_ble_gattc_service_found_set_t **service_set_info);



// 搜寻（发现）特征（所有或指定（ UUID? ））
// ？获取指定服务的所有特征（本地）
// ？获取指定特征UUID的特征（本地）
// ？获取指定特征 handle 的服务特征
// ？获取指定特征 UUID 的服务特征

// 如果uuid长度为NULL，发现所有服务的所有特征，否则按照uuid过滤。
xf_err_t xf_ble_gattc_discover_chara(
    uint8_t app_id, uint16_t conn_id,
    uint16_t start_handle,
    uint16_t end_handle,
    xf_bt_uuid_info_t *chara_uuid,
    xf_ble_gattc_chara_found_set_t **out_chara_set);


// 搜寻（发现）特征描述符（所有或指定（有些平台能指定有些貌似不行）（ handle ））
// ？获取指定特征的所有描述（本地）
// ？获取指定特征 UUID 的描述（本地）
// ？获取指定特征 handle 的描述（本地）
// ？获取特征描述


xf_err_t xf_ble_gattc_discovery_desc_all(
    uint8_t app_id, uint16_t conn_id,
    uint16_t start_handle,
    uint16_t end_handle,
    xf_ble_gattc_desc_found_set_t **out_desc_set);

xf_err_t xf_ble_gattc_discovery_desc_by_chara_uuid(
    uint8_t app_id, uint16_t conn_id,
    uint16_t start_handle,
    uint16_t end_handle,
    xf_bt_uuid_info_t chara_uuid,
    xf_bt_uuid_info_t desc_uuid,
    uint16_t *count );

xf_err_t xf_ble_gattc_discover_desc_by_chara_handle(
    uint8_t app_id, uint16_t conn_id,
    uint16_t start_handle,
    uint16_t end_handle,
    uint16_t chara_handle,
    xf_bt_uuid_info_t desc_uuid,
    uint16_t *count );

// ？获取指定服务或特征的属性数目
// 可以利用以上各个 发现的 api 的 count 参数获取


// 以句柄发送（特征值）读请求
xf_err_t xf_ble_gattc_request_read_by_handle(
    uint8_t app_id, uint16_t conn_id,
    uint16_t chara_val_handle);

// 以 UUID 发送（特征值）读请求
xf_err_t xf_ble_gattc_request_read_by_uuid(
    uint8_t app_id, uint16_t conn_id, 
    uint16_t start_handle,
    uint16_t end_handle, 
    const xf_bt_uuid_info_t *chara_val_uuid);

// 发送（特征值）写请求 - 需要回应
// 发送（特征值）写命令 - 无需回应
xf_err_t xf_ble_gattc_request_write(
    uint8_t app_id, uint16_t conn_id,
    uint16_t chara_val_handle,
    uint8_t *value,
    uint16_t value_len,
    xf_ble_gattc_write_type_t write_type);


// 发送协商MTU 请求
xf_err_t xf_ble_gattc_request_exchange_mtu(
    uint8_t app_id, uint16_t conn_id,
    uint16_t mtu_size);


typedef xf_err_t (*xf_ble_gattc_event_cb_t)(
    xf_ble_gattc_event_t event,
    xf_ble_gattc_evt_cb_param_t param);

xf_err_t xf_ble_gattc_event_cb_register(
    xf_ble_gattc_event_cb_t evt_cb,
    xf_ble_gattc_event_t events);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GATT_CLIENT_H__ */
