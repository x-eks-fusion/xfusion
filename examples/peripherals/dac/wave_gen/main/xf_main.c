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
#include "kernel/xf_hal_dev.h"
#include "xf_task.h"
#include <math.h>

/* ==================== [Defines] =========================================== */

#define TIM0_NUM                0
#define TIM0_FREQ               1000*1000
#define TIM0_AUTO_RELOAD        true
#define TIM0_TARGET_TICKS       14

#define FREQ                    1000
#define OUTPUT_POINT_NUM       (int)(1000000 / (TIM0_TARGET_TICKS * FREQ) + 0.5)
#define POINT_ARR_LEN           200

#define DAC_NUM                 0

#define CONST_PERIOD_2_PI       6.2832

#define WAVE_CHOOSE             0
#define AMP_DAC                 255

#define TAG                     "wave_gen"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void time0_callback(xf_tim_num_t tim_num, uint32_t ticks, void *user_data);
static void prepare_data(int pnt_num);

/* ==================== [Static Variables] ================================== */

static int raw_val[POINT_ARR_LEN];

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_hal_tim_init(TIM0_NUM, TIM0_FREQ, XF_HAL_TIM_COUNT_DIR_UP, TIM0_AUTO_RELOAD);
    xf_hal_tim_set_isr(TIM0_NUM, time0_callback, NULL);

    xf_hal_dac_init(DAC_NUM);

    prepare_data(OUTPUT_POINT_NUM);

    xf_hal_dac_enable(DAC_NUM);
    xf_hal_tim_start(TIM0_NUM, TIM0_TARGET_TICKS);
}

/* ==================== [Static Functions] ================================== */

static void prepare_data(int pnt_num)
{
    for (int i = 0; i < pnt_num; i ++) {
        if (WAVE_CHOOSE == 0) {
            raw_val[i] = (int)((sin(i * CONST_PERIOD_2_PI / pnt_num) + 1) * (double)(
                                   AMP_DAC) / 2 + 0.5);
            XF_LOGI(TAG, "Generating a sine wave");
        } else if (WAVE_CHOOSE == 1) {
            raw_val[i] = (i > (pnt_num / 2)) ? (2 * AMP_DAC *
                                                (pnt_num - i) / pnt_num) :
                         (2 * AMP_DAC * i / pnt_num);
            XF_LOGI(TAG, "Generating a triangle wave");
        } else if (WAVE_CHOOSE == 2) {
            raw_val[i] = (i == pnt_num) ? 0 : (i * AMP_DAC / pnt_num);
            XF_LOGI(TAG, "Generating a sawtooth wave");
        } else if (WAVE_CHOOSE == 3) {
            raw_val[i] = (i < (pnt_num / 2)) ? AMP_DAC : 0;
            XF_LOGI(TAG, "Generating a square wave");
        }
    }
}

static void time0_callback(xf_tim_num_t tim_num, uint32_t ticks,
                           void *user_data)
{
    UNUSED(tim_num);
    UNUSED(ticks);
    UNUSED(user_data);
    static uint8_t index = 0;
    xf_hal_dac_write(DAC_NUM, raw_val[index]);

    index++;
    if (index >= OUTPUT_POINT_NUM) {
        index = 0;
    }
}
