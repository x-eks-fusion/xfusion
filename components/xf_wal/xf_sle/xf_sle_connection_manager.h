/**
 * @file xf_sle_connection_manager.h
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-08-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_SLE_XF_SLE_CONNECTION_MANAGER_H__
#define __XF_SLE_XF_SLE_CONNECTION_MANAGER_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sle_types.h"
#include "xf_sle_connection_manager_types.h"

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
 * @brief SLE 更新连接参数
 *
 * @param params 更新连接参数的信息，见 @ref xf_sle_conn_param_update_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_update_conn_params(
    xf_sle_conn_param_update_t *params);

/**
 * @brief SLE 发起连接
 *
 * @param addr 要连接的地址，见 @ref xf_sle_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_connect(const xf_sle_addr_t *addr);

/**
 * @brief SLE 断开连接
 *
 * @param addr 要断连的地址，见 @ref xf_sle_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_disconnect(const xf_sle_addr_t *addr);

/**
 * @brief SLE 断开所有连接
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_disconnect_all(void);

/**
 * @brief SLE 添加配对
 *
 * @param addr 要配对的设备的地址，见 @ref xf_sle_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_add_pair(const xf_sle_addr_t *addr);

/**
 * @brief SLE 删除配对
 *
 * @param addr 要删除配对的设备的地址，见 @ref xf_sle_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_del_pair(const xf_sle_addr_t *addr);

/**
 * @brief SLE 删除所有配对
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_del_pair_all(void);

/**
 * @brief SLE 获取已配对的设备
 *
 * @param max_num 要获取的最大数量
 * @param[out] dev_list 获取到的设备列表，见 @ref xf_sle_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_get_pair_list(
    uint16_t *max_num, xf_sle_addr_t *dev_list);

/**
 * @brief SLE 获取已绑定的设备
 *
 * @param max_num 要获取的最大数量
 * @param[out] dev_list 获取到的设备列表，见 @ref xf_sle_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_get_bond_list(
    int *max_num, xf_sle_addr_t *dev_list);

/**
 * @brief SLE 获取对端设备的 RSSI
 *
 * @param conn_id 链接 (连接) ID
 * @param rssi
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_get_peer_rssi(uint16_t conn_id, int8_t *rssi);

/**
 * @brief SLE 设置 PHY 参数
 *
 * @param conn_id 链接 (连接) ID
 * @param sle_phy SLE PHY 参数， @ref xf_sle_set_phy_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_set_phy_params(uint16_t conn_id, xf_sle_set_phy_t *sle_phy);

/**
 * @brief SLE 设置 调制与编码策略（  Modulation and Coding Scheme ）
 *
 * @param conn_id 链接 (连接) ID
 * @param mcs SLE 调制与编码策略（  Modulation and Coding Scheme ）
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_set_mcs(uint16_t conn_id, uint8_t mcs);

/**
 * @brief SLE 设置连接链路上所偏好的最大传输 payload 字节数
 *
 * @param conn_id 链接 (连接) ID
 * @param tx_octets 偏好的 payload 字节数
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_set_data_len(uint16_t conn_id, uint16_t tx_octets);

//
/**
 * @brief SLE 设置默认连接参数
 *
 * @param conn_param_def SLE 默认连接参数
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_set_default_conn_params(
    xf_sle_conn_param_def_t *conn_param_def);

/**
 * @brief SLEE 配置最大功率档位（根据指定的最大功率）
 *
 * @param target_max_pwr 目标最大功率
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_sle_set_max_pwr_level_by_pwr(int8_t target_max_pwr);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_wal_sle
 * @}
 */

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_XF_SLE_CONNECTION_MANAGER_H__ */
