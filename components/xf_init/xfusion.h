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
 * @brief 运行 xfusion。
 * 
 * 含启动自动初始化、创建发布订阅服务任务、运行 `xf_main()` 等。
 * 
 * @param p_ops xf_init 预初始化函数操作集。见 @ref xf_init_preinit_ops_t.
 */
void xfusion_run(const xf_init_preinit_ops_t *const p_ops);

/**
 * End of group_xf_init_port
 * @}
 */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XFUSION_H__
