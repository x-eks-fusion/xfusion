/**
 * @file xf_sle_config.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @date 2025-03-12
 * 
 * @Copyright (c) 2025, CorAL. All rights reserved.
 */

#ifndef __XF_SLE_CONFIG_H__
#define __XF_SLE_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#if defined(CONFIG_XF_SLE_ENABLE) || CONFIG_XF_SLE_ENABLE
#define XF_SLE_ENABLE (1)
#else
#define XF_SLE_ENABLE (0)
#endif

#if defined(CONFIG_XF_SLE_ADV_ID_INVALID) || (CONFIG_XF_SLE_ADV_ID_INVALID)
#define XF_SLE_ADV_ID_INVALID   (CONFIG_XF_SLE_ADV_ID_INVALID)
#endif

#if defined(CONFIG_XF_SLE_CONN_ID_INVALID) || (CONFIG_XF_SLE_CONN_ID_INVALID)
#define XF_SLE_CONN_ID_INVALID   (CONFIG_XF_SLE_CONN_ID_INVALID)
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_SLE_CONFIG_H__
