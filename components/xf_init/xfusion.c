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
#include "xf_init.h"

#ifdef CONFIG_XF_OSAL_ENABLE
#include "xf_osal.h"
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void xf_task_on_idle(unsigned long int max_idle_ms);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

extern void xf_main(void);

void xfusion_init(void)
{
    xf_log_set_time_func((xf_log_time_func_t)xf_sys_time_get_ms);

    xf_init();

    xf_main();
}

void xfusion_run(void)
{
    xf_task_manager_run_default();
}

/* ==================== [Static Functions] ================================== */

static void xf_task_on_idle(unsigned long int max_idle_ms)
{
#if CONFIG_XF_OSAL_ENABLE
    xf_osal_delay_ms(max_idle_ms);
#else
    (void)max_idle_ms;
    xf_sys_watchdog_kick();
#endif
}

static xf_err_t default_manager_init(void)
{
    xf_task_tick_init(xf_sys_time_get_ms);
    return xf_task_manager_default_init(xf_task_on_idle);
}

XF_INIT_EXPORT_BOARD(default_manager_init);
