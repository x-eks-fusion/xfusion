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

#if XF_SLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

// 更新连接参数
xf_err_t xf_sle_update_conn_params(
    xf_sle_conn_param_update_t *params);
// 与指定地址设备进行连接
xf_err_t xf_sle_connect(const xf_sle_addr_t *addr);
// 与指定地址设备断开连接
xf_err_t xf_sle_disconnect(const xf_sle_addr_t *addr);
// 与所有设备断开连接
xf_err_t xf_sle_disconnect_all(void);
// 与指定地址设备进行配对
xf_err_t xf_sle_add_pair(const xf_sle_addr_t *addr);
// 与指定地址设备取消配对
xf_err_t xf_sle_del_pair(const xf_sle_addr_t *addr);
// 与所有设备取消配对
xf_err_t xf_sle_del_pair_all(void);
// 获取已配对的设备
xf_err_t xf_sle_get_pair_list(
    uint16_t *max_num, xf_sle_addr_t *dev_list);
// 获取已绑定的设备
xf_err_t xf_sle_get_bond_list(
    int *max_num, xf_sle_addr_t *dev_list);

// 获取对端设备 RSSI
xf_err_t xf_sle_get_peer_rssi(uint16_t conn_id, int8_t *rssi);

// 设置 PHY 参数
xf_err_t xf_sle_set_phy_params(uint16_t conn_id, xf_sle_set_phy_t *sle_phy);

// 设置 调制与编码策略（  Modulation and Coding Scheme ）
xf_err_t xf_sle_set_mcs(uint16_t conn_id, uint8_t mcs);

// 设置连接链路上所偏好的最大传输 payload 字节数
xf_err_t xf_sle_set_data_len(uint16_t conn_id, uint16_t tx_octets);

// 设置 sle 默认连接参数
xf_err_t xf_sle_set_default_conn_params(
    xf_sle_conn_param_def_t *conn_param_def);

// 配置定制化信息
xf_err_t xf_sle_set_max_pwr(int8_t ble_pwr, int8_t sle_pwr);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_XF_SLE_CONNECTION_MANAGER_H__ */
