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
 * xf_task 定时任务示例。
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
    xf_ntask_create_loop(xf_main_task, NULL, 1, 1000);
    XF_LOGI(TAG, "create ntask");
}

/* ==================== [Static Functions] ================================== */

static void xf_main_task(xf_task_t task)
{
    XF_LOGI(TAG, "hello world");
}
