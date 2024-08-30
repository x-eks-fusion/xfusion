/**
 * @file xf_init_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief 使用 xfusion 菜单配置 xf_init 内部配置。
 * @version 0.1
 * @date 2024-07-01
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_INIT_CONFIG_H__
#define __XF_INIT_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/**
 * @brief 定义自动初始化的实现方式.
 */
#if defined(CONFIG_XF_INIT_IMPL_BY_SECTION_ENABLE)
#   define XF_INIT_IMPL_METHOD              XF_INIT_IMPL_BY_SECTION
#elif defined(CONFIG_XF_INIT_IMPL_BY_CONSTRUCTOR_ENABLE)
#   define XF_INIT_IMPL_METHOD              XF_INIT_IMPL_BY_CONSTRUCTOR
#elif defined(CONFIG_XF_INIT_IMPL_BY_REGISTRY_ENABLE)
#   define XF_INIT_IMPL_METHOD              XF_INIT_IMPL_BY_REGISTRY
#endif

#if defined(CONFIG_XF_INIT_USER_REGISTRY_PATH)
/**
 * @brief 如果需要使用用户注册表, 请用绝对路径定义该宏.
 * 如需使用相对路径,则是相对与 `src/xf_init_registry.inc` 的路径.
 * 否则不要定义该宏.
 */
#   define XF_INIT_USER_REGISTRY_PATH       CONFIG_XF_INIT_USER_REGISTRY_PATH
#endif

/**
 * @brief 配置 init 相关的 log 等级。
 */
#ifndef XF_LOG_LEVEL
#   define XF_LOG_LEVEL                     CONFIG_XF_INIT_LOG_LEVEL
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_INIT_CONFIG_H__
