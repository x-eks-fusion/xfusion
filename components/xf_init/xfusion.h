/**
 * @file xfusion.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief 运行 xfusion。
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XFUSION_H__
#define __XFUSION_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_init/src/xf_init.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @ingroup group_xf_init_port
 * @{
 */

/**
 * @brief 初始化 xfusion，包含log初始化，自动初始化等。
 *
 */
void xfusion_init(void);

/**
 * @brief 运行 xfusion。需要将该函数放到循环里面调用。
 *
 */
void xfusion_run(void);

/**
 * End of group_xf_init_port
 * @}
 */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XFUSION_H__
