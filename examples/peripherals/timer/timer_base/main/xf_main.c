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
 * @example{lineno} examples/peripherals/timer/timer_base/main/xf_main.c
 * xf_hal timer 基础示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"

/* ==================== [Defines] =========================================== */

#define TIM0_NUM            0
#define TIM0_FREQ           1000*1000
#define TIM0_AUTO_RELOAD    false
#define TIM0_TARGET_TICKS   500*1000

#define TIM1_NUM            1
#define TIM1_FREQ           1000*1000
#define TIM1_AUTO_RELOAD    true
#define TIM1_TARGET_TICKS   500*1000

#define TAG "timer_base"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void time0_callback(xf_tim_num_t tim_num, uint32_t ticks,
                           void *user_data);
static void time1_callback(xf_tim_num_t tim_num, uint32_t ticks,
                           void *user_data);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_hal_tim_init(TIM0_NUM, TIM0_FREQ, XF_HAL_TIM_COUNT_DIR_UP, TIM0_AUTO_RELOAD);
    xf_hal_tim_set_cb(TIM0_NUM, time0_callback, NULL);
    xf_hal_tim_start(TIM0_NUM, TIM0_TARGET_TICKS);

    xf_hal_tim_init(TIM1_NUM, TIM1_FREQ, XF_HAL_TIM_COUNT_DIR_UP, TIM1_AUTO_RELOAD);
    xf_hal_tim_set_cb(TIM1_NUM, time1_callback, NULL);
    xf_hal_tim_start(TIM1_NUM, TIM1_TARGET_TICKS);
}

/* ==================== [Static Functions] ================================== */

static void time0_callback(xf_tim_num_t tim_num, uint32_t ticks,
                           void *user_data)
{
    XF_LOGI(TAG, "ticks:%ld, user_data:%p", ticks, user_data);
    xf_hal_tim_start(tim_num, ticks + 500 * 1000);
}

static void time1_callback(xf_tim_num_t tim_num, uint32_t ticks,
                           void *user_data)
{
    UNUSED(tim_num);
    XF_LOGI(TAG, "ticks:%ld, user_data:%p", ticks, user_data);
}
