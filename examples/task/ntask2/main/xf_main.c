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
 * @example examples/task/ntask2/main/xf_main.c
 * xf_task 无栈协程示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define TAG "example_ntask2"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void ntask1(xf_task_t task);
static void ntask2(xf_task_t task);

/* ==================== [Static Variables] ================================== */

static xf_ntask_sem_t sem;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_ntask_sem_init(&sem, 0);

    xf_ntask_create_loop(ntask1, (void *)1, 1, 10);
    xf_ntask_create(ntask2, (void *)2, 1, 10, 1);
}

/* ==================== [Static Functions] ================================== */

static void ntask1(xf_task_t task)
{
    XF_NTASK_BEGIN(task);
    XF_LOGI(TAG, "ntask 1 init");
    int *num = (int *)xf_malloc(sizeof(int));
    xf_ntask_set_hook_ptr(task, num);
    while (1) {
        num = xf_ntask_get_hook_ptr(task);
        *num += 1;
        if (*num == 5) {
            xf_ntask_sem_signal(&sem);
        }

        XF_LOGI(TAG, "num:%d", *num);
        xf_ntask_delay(1000);
    }
    XF_NTASK_END();
}

static void ntask2(xf_task_t task)
{
    XF_NTASK_BEGIN(task);
    XF_LOGI(TAG, "ntask 2 init");
    xf_ntask_sem_wait(&sem);
    XF_LOGI(TAG, "ntask 2 init");
    XF_NTASK_END();
}
