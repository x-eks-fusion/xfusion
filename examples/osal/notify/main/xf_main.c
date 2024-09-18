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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* ==================== [Defines] =========================================== */

#define TAG "main"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void task1(void *argument);
static void task2(void *argument);

/* ==================== [Static Variables] ================================== */

static xf_osal_thread_t thread1 = NULL;
static xf_osal_thread_t thread2 = NULL;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_osal_thread_attr_t attr1 = {
        .name = "task1",
        .priority = XF_OSAL_PRIORITY_NORMOL,
        .stack_size = 1024 * 3,
    };
    xf_osal_thread_attr_t attr2 = {
        .name = "task2",
        .priority = XF_OSAL_PRIORITY_LOW,
        .stack_size = 1024 * 3,
    };

    thread1 = xf_osal_thread_create(task1, NULL, &attr1);
    if (thread1 == NULL) {
        XF_LOGE(TAG, "xf check error: %d", thread1);
        return;
    }

    thread2 = xf_osal_thread_create(task2, NULL, &attr2);
    if (thread2 == NULL) {
        XF_LOGE(TAG, "xf check error: %d", thread2);
        return;
    }

}

/* ==================== [Static Functions] ================================== */
static void task1(void *argument)
{
    xf_err_t err = XF_OK;
    err |= xf_osal_delay_ms(1000);
    err |= xf_osal_thread_notify_set(thread2, 0x01);
    XF_LOGI(TAG, "task1, err:%d", err);
    while (1)
    {
        if (xf_osal_thread_notify_wait(0x01, XF_OSAL_WAIT_ALL, XF_OSAL_WAIT_FOREVER) == XF_OK)
        {
            XF_LOGI(TAG, "task1 running");
            xf_osal_delay_ms(1000);
        }
    }
}

static void task2(void *argument)
{
    XF_LOGI(TAG, "task2");
    while (1) {
        xf_err_t err = xf_osal_thread_notify_wait(0x01, XF_OSAL_NO_CLEAR, XF_OSAL_WAIT_FOREVER);
        if (err == XF_OK) {
            uint32_t notify = xf_osal_thread_notify_get();
            XF_LOGI(TAG, "task2 running, notify:%d", notify);
            err = xf_osal_thread_notify_clear(0x01);
            if (err != XF_OK)
            {
                XF_LOGE(TAG, "task2 error, %d", err);
            }

            err = xf_osal_thread_notify_set(thread1, 0x01);
            if (err != XF_OK)
            {
                XF_LOGE(TAG, "task2 error, %d", err);
            }
            xf_osal_delay_ms(1000);
        }
        else
        {
            XF_LOGE(TAG, "task2 error, %d", err);
        }

    }
}
