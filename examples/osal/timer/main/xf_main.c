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
    return;\
}

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

void timer1(void *argument);
void timer2(void *argument);

/* ==================== [Static Variables] ================================== */

static xf_osal_timer_t timer = NULL;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_osal_timer_attr_t attr1 = {
        .name = "timer1",
    };
    xf_osal_timer_attr_t attr2 = {
        .name = "timer2",
    };
    timer = xf_osal_timer_create(timer1, XF_OSAL_TIMER_PERIODIC, NULL, &attr1);
    XF_LOGI(TAG, "timer: %p", timer);
    if (timer == NULL) {
        XF_LOGE(TAG, "xf check error: %d", timer);
        return;
    }
    xf_osal_timer_t timer_2 = xf_osal_timer_create(timer2, XF_OSAL_TIMER_PERIODIC, NULL, &attr2);
    if (timer_2 == NULL) {
        XF_LOGE(TAG, "xf check error: %d", timer_2);
        return;
    }
    const char *name = xf_osal_timer_get_name(timer);
    XF_LOGI(TAG, "timer name: %s", name);
    name = xf_osal_timer_get_name(timer_2);
    XF_LOGI(TAG, "timer name: %s", name);
    XF_OSAL_CHECK(xf_osal_timer_start(timer, 1000U));
    XF_OSAL_CHECK(xf_osal_timer_start(timer_2, 300U));
}

/* ==================== [Static Functions] ================================== */

void timer1(void *argument)
{
    static uint32_t count = 0;
    count++;
    XF_LOGI(TAG, "timer1 count: %ld", count);
}

void timer2(void *argument)
{
    static uint32_t count = 0;
    count++;
    XF_LOGI(TAG, "timer2 count: %ld", count);
    if (count % 10 == 0 && count < 40) {
        if (xf_osal_timer_is_running(timer) == 1) {
            xf_osal_timer_stop(timer);
        } else {
            xf_osal_timer_start(timer, 1000U);
        }
    }
    if (count == 40) {
        xf_osal_timer_delete(timer);
    }

}
