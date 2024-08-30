/**
 * @file xf_heap_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief 使用 xfusion 菜单配置 xf_heap 内部配置。
 * @version 0.1
 * @date 2024-07-09
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HEAP_CONFIG_H__
#define __XF_HEAP_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_HEAP_BYTE_ALIGNMENT CONFIG_XF_HEAP_BYTE_ALIGNMENT

#define XF_HEAP_STATIC_SIZE CONFIG_XF_HEAP_STATIC_SIZE

#if CONFIG_XF_STDINT_ENABLE
#   include <stdint.h>
#else
#   define XF_HEAP_INTPTR_TYPE int /*!< 针对不同的单片机，配置不同的指针整数类型 */
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_HEAP_CONFIG_H__
