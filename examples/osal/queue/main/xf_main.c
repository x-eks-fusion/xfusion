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
 * @example{lineno} examples/osal/queue/main/xf_main.c
 * xf_osal 消息队列示例。
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

static xf_osal_queue_t queue = NULL;

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

    xf_osal_queue_attr_t queue_attr = {
        .name = "queue",
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

    queue = xf_osal_queue_create(5, sizeof(int32_t), &queue_attr);
    if (queue == NULL) {
        XF_LOGE(TAG, "xf queue create error");
        return;
    }

    xf_err_t err = xf_osal_queue_reset(queue);
    if (err != XF_OK) {
        XF_LOGE(TAG, "xf queue reset error");
        return;
    }

    xf_osal_queue_t queue1 = xf_osal_queue_create(5, sizeof(int32_t), &queue_attr);
    if (queue1 == NULL) {
        XF_LOGE(TAG, "xf queue create error");
        return;
    }

    err = xf_osal_queue_delete(queue1);
    if (err != XF_OK) {
        XF_LOGE(TAG, "xf queue delete error");
        return;
    }
}

/* ==================== [Static Functions] ================================== */

static void task1(void *argument)
{
    int32_t rxData;
    while (1) {
        if (xf_osal_queue_get(queue, &rxData, 0, XF_OSAL_WAIT_FOREVER) == XF_OK) {
            XF_LOGI(TAG, "xf queue recv %d", rxData);
        }
        xf_osal_delay_ms(1000);
    }
}

static void task2(void *argument)
{
    int32_t txData = 0;
    while (1) {
        txData++;
        XF_LOGI(TAG, "xf queue send %d", txData);
        if (xf_osal_queue_put(queue, &txData, 0, XF_OSAL_WAIT_FOREVER) != XF_OK) {
            XF_LOGI(TAG, "xf queue send error");
        }
        xf_osal_delay_ms(1500);
    }
}
