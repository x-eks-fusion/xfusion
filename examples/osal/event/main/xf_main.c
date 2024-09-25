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

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void task1(void *argument);
static void task2(void *argument);

/* ==================== [Static Variables] ================================== */

xf_osal_event_t event = NULL;

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
    xf_osal_event_attr_t event_attr = {
        .name = "event",
    };

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

    event = xf_osal_event_create(&event_attr);
    if (event == NULL) {
        XF_LOGE(TAG, "xf event create error");
        return;
    }

    xf_osal_event_t event1 = xf_osal_event_create(&event_attr);
    if (event1 == NULL) {
        XF_LOGE(TAG, "xf event create error");
        return;
    }
    xf_err_t err = xf_osal_event_delete(event1);
    if (err != XF_OK) {
        XF_LOGE(TAG, "xf event delete error");
        return;
    }

}

/* ==================== [Static Functions] ================================== */

static void task1(void *argument)
{
    xf_err_t err = XF_OK;
    err |= xf_osal_delay_ms(1000);
    err |= xf_osal_event_set(event, 0x01);
    XF_LOGI(TAG, "task1, err:%d", err);
    while (1) {
        if (xf_osal_event_wait(event, 0x01, XF_OSAL_WAIT_ALL, XF_OSAL_WAIT_FOREVER) == XF_OK) {
            XF_LOGI(TAG, "task1 running");
            xf_osal_delay_ms(1000);
        }
    }
}

static void task2(void *argument)
{
    XF_LOGI(TAG, "task2");
    while (1) {
        xf_err_t err = xf_osal_event_wait(event, 0x01, XF_OSAL_NO_CLEAR, XF_OSAL_WAIT_FOREVER);
        if (err == XF_OK) {
            uint32_t event_flag = xf_osal_event_get(event);
            XF_LOGI(TAG, "task2 running, event:%d", event_flag);
            xf_osal_event_clear(event, 0x01);
            xf_osal_event_set(event, 0x01);
            xf_osal_delay_ms(1000);
        } else {
            XF_LOGE(TAG, "task2 error, %d", err);
        }
    }
}
