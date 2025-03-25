/**
 * @file xf_lp_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2025-02-14
 *
 * @copyright Copyright (c) 2025, CorAL. All rights reserved.
 *
 */

#ifndef __XF_LP_CONFIG_H__
#define __XF_LP_CONFIG_H__

/* ==================== [Includes] ========================================== */
#include "xfconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_LP_ENABLE CONFIG_XF_LP_ENABLE
#define XF_LP_PRIORITY_MAX CONFIG_XF_LP_PRIORITY_MAX
#define XF_LP_CPU_MODE_NONE CONFIG_XF_LP_CPU_MODE_NONE
#define XF_LP_CPU_MODE_SWITCH_CPU CONFIG_XF_LP_CPU_MODE_SWITCH_CPU
#define XF_LP_CPU_MODE_REDUCE_FREQ CONFIG_XF_LP_CPU_MODE_REDUCE_FREQ

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_LP_CONFIG_H__

