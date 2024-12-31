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
 * @example{lineno} examples/osal/semaphore/main/xf_main.c
 * xf_osal 信号量示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"
#include "xf_osal.h"

/* ==================== [Defines] =========================================== */

#define TAG "main"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void task1(void *argument);
static void task2(void *argument);

/* ==================== [Static Variables] ================================== */

static xf_osal_semaphore_t sem = NULL;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_osal_semaphore_attr_t sem_attr = {
        .name = "sem",
    };
    xf_osal_thread_attr_t attr1 = {
        .name = "task1",
        .priority = XF_OSAL_PRIORITY_NORMOL,
        .stack_size = 1024 * 2,
    };
    xf_osal_thread_attr_t attr2 = {
        .name = "task2",
        .priority = XF_OSAL_PRIORITY_ABOVE_NORMAL,
        .stack_size = 1024 * 2,
    };

    sem = xf_osal_semaphore_create(1, 0, &sem_attr);
    if (sem == NULL) {
        XF_LOGE(TAG, "xf sem create error");
        return;
    }

    xf_osal_semaphore_t sem2 = xf_osal_semaphore_create(1, 0, &sem_attr);
    if (sem2 == NULL) {
        XF_LOGE(TAG, "xf sem2 create error");
        return;
    }

    xf_err_t err = xf_osal_semaphore_delete(sem2);
    if (err != XF_OK) {
        XF_LOGE(TAG, "xf sem2 delete error");
        return;
    }

    xf_osal_thread_t thread1 = xf_osal_thread_create(task1, NULL, &attr1);
    if (thread1 == NULL) {
        XF_LOGE(TAG, "xf thread1 create error");
        return;
    }

    xf_osal_thread_t thread2 = xf_osal_thread_create(task2, NULL, &attr2);
    if (thread2 == NULL) {
        XF_LOGE(TAG, "xf thread2 create error");
        return;
    }
    uint32_t count = xf_osal_semaphore_get_count(sem);
    XF_LOGI(TAG, "count: %d", count);
}

/* ==================== [Static Functions] ================================== */

static void task1(void *argument)
{
    while (1) {
        XF_LOGI(TAG, "task1 running");
        xf_osal_semaphore_release(sem);
        xf_osal_delay_ms(1000);
    }
}

static void task2(void *argument)
{
    while (1) {
        if (xf_osal_semaphore_acquire(sem, 1000) == XF_OK) {
            XF_LOGI(TAG, "task2 running");
            xf_osal_delay_ms(1000);
        } else {
            XF_LOGI(TAG, "task2 timeout");
        }
    }
}
