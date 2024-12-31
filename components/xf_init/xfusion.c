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
#include "xf_sys.h"

#if XF_OSAL_ENABLE
#include "xf_osal.h"
#endif


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

static void xf_task_on_idle(unsigned long int max_idle_ms);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

extern void xf_main(void);

void xfusion_init(void)
{
    xf_log_set_time_func((xf_log_time_func_t)xf_sys_time_get_ms);

    xf_init();

    xf_task_tick_init(xf_sys_time_get_ms);
    xf_task_manager_default_init(xf_task_on_idle);

#if XF_TASK_MBUS_IS_ENABLE
    xf_ttask_create_loop(mbus_handle, NULL, 0, 10);
#endif

    xf_main();
}

void xfusion_run(void)
{
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

static void xf_task_on_idle(unsigned long int max_idle_ms)
{
#if XF_OSAL_ENABLE
    xf_osal_delay_ms(max_idle_ms);
#endif
}
