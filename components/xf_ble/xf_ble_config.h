/**
 * @file xf_ble_config.h
 * @brief 
 * @author dotc (dotchan@qq.com)
 * @date 2025-02-24
 * 
 * @Copyright (c) 2025, CorAL. All rights reserved.
 */

#ifndef __XF_BLE_CONFIG_H__
#define __XF_BLE_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#if defined(CONFIG_XF_BLE_ENABLE) || CONFIG_XF_BLE_ENABLE
#define XF_BLE_ENABLE (1)
#else
#define XF_BLE_ENABLE (0)
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_BLE_CONFIG_H__
