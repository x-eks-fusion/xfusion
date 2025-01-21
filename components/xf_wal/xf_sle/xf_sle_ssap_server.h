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

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief SLE SSAPS 服务端注册
 *
 * @param[in] app_uuid 要注册的服务端 (应用) 的 UUID ，见 @ref xf_sle_uuid_info_t
 * @param[out] app_id 服务端 (应用) ID
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssaps_app_register(
    xf_sle_uuid_info_t *app_uuid, uint8_t *app_id);

/**
 * @brief SLE SSAPS 服务端注销
 *
 * @param app_id 服务端 (应用) ID
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssaps_app_unregister(uint8_t app_id);

/* TODO 支持多服务添加到一个 profile  */

/**
 * @brief SLE SSAPS 服务端 (应用) 添加服务
 *
 * @param app_id 服务端 (应用) ID
 * @param service 要添加的服务信息 ，见 @ref xf_sle_ssaps_service_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssaps_add_service_to_app(
    uint8_t app_id, xf_sle_ssaps_service_t *service);

/**
 * @brief SLE SSAPS 服务开启
 *
 * @param app_id 服务端 (应用) ID
 * @param service_handle 指定的服务句柄
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssaps_start_service(
    uint8_t app_id, uint16_t service_handle);

/**
 * @brief SLE SSAPS 服务停止
 *
 * @param app_id 服务端 (应用) ID
 * @param service_handle 指定的服务句柄
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssaps_stop_service(
    uint8_t app_id, uint16_t service_handle);

/**
 * @brief SLE SSAPS 删除所有服务
 *
 * @param app_id 服务端 (应用) ID
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssaps_del_services_all(uint8_t app_id);

/**
 * @brief SLE SSAPS 发送 (请求的) 响应
 *
 * @param app_id 服务端 (应用) ID
 * @param conn_id 链接 (连接) ID
 * @param trans_id 传输 ID
 * @param err_code 错误码，见 @ref xf_sle_ssap_err_t
 * @param rsp 发送的响应的信息，见 @ref xf_sle_ssaps_response_value_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssaps_send_response(
    uint8_t app_id, uint16_t conn_id, uint32_t trans_id,
    xf_sle_ssap_err_t err_code, xf_sle_ssaps_response_value_t *rsp);

/**
 * @brief SLE SSAPS 发送通知或指示
 * @par Description: 向对端发送通知或指示，具体发送状态取决于特征描述符：客户端特征配置
 *                   value = 0x0000：不允许通知和指示
 *                   value = 0x0001：允许通知
 *                   value = 0x0002：允许指示
 * @param app_id 服务端 (应用) ID
 * @param conn_id 链接 (连接) ID ，如果向全部对端发送则输入conn_id = 0xffff；
 * @param param 通知或指示参数，见 @ref xf_sle_ssaps_ntf_ind_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssaps_send_notify_indicate(
    uint8_t app_id, uint16_t conn_id,
    xf_sle_ssaps_ntf_ind_t *param);

/**
 * @brief  SLE SSAPS 设置服务端 INFO (连接前)
 * @param app_id 服务端 (应用) ID
 * @param info  服务端 INFO
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssaps_set_info(
    uint8_t app_id, xf_sle_ssap_exchange_info_t *info);

/**
 * @brief SLE SSAPS 事件回调注册
 *
 * @param evt_cb 事件回调，见 @ref xf_sle_ssaps_event_cb_t
 * @param events 事件，见 @ref xf_sle_ssaps_event_t
 * @note 当前仅支持所有事件注册在同一个回调，暂不支持指定事件对应单独的回调，
 * 所以 参数 'events' 填 0 即可
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssaps_event_cb_register(
    xf_sle_ssaps_event_cb_t evt_cb,
    xf_sle_ssaps_event_t events);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_wal_sle_ssap
 * @}
 */

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_SSAP_SERVER_H__ */
