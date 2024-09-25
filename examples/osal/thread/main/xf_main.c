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

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"
#include "xf_osal.h"

/* ==================== [Defines] =========================================== */

#define TAG "main"

#define XF_OSAL_CHECK(x) if (x != XF_OK)\
{\
    XF_LOGE(TAG, "xf check error: %d", x);\
    xf_osal_thread_delete(NULL);\
    return;\
}

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void task1(void *argument);
static void task2(void *argument);
static void task3(void *argument);

/* ==================== [Static Variables] ================================== */

static xf_osal_thread_t thread[2] = {0};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
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
    xf_osal_thread_attr_t attr3 = {
        .name = "task3",
        .priority = XF_OSAL_PRIORITY_BELOW_NORMAL,
        .stack_size = 1024 * 2,
    };
    xf_osal_thread_t thread1 = xf_osal_thread_create(task1, NULL, &attr1);
    if (thread1 == NULL) {
        XF_LOGE(TAG, "xf thread1 create error");
        return;
    }
    thread[0] = xf_osal_thread_create(task2, NULL, &attr2);
    if (thread[0] == NULL) {
        XF_LOGE(TAG, "xf thread2 create error");
        return;
    }
    thread[1] = xf_osal_thread_create(task3, NULL, &attr3);
    if (thread[1] == NULL) {
        XF_LOGE(TAG, "xf thread2 create error");
        return;
    }
}

/* ==================== [Static Functions] ================================== */

static void task1(void *argument)
{
    (void)argument;
    XF_LOGI(TAG, "task1 running");
    xf_osal_thread_t task = xf_osal_thread_get_current();
    const char *name = xf_osal_thread_get_name(task);
    XF_LOGI(TAG, "thread name: %s", name);
    xf_osal_state_t state = xf_osal_thread_get_state(task);
    XF_LOGI(TAG, "thread state: %d", state);
    uint32_t stack_space = xf_osal_thread_get_stack_space(task);
    XF_LOGI(TAG, "thread stack space: %d", stack_space);
    xf_err_t err = xf_osal_thread_set_priority(task, XF_OSAL_PRIORITY_HIGH);
    if (err != XF_OK) {
        XF_LOGE(TAG, "xf check error: %d", err);
    }
    xf_osal_priority_t priority = xf_osal_thread_get_priority(task);
    XF_LOGI(TAG, "thread priority: %d", priority);
    XF_OSAL_CHECK(xf_osal_delay(1000));
    XF_OSAL_CHECK(xf_osal_thread_resume(thread[0]));
    XF_OSAL_CHECK(xf_osal_thread_yield());
    xf_osal_thread_delete(NULL); // 删除 task1
}

static void task2(void *argument)
{
    (void)argument;
    uint32_t tick_count = 0;
    uint32_t count = 0;
    XF_OSAL_CHECK(xf_osal_thread_suspend(thread[0]));
    XF_LOGI(TAG, "task2 running");
    while (1) {
        tick_count = xf_osal_kernel_get_tick_count();
        count = xf_osal_thread_get_count();
        XF_LOGI(TAG, "task2 count: %ld", count);
        count = xf_osal_thread_get_active_count(thread, 2);
        XF_LOGI(TAG, "task2 active count: %ld", count);
        XF_OSAL_CHECK(xf_osal_delay_until(tick_count + 1000));
    }
}

static void task3(void *argument)
{
    (void)argument;
    XF_LOGI(TAG, "task3 running");
    while (1) {
        XF_LOGI(TAG, "task3");
        xf_osal_delay_ms(1000);
    }
}
