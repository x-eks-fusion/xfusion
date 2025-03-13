/**
 * @file xf_ota_config_internal.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief xf_ota 模块内部配置总头文件。
 *        确保 xf_ota_config.h 的所有定义都有默认值。
 * @version 1.0
 * @date 2024-12-19
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_OTA_CONFIG_INTERNAL_H__
#define __XF_OTA_CONFIG_INTERNAL_H__

/* ==================== [Includes] ========================================== */

#include "xf_ota_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#if (!defined(XF_OTA_ENABLE) || (XF_OTA_ENABLE) || defined(__DOXYGEN__))
#   define XF_OTA_IS_ENABLE                     1
#else
#   define XF_OTA_IS_ENABLE                     0
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_OTA_CONFIG_INTERNAL_H__
