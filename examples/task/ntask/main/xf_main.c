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
 * @example examples/task/ntask/main/xf_main.c
 * xf_task 无栈协程示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define TAG "example_ntask"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void xf_main_task(xf_task_t task);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_ntask_create(xf_main_task, (void *)1, 1);
}

/* ==================== [Static Functions] ================================== */

xf_async_t test(xf_task_t task, int a)
{
    XF_NTASK_BEGIN(task);

    XF_LOGI(TAG, "task:%d", a);
    xf_ntask_delay(1000);
    a = xf_ntask_args_get_int(task, "a");
    XF_LOGI(TAG, "task:%d", a);

    XF_NTASK_END();
}

static void xf_main_task(xf_task_t task)
{
    XF_NTASK_BEGIN(task);

    int a = 2;
    xf_ntask_args_set_int(task, "a", a);
    while (1) {
        xf_await(test(task, 1));
    }

    XF_NTASK_END();
}
