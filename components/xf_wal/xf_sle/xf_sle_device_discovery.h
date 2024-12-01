/**
 * @file xf_sle_device_discovery.h
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-08-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_SLE_DEVICE_DISCOVERY_H__
#define __XF_SLE_DEVICE_DISCOVERY_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sle_types.h"
#include "xf_sle_device_discovery_types.h"

#if XF_SLE_IS_ENABLE || defined(__DOXYGEN__)

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_wal_sle
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
 * @brief SLE 功能开启
 *
 * @note 包含所有 SLE 开启前的处理
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_enable(void);

/**
 * @brief SLE 功能关闭
 *
 * @note 包含所有 SLE 关闭前的处理
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_disable(void);

/**
 * @brief SLE 设置本端设备地址
 *
 * @param addr SLE 地址信息，见 @ref xf_sle_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - XF_ERR_NOT_SUPPORTED  不支持
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_set_local_addr(xf_sle_addr_t *addr);

/**
 * @brief SLE 获取本端设备地址
 *
 * @param addr SLE 地址信息，见 @ref xf_sle_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_get_local_addr(xf_sle_addr_t *addr);

/**
 * @brief SLE 设置本端设备名称
 *
 * @param name 设备名
 * @param len 设备名长度
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_set_local_name(const uint8_t *name, const uint8_t len);

/**
 * @brief SLE 获取本端设备名称
 *
 * @param[out] name 设备名
 * @param len 设备名长度
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_get_local_name(uint8_t *name, uint8_t *len);

/**
 * @brief SLE 设置广播 (公开) 数据
 *
 * @param announce_id 广播 ID
 * @param data 广播数据，见 @ref xf_sle_announce_data_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_set_announce_data(
    uint8_t announce_id, const xf_sle_announce_data_t *data);

/**
 * @brief SLE 设置广播 (公开) 参数
 *
 * @param announce_id 广播 ID
 * @param param 广播参数，见 @ref xf_sle_announce_param_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_set_announce_param(
    uint8_t announce_id, const xf_sle_announce_param_t *param);

/**
 * @brief SLE 广播开启
 *
 * @param announce_id 广播 ID
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_start_announce(uint8_t announce_id);

/**
 * @brief SLE 广播关闭
 *
 * @param announce_id 广播 ID
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_stop_announce(uint8_t announce_id);

/**
 * @brief SLE 设置扫描参数
 *
 * @param param 扫描参数，见 @ref xf_sle_seek_param_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_set_seek_param(xf_sle_seek_param_t *param);

/**
 * @brief SLE 扫描开启
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_start_seek(void);

/**
 * @brief SLE 扫描停止
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_stop_seek(void);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_wal_sle
 * @}
 */

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_DEVICE_DISCOVERY_H__ */
