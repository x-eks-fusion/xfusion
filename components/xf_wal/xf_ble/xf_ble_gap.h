/**
 * @file xf_ble_gap.h
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-08-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_BLE_GAP_H__
#define __XF_BLE_GAP_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_ble_gap_types.h"
#include "xf_ble_sm_types.h"

#if XF_BLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief BLE 功能开启
 *
 * @note 包含所有 BLE 开启前的处理
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_enable(void);

/**
 * @brief BLE 功能关闭
 *
 * @note 包含所有 BLE 关闭前的处理
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_disable(void);

/**
 * @brief BLE GAP 设置本端设备地址
 *
 * @param addr BLE 地址值
 * @param type BLE 地址类型，见 @ref xf_ble_addr_type_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - XF_ERR_NOT_SUPPORTED  不支持
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_set_local_addr(
    uint8_t addr[XF_BLE_ADDR_LEN],
    xf_ble_addr_type_t type);

/**
 * @brief BLE GAP 获取本端设备地址
 *
 * @param[out] addr BLE 地址信息，见 @ref xf_ble_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_get_local_addr(xf_ble_addr_t *addr);

/**
 * @brief BLE GAP 设置本端设备的外观
 *
 * @param appearance 外观值，见 @ref xf_ble_appearance_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_set_local_appearance(xf_ble_appearance_t appearance);

/**
 * @brief BLE GAP 获取本端设备的外观
 *
 * @return xf_ble_appearance_t，见 @ref xf_ble_appearance_t
 */
xf_ble_appearance_t xf_ble_gap_get_local_appearance(void);

/**
 * @brief BLE GAP 设置本端设备名称
 *
 * @param name 设备名
 * @param len 设备名长度
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_set_local_name(const uint8_t *name, const uint8_t len);

/**
 * @brief BLE GAP 获取本端设备名称
 *
 * @param[out] name 设备名
 * @param len 设备名长度
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_get_local_name(uint8_t *name, uint8_t *len);

/**
 * @brief BLE GAP 设置广播数据
 *
 * @param data 广播数据，见 @ref xf_ble_gap_adv_data_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_set_adv_data(const xf_ble_gap_adv_data_t *data);

/**
 * @brief BLE GAP 设置广播参数
 *
 * @param param 广播参数，见 @ref xf_ble_gap_adv_param_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_set_adv_param(const xf_ble_gap_adv_param_t *param);

/**
 * @brief BLE GAP 广播开启
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_start_adv(void);

/**
 * @brief BLE GAP 广播关闭
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_stop_adv(void);

/**
 * @brief BLE GAP 设置扫描参数
 *
 * @param param 扫描参数，见 @ref xf_ble_gap_scan_param_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_set_scan_param(const xf_ble_gap_scan_param_t *param);

/**
 * @brief BLE GAP 扫描开启
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_start_scan(void);

/**
 * @brief BLE GAP 扫描停止
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_stop_scan(void);

/**
 * @brief BLE GAP 更新连接参数
 *
 * @param params 更新连接参数的信息，见 @ref xf_ble_gap_conn_param_update_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_update_conn_params(xf_ble_gap_conn_param_update_t *params);

/**
 * @brief BLE GAP 发起连接
 *
 * @param addr 要连接的地址，见 @ref xf_ble_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_connect(const xf_ble_addr_t *addr);

/**
 * @brief BLE GAP 断开连接
 *
 * @param addr 要断连的地址，见 @ref xf_ble_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_disconnect(const xf_ble_addr_t *addr);

/**
 * @brief BLE GAP 添加配对
 *
 * @param addr 要配对的设备的地址，见 @ref xf_ble_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_add_pair(const xf_ble_addr_t *addr);

/**
 * @brief BLE GAP 删除配对
 *
 * @param addr 要删除配对的设备的地址，见 @ref xf_ble_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_del_pair(const xf_ble_addr_t *addr);

/**
 * @brief BLE GAP 删除所有配对
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_del_pair_all(void);

/**
 * @brief BLE GAP 获取已配对的设备
 *
 * @param max_num 要获取的最大数量
 * @param[out] dev_list 获取到的设备列表，见 @ref xf_ble_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_get_pair_list(
    uint16_t *max_num, xf_ble_addr_t *dev_list);

/**
 * @brief BLE GAP 获取已绑定的设备
 *
 * @param max_num 要获取的最大数量
 * @param[out] dev_list 获取到的设备列表，见 @ref xf_ble_addr_t
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_get_bond_list(
    int *max_num, xf_ble_addr_t *dev_list);

/**
 * @brief BLE GAP 设置安全参数
 *
 * @warning 当前该接口有部分问题未确定，禁止调用
 * @param param_type 安全参数类型，见 @ref xf_ble_sm_param_type_t
 * @param value 安全参数的数据值
 * @param len  安全参数的数据长度
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - (OTHER)               @ref xf_err_t
 */
xf_err_t xf_ble_gap_set_security_param(
    xf_ble_sm_param_type_t param_type, void *value, uint8_t len);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GAP_H__ */
