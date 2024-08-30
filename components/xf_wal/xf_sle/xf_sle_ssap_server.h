/**
 * @file xf_sle_ssap_server.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_SLE_SSAP_SERVER_H__
#define __XF_SLE_SSAP_SERVER_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sle_types.h"
#include "xf_sle_ssap_server_types.h"

#if XF_SLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

// 注册 ssap 服务端应用
xf_err_t xf_sle_ssaps_register_app(
    xf_sle_uuid_info_t *app_uuid, uint8_t *app_id);

// 注销 ssap 服务端应用
xf_err_t xf_sle_ssaps_unregister_app(uint8_t app_id);

/* TODO 支持多服务添加到一个 profile  */
xf_err_t xf_sle_ssaps_add_service_to_app(
    uint8_t app_id, xf_sle_ssaps_service_t *service);

// 启动一个 ssap 服务
xf_err_t xf_sle_ssaps_start_service(
    uint8_t app_id, uint16_t service_handle);

// 停止一个 ssap 服务
xf_err_t xf_sle_ssaps_stop_service(
    uint8_t app_id, uint16_t service_handle);

// 删除所有 GATT 服务
xf_err_t xf_ble_ssaps_del_services_all(uint8_t app_id);

// 发送请求的响应
xf_err_t xf_ble_ssaps_send_response(
    uint8_t app_id, uint16_t conn_id, uint32_t trans_id,
    xf_sle_ssap_err_t err_code, xf_sle_ssaps_response_value_t *rsp);

/**
 * @if Eng
 * @brief  Send indication or notification to remote device.
 * @par Description: Send indication or notification to remote device,
 *                   send status depend on character descriptor: client property configuration.
 *                   value = 0x0000: notification and indication not allowed
 *                   value = 0x0001: notification allowed
 *                   value = 0x0002: indication allowed
 * @param  [in] app_id server ID.
 * @param  [in] conn_id   connection ID，To send packets to all peer ends, enter conn_id = 0xffff.
 * @param  [in] param     notify/indicate parameter.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @par Depends:
 * @li sle_ssap_stru.h
 * @else
 * @brief  向对端发送通知或指示。
 * @par Description: 向对端发送通知或指示，具体发送状态取决于特征描述符：客户端特征配置
 *                   value = 0x0000：不允许通知和指示
 *                   value = 0x0001：允许通知
 *                   value = 0x0002：允许指示
 * @param  [in] app_id 服务端 ID。
 * @param  [in] conn_id   连接ID，如果向全部对端发送则输入conn_id = 0xffff；
 * @param  [in] param     通知或指示参数。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败。参考 @ref errcode_t
 * @par 依赖：
 * @li sle_ssap_stru.h
 * @endif
 */
xf_err_t xf_sle_ssaps_send_notify_indicate(
    uint8_t app_id, uint16_t conn_id,
    xf_sle_ssaps_ntf_ind_t *param);

/**
 * @if Eng
 * @brief  Set server info before connected.
 * @par Description: Set server info before connected.
 * @param  [in] server_id server ID.
 * @param  [in] info      server info.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @par Depends:
 * @li bts_def.h
 * @else
 * @brief  在连接之前设置服务端info。
 * @par Description: 在连接之前设置服务端info。
 * @param  [in] server_id 服务端ID。
 * @param  [in] info      服务端info。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败。参考 @ref errcode_t
 * @par 依赖：
 * @li bts_def.h
 * @endif
 */
xf_err_t xf_sle_ssaps_set_info(
    uint8_t app_id, xf_sle_ssap_exchange_info_t *info);


typedef xf_err_t (*xf_sle_ssaps_event_cb_t)(
    xf_sle_ssaps_event_t event,
    xf_sle_ssaps_evt_cb_param_t *param);

xf_err_t xf_sle_ssaps_event_cb_register(
    xf_sle_ssaps_event_cb_t evt_cb,
    xf_sle_ssaps_event_t events);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_SSAP_SERVER_H__ */
