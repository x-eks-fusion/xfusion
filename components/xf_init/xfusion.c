/**
 * @file xfusion.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief xfusion_run() 实现。
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xfusion.h"
#include "xf_task.h"
#include "xf_log.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

#if XF_TASK_MBUS_IS_ENABLE
/**
 * @brief 发布订阅默认处理函数。
 * 
 * @param task 当前任务句柄。
 */
static void mbus_handle(xf_task_t task);
#endif

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xfusion_run(const xf_init_preinit_ops_t *const p_ops)
{
    xf_init(p_ops);
#if XF_TASK_MBUS_IS_ENABLE
    xf_ntask_create_loop(mbus_handle, NULL, 0, 10);
#endif
    xf_main();
    xf_task_manager_run_default();
}

/* ==================== [Static Functions] ================================== */

#if XF_TASK_MBUS_IS_ENABLE

static void mbus_handle(xf_task_t task)
{
    (void)task;
    xf_task_mbus_handle();
}

#endif

