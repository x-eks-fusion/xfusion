/**
 * @file xf_ble_gatt_server.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_BLE_GATT_SERVER_H__
#define __XF_BLE_GATT_SERVER_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#include "xf_ble_types.h"
#include "xf_ble_gatt_server_types.h"

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
xf_err_t xf_ble_gatts_register_app_profile(
    xf_bt_uuid_info_t *app_uuid, uint8_t *app_id);

xf_err_t xf_ble_gatts_unregister_app_profile(uint8_t app_id);

/* TODO 支持多服务添加到一个 profile  */
xf_err_t xf_ble_gatts_add_service_set_to_app_profile(
    uint8_t app_id, xf_ble_gatts_service_t **service_set);

xf_err_t xf_ble_gatts_add_service_to_app_profile(
    uint8_t app_id, xf_ble_gatts_service_t *service);

// 启动一个 GATT 服务
xf_err_t xf_ble_gatts_start_service(
    uint8_t app_id, uint16_t service_handle);

// 停止一个 GATT 服务
xf_err_t xf_ble_gatts_stop_service(
    uint8_t app_id, uint16_t service_handle);

// 删除所有 GATT 服务
xf_err_t xf_ble_gatts_del_services_all(uint8_t app_id);


// 发送指示或通知（向客户端）
xf_err_t xf_ble_gatts_send_notification(
    uint8_t app_id, uint16_t conn_id,
    xf_ble_gatts_ntf_ind_t *param);

// 发送指示（向客户端）
xf_err_t xf_ble_gatts_send_indication(
    uint8_t app_id, uint16_t conn_id,
    xf_ble_gatts_ntf_ind_t *param);

// 发送请求的响应
xf_err_t xf_ble_gatts_send_response(
    uint8_t app_id, uint16_t conn_id, uint32_t trans_id,
    xf_ble_gatt_err_t err_code, xf_ble_gatts_response_value_t *rsp);

typedef xf_err_t (*xf_ble_gatts_event_cb_t)(
    xf_ble_gatts_event_t event,
    xf_ble_gatts_evt_cb_param_t param);

xf_err_t xf_ble_gatts_event_cb_register(
    xf_ble_gatts_event_cb_t evt_cb,
    xf_ble_gatts_event_t events);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GATT_SERVER_H__ */
