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

// 使能 BLE 
xf_err_t xf_ble_enable(void);
// 失能 BLE
xf_err_t xf_ble_disable(void);

// 设置本地设备地址
// xf_err_t xf_ble_gap_set_local_addr(xf_bt_dev_addr_t addr_info);
xf_err_t xf_ble_gap_set_local_addr(
    uint8_t addr[XF_BT_DEV_ADDR_LEN],
    xf_bt_dev_addr_type_t type );
// 获取本地设备地址
xf_err_t xf_ble_gap_get_local_addr(
    xf_bt_dev_addr_t *addr);

// 设置本地设备的外观
/**
 * @param[in]       appearance   - External appearance value, these values are defined by the Bluetooth SIG, please refer to
 *                  https://www.bluetooth.com/specifications/assigned-numbers/
 */
xf_err_t xf_ble_gap_set_local_appearance(xf_ble_appearance_t appearance);

// ？获取本地设备的外观
xf_ble_appearance_t xf_ble_gap_get_local_appearance(void);

// 设置本地设备名称 （ IDF 只有 name 参数，然后内部 strlen 获取长度）
xf_err_t xf_ble_gap_set_local_name(const uint8_t *name, const uint8_t len);

// 获取本地设备名称
xf_err_t xf_ble_gap_get_local_name(uint8_t *name, uint8_t *len);


/* 广播 */
// 设置广播数据 WS63 有个 adv_id 用于多广播？
xf_err_t xf_ble_gap_set_adv_data(const xf_ble_gap_adv_data_t *data);

// 设置广播参数
xf_err_t xf_ble_gap_set_adv_param(const xf_ble_gap_adv_param_t *param);

// 开始广播 (WS63 会有 adv_id)
xf_err_t xf_ble_gap_start_adv(void);
// 停止广播 (WS63 会有 adv_id)
xf_err_t xf_ble_gap_stop_adv(void);

/* 扫描 */
// 设置扫描参数
xf_err_t xf_ble_gap_set_scan_param(const xf_ble_gap_scan_param_t *param);

// 开始扫描   IDF 会把 duration 提出来放在这作为参数传入
xf_err_t xf_ble_gap_start_scan(void);

// 停止扫描
xf_err_t xf_ble_gap_stop_scan(void);

/* 连接 */
// 更新连接参数
xf_err_t xf_ble_gap_update_conn_params(xf_ble_gap_conn_param_update_t *params);
// ？获取连接参数

// 与设备建立（ACL）连接  IDF: 没有 直接嵌在 gattc_open之中 FRQ：有，且需传入上面的各项连接参数
xf_err_t xf_ble_gap_connect(const xf_bt_dev_addr_t *addr);

// 断开设备连接，包括所有profile连接
xf_err_t xf_ble_gap_disconnect(const xf_bt_dev_addr_t *addr);

/* 配对 */
// 发起/启动配对
xf_err_t xf_ble_gap_add_pair(const xf_bt_dev_addr_t *addr);

// 取消指定设备的配对
xf_err_t xf_ble_gap_del_pair(const xf_bt_dev_addr_t *addr);

// 取消所有设备的配对
xf_err_t xf_ble_gap_del_pair_all(void);

// 获取已配对的设备
xf_err_t xf_ble_gap_get_pair_list(
    uint16_t *max_num, xf_bt_dev_addr_t *dev_list);

// 获取已绑定的设备
xf_err_t xf_ble_gap_get_bond_list(
    int *max_num, xf_bt_dev_addr_t *dev_list);

// 设置安全参数
xf_err_t xf_ble_gap_set_security_param(
    xf_ble_sm_param_type_t param_type, void *value, uint8_t len);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GAP_H__ */
