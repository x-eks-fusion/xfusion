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
 * @brief SLE SSAPC 客户端注册
 *
 * @param[in] app_uuid 要注册的客户端 (应用) 的 UUID ，见 @ref xf_sle_uuid_info_t
 * @param[out] app_id 客户端 (应用) ID
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssapc_app_register(
    xf_sle_uuid_info_t *app_uuid, uint8_t *app_id);

/**
 * @brief SLE SSAPC 客户端注销
 *
 * @param app_id 客户端 ID
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssapc_app_unregister(uint8_t app_id);

/**
 * @brief SLE SSAPC 搜寻服务
 *
 * @param app_id 客户端 ID (应用 ID )
 * @param conn_id 连接 ID (链接 ID )
 * @param param 搜寻参数，见 @ref xf_sle_ssapc_find_struct_param_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssapc_discover_service(
    uint8_t app_id, uint16_t conn_id,
    xf_sle_ssapc_find_struct_param_t *param);

/**
 * @brief SLE SSAPC 搜寻属性 (property)
 *
 * @param app_id 客户端 ID (应用 ID )
 * @param conn_id 连接 ID (链接 ID )
 * @param param 搜寻参数，见 @ref xf_sle_ssapc_find_struct_param_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssapc_discovery_property(
    uint8_t app_id, uint16_t conn_id,
    xf_sle_ssapc_find_struct_param_t *param);

/**
 * @brief SLE SSAPC 通过句柄发起读请求
 *
 * @param app_id 客户端 ID (应用 ID)
 * @param conn_id 连接 ID (链接 ID )
 * @param type 类型
 * @param handle 句柄
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssapc_request_read_by_handle(
    uint8_t app_id, uint16_t conn_id,
    uint8_t type, uint16_t handle);

/**
 * @brief SLE SSAPC 通过 UUID 发起读请求
 *
 * @param app_id 客户端 ID (应用 ID)
 * @param conn_id 连接 ID (链接 ID )
 * @param type 类型
 * @param start_handle 起始句柄
 * @param end_handle 结束句柄
 * @param uuid 指定的 UUID，见 @ref xf_sle_uuid_info_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssapc_request_read_by_uuid(
    uint8_t app_id, uint16_t conn_id,
    uint8_t type,
    uint16_t start_handle,
    uint16_t end_handle,
    const xf_sle_uuid_info_t *uuid);

/**
 * @brief SLE SSAPC 发起写请求
 *
 * @param app_id 客户端 ID (应用 ID)
 * @param conn_id 连接 ID (链接 ID )
 * @param type 类型
 * @param handle 句柄
 * @param data 要写的数据
 * @param data_len 要写的数据的长度
 * @note 需要 (对端) 回应 (写请求)
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssapc_request_write_data(
    uint8_t app_id, uint16_t conn_id,
    uint16_t handle, uint8_t type,
    uint8_t *data, uint16_t data_len);

/**
 * @brief SLE SSAPC 发起写命令
 *
 * @param app_id 客户端 ID (应用 ID)
 * @param conn_id 连接 ID (链接 ID )
 * @param handle 句柄
 * @param type 类型
 * @param data 要写的数据
 * @param data_len 要写的数据的长度
 * @note 无需 (对端) 响应 (写命令)
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssapc_request_write_cmd(
    uint8_t app_id, uint16_t conn_id,
    uint16_t handle, uint8_t type,
    uint8_t *data, uint16_t data_len);

/**
 * @brief SLE SSAPC 发送协商 (MTU、Version) 请求
 *
 * @param app_id 客户端 ID (应用 ID)
 * @param conn_id 连接 ID (链接 ID )
 * @param param 协商信息，见 @ref xf_sle_ssap_exchange_info_t
 * @return xf_err_t
 */
xf_err_t xf_sle_ssapc_request_exchange_info(
    uint8_t app_id, uint16_t conn_id,
    xf_sle_ssap_exchange_info_t *param);

/**
 * @brief SLE SSAPC 事件回调注册
 *
 * @param evt_cb 事件回调，见 @ref xf_sle_ssapc_event_cb_t
 * @param events 事件，见 @ref xf_sle_ssapc_event_t
 * @note 当前仅支持所有事件注册在同一个回调，暂不支持指定事件对应单独的回调，
 * 所以 参数 'events' 填 0 即可
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_ssapc_event_cb_register(
    xf_sle_ssapc_event_cb_t evt_cb,
    xf_sle_ssapc_event_t events);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_wal_sle_ssap
 * @}
 */

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_SSAP_CLIENT_H__ */
