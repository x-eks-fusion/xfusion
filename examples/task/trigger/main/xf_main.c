/**
 * @file xf_main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-23
 *
 * Copyright (c) 2023, CorAL. All rights reserved.
 *
 */

/**
 * @example examples/task/trigger/main/xf_main.c
 * xf_task 外部触发任务示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define TAG "trigger"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void ntask(xf_task_t task);
static void ntask_trigger(xf_task_t task);

/* ==================== [Static Variables] ================================== */

static xf_task_t *task1;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    // 创建一个任务用于触发task1
    xf_ttask_create_loop(ntask, (void *)1, 1, 2000);
    // 当延时为0的时候，只能通过外部信号触发
    task1 = xf_ttask_create_loop(ntask_trigger, (void *)2, 1, 0);
}

/* ==================== [Static Functions] ================================== */

static void ntask(xf_task_t task)
{
    intptr_t num = (intptr_t) xf_task_get_arg(task);
    xf_task_trigger(task1);
    XF_LOGI(TAG, "task:%d", num);
}

static void ntask_trigger(xf_task_t task)
{
    intptr_t num = (intptr_t) xf_task_get_arg(task);
    XF_LOGI(TAG, "task:%d", num);
}
