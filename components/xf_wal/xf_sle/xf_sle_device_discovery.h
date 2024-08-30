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

#if XF_SLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

xf_err_t xf_sle_enable(void);

xf_err_t xf_sle_disable(void);

// 设置本地设备地址v
xf_err_t xf_sle_set_local_addr(xf_sle_addr_t *addr);

// 获取本地设备地址
xf_err_t xf_sle_get_local_addr(xf_sle_addr_t *addr);

// 设置本地设备名称
xf_err_t xf_sle_set_local_name(const uint8_t *name, const uint8_t len);

// 获取本地设备名称
xf_err_t xf_sle_get_local_name(uint8_t *name, uint8_t *len);

// 设置设备公开（广播）数据
xf_err_t xf_sle_set_announce_data(
    uint8_t announce_id, const xf_sle_announce_data_t *data);

// 设置设备公开（广播）参数
xf_err_t xf_sle_set_announce_param(
    uint8_t announce_id, const xf_sle_announce_param_t *param);

// 开始设备公开（广播）
xf_err_t xf_sle_start_announce(uint8_t announce_id);

// 停止设备公开（广播）
xf_err_t xf_sle_stop_announce(uint8_t announce_id);

// // 删除广播
// errcode_t sle_remove_announce(uint8_t announce_id);

// 设置设备公开扫描参数
xf_err_t xf_sle_set_seek_param(xf_sle_seek_param_t *param);

//  开始设备公开扫描
xf_err_t xf_sle_start_seek(void);

// 停止设备公开扫描
xf_err_t xf_sle_stop_seek(void);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_DEVICE_DISCOVERY_H__ */
