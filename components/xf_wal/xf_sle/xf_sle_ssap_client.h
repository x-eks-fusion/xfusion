/**
 * @file xf_sle_ssap_client.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_SLE_SSAP_CLIENT_H__
#define __XF_SLE_SSAP_CLIENT_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sle_types.h"
#include "xf_sle_ssap_client_types.h"

#if XF_SLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

// 注册ssap客户端
xf_err_t xf_sle_ssapc_register_app(
    xf_sle_uuid_info_t *app_uuid, uint8_t *app_id);

// 注销ssap客户端
xf_err_t xf_sle_ssapc_unregister_app(uint8_t app_id);

xf_err_t xf_sle_ssapc_discover_service(
    uint8_t app_id, uint16_t conn_id,
    xf_sle_ssapc_find_struct_param_t *param);

xf_err_t xf_ble_ssapc_discovery_property(
    uint8_t app_id, uint16_t conn_id,
    xf_sle_ssapc_find_struct_param_t *param);

// 以句柄发送（特征值）读请求
xf_err_t xf_sle_ssapc_request_read_by_handle(
    uint8_t app_id, uint16_t conn_id,
    uint8_t type, uint16_t handle);

// 以 UUID 发送（特征值）读请求
xf_err_t xf_sle_ssapc_request_read_by_uuid(
    uint8_t app_id, uint16_t conn_id,
    uint8_t type,
    uint16_t start_handle,
    uint16_t end_handle,
    const xf_sle_uuid_info_t *uuid);

// 以句柄发送（特征值）写数据请求
xf_err_t xf_sle_ssapc_request_write_data(
    uint8_t app_id, uint16_t conn_id,
    uint8_t type, uint16_t handle,
    uint8_t *data, uint16_t data_len);

// 以句柄发送（特征值）写命令请求
xf_err_t xf_sle_ssapc_request_write_cmd(
    uint8_t app_id, uint16_t conn_id,
    uint16_t handle, uint8_t type,
    uint8_t *data, uint16_t data_len);

// 发送交互（协商）请求
xf_err_t xf_sle_ssapc_request_exchange_info(
    uint8_t app_id, uint16_t conn_id,
    xf_sle_ssap_exchange_info_t *param);

typedef xf_err_t (*xf_sle_ssapc_event_cb_t)(
    xf_sle_ssapc_event_t event,
    xf_sle_ssapc_evt_cb_param_t *param);

xf_err_t xf_sle_ssapc_event_cb_register(
    xf_sle_ssapc_event_cb_t evt_cb,
    xf_sle_ssapc_event_t events);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_SSAP_CLIENT_H__ */
