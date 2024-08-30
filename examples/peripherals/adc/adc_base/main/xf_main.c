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
#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define ADC_NUM         4

#define TASK_PRIORITY   5
#define TASK_DELAY_MS   100

/* ==================== [Typedefs] ========================================== */

#define TAG "adc_base"

/* ==================== [Static Prototypes] ================================= */

static void adc_read_task(xf_task_t task);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_hal_adc_init(ADC_NUM);
    xf_hal_adc_enable(ADC_NUM);
    xf_ntask_create_loop(adc_read_task, NULL, TASK_PRIORITY, TASK_DELAY_MS);
}

/* ==================== [Static Functions] ================================== */

static void adc_read_task(xf_task_t task)
{
    UNUSED(task);
    uint32_t value = xf_hal_adc_read_raw(ADC_NUM);
    XF_LOGI(TAG, "value:%u", value);
}
