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
 * @example examples/task/ttask/main/xf_main.c
 * xf_task 定时器任务示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define TAG "example_ttask"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void task(xf_task_t task);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_ttask_create_loop(task, (void *)1, 1, 10);
    xf_ttask_create(task, (void *)2, 1, 10, 1);
}

/* ==================== [Static Functions] ================================== */

static void task(xf_task_t task)
{
    XF_LOGI(TAG, "hello ttask");
}

