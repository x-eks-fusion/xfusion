/**
 * @file xf_task_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief 使用 xfusion 菜单配置 xf_task 内部配置。
 * @version 0.1
 * @date 2024-07-09
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_TASK_CONFIG_H__
#define __XF_TASK_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_TASK_PRIORITY_LEVELS     CONFIG_XF_TASK_PRIORITY_LEVELS

#define XF_TASK_HUNGRY_ENABLE       CONFIG_XF_TASK_HUNGRY_ENABLE

#define XF_TASK_CONTEXT_ENABLE      CONFIG_XF_TASK_CONTEXT_ENABLE

#ifndef XF_LOG_LEVEL
#define XF_LOG_LEVEL                CONFIG_XF_TASK_LOG_LEVEL
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_TASK_CONFIG_H__
