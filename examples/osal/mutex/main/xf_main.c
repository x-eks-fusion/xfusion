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
 * @example{lineno} examples/osal/mutex/main/xf_main.c
 * xf_osal 互斥锁示例。
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

static xf_osal_mutex_t mutex = NULL;
static int shared_resource = 0;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_osal_mutex_attr_t mutex_attr = {
        .name = "mutex",
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

    mutex = xf_osal_mutex_create(&mutex_attr);
    if (mutex == NULL) {
        XF_LOGE(TAG, "xf mutex create error");
        return;
    }

    xf_osal_mutex_t mutex2 = xf_osal_mutex_create(&mutex_attr);
    if (mutex2 == NULL) {
        XF_LOGE(TAG, "xf mutex2 create error");
        return;
    }

    xf_err_t err = xf_osal_mutex_delete(mutex2);
    if (err != XF_OK) {
        XF_LOGE(TAG, "xf mutex2 delete error");
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
}

/* ==================== [Static Functions] ================================== */

static void task1(void *argument)
{
    XF_LOGI(TAG, "%s:%p", __func__, xf_osal_thread_get_current());
    while (1) {
        if (xf_osal_mutex_acquire(mutex, 100) == XF_OK) {
            XF_LOGI(TAG, "Task1: Accessing shared resource...");
            shared_resource++;
            XF_LOGI(TAG, "Task1: Shared resource value: %d\n", shared_resource);
            xf_osal_mutex_release(mutex);
            xf_osal_delay_ms(800);
        } else {
            XF_LOGI(TAG, "Task1: Acquire mutex timeout");
            xf_osal_thread_t thread = xf_osal_mutex_get_owner(mutex);
            XF_LOGI(TAG, "Task1: Mutex owner: %p", thread);
        }
    }
}

static void task2(void *argument)
{
    XF_LOGI(TAG, "%s:%p", __func__, xf_osal_thread_get_current());
    while (1) {
        if (xf_osal_mutex_acquire(mutex, XF_OSAL_WAIT_FOREVER) == XF_OK) {
            XF_LOGI(TAG, "Task2: Accessing shared resource...");
            shared_resource++;
            XF_LOGI(TAG, "Task1: Shared resource value: %d\n", shared_resource);
            xf_osal_delay_ms(1000);
            xf_osal_mutex_release(mutex);
        }
    }
}
