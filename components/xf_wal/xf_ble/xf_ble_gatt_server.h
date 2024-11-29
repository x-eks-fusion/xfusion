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
 * @brief BLE GATTS 服务端注册
 *
 * @param[in] app_uuid 要注册的服务端 (应用) 的 UUID ，见 @ref xf_ble_uuid_info_t
 * @param[out] app_id 服务端 (应用) ID
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gatts_app_register(
    xf_ble_uuid_info_t *app_uuid, uint8_t *app_id);

/**
 * @brief BLE GATTS 服务端注销
 *
 * @param app_id 服务端 (应用) ID
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gatts_app_unregister(uint8_t app_id);

/* TODO 支持多服务添加到一个 profile  */

/**
 * @brief BLE GATTS 向服务端 (应用) 添加服务
 *
 * @param app_id 服务端 (应用) ID
 * @param service 要添加的服务信息 ，见 @ref xf_ble_gatts_service_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gatts_add_service_to_app_profile(
    uint8_t app_id, xf_ble_gatts_service_t *service);

/**
 * @brief BLE GATTS 服务开启
 *
 * @param app_id 服务端 (应用) ID
 * @param service_handle 指定的服务句柄
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gatts_start_service(
    uint8_t app_id, uint16_t service_handle);
/**
 * @brief BLE GATTS 服务停止
 *
 * @param app_id 服务端 (应用) ID
 * @param service_handle 指定的服务句柄
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gatts_stop_service(
    uint8_t app_id, uint16_t service_handle);

/**
 * @brief BLE GATTS 删除所有服务
 *
 * @param app_id 服务端 (应用) ID
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gatts_del_services_all(uint8_t app_id);

/**
 * @brief BLE GATTS 发送通知
 *
 * @param app_id 服务端 (应用) ID
 * @param conn_id 链接 (连接) ID
 * @param param 发送的通知的信息，见 @ref xf_ble_gatts_ntf_ind_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gatts_send_notification(
    uint8_t app_id, uint16_t conn_id,
    xf_ble_gatts_ntf_ind_t *param);

/**
 * @brief BLE GATTS 发送指示
 *
 * @param app_id 服务端 (应用) ID
 * @param conn_id 链接 (连接) ID
 * @param param 发送的指示的信息，见 @ref xf_ble_gatts_ntf_ind_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gatts_send_indication(
    uint8_t app_id, uint16_t conn_id,
    xf_ble_gatts_ntf_ind_t *param);

/**
 * @brief BLE GATTS 发送 (请求的) 响应
 *
 * @param app_id 服务端 (应用) ID
 * @param conn_id 链接 (连接) ID
 * @param trans_id 传输 ID
 * @param err_code 错误码，见 @ref xf_ble_gatt_err_t
 * @param rsp 发送的响应的信息，见 @ref xf_ble_gatts_response_value_t
 * @return xf_err_t
 */
xf_err_t xf_ble_gatts_send_response(
    uint8_t app_id, uint16_t conn_id, uint32_t trans_id,
    xf_ble_gatt_err_t err_code, xf_ble_gatts_response_value_t *rsp);

/**
 * @brief BLE GATTS 事件回调函数原型
 *
 * @param event 事件，见 @ref xf_ble_gatts_event_t
 * @param param 事件回调参数，见 @ref xf_ble_gatts_evt_cb_param_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
typedef xf_err_t (*xf_ble_gatts_event_cb_t)(
    xf_ble_gatts_event_t event,
    xf_ble_gatts_evt_cb_param_t param);

/**
 * @brief BLE GATTS 事件回调注册
 *
 * @param evt_cb 事件回调，见 @ref xf_ble_gatts_event_cb_t
 * @param events 事件，见 @ref xf_ble_gatts_event_t
 * @note 当前仅支持所有事件注册在同一个回调，暂不支持指定事件对应单独的回调，
 * 所以 参数 'events' 填 0 即可
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gatts_event_cb_register(
    xf_ble_gatts_event_cb_t evt_cb,
    xf_ble_gatts_event_t events);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GATT_SERVER_H__ */
