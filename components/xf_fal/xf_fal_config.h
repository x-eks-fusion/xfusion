/**
 * @file xf_fal_config.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief 使用 xfusion 菜单配置 xf_heap 内部配置。
 * @version 1.0
 * @date 2024-12-10
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_FAL_CONFIG_H__
#define __XF_FAL_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_FAL_ENABLE CONFIG_XF_FAL_ENABLE
#define XF_FAL_LOCK_DISABLE CONFIG_XF_FAL_LOCK_DISABLE
#define XF_FAL_FLASH_DEVICE_NUM CONFIG_XF_FAL_FLASH_DEVICE_NUM
#define XF_FAL_PARTITION_TABLE_NUM CONFIG_XF_FAL_PARTITION_TABLE_NUM
#define XF_FAL_DEV_NAME_MAX CONFIG_XF_FAL_DEV_NAME_MAX
#define XF_FAL_CACHE_NUM CONFIG_XF_FAL_CACHE_NUM
#define XF_FAL_DEFAULT_FLASH_DEVICE_NAME CONFIG_XF_FAL_DEFAULT_FLASH_DEVICE_NAME
#define XF_FAL_DEFAULT_PARTITION_NAME CONFIG_XF_FAL_DEFAULT_PARTITION_NAME
#define XF_FAL_DEFAULT_PARTITION_OFFSET CONFIG_XF_FAL_DEFAULT_PARTITION_OFFSET
#define XF_FAL_DEFAULT_PARTITION_LENGTH CONFIG_XF_FAL_DEFAULT_PARTITION_LENGTH

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_FAL_CONFIG_H__
