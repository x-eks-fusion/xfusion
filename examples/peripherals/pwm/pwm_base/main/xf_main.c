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

/* ==================== [Defines] =========================================== */

#define PWM_NUM             0
#define PWM_FREQ            5000
#define PWM_DUTY            512
#define PWM_IO              5
#define PWM_DUTY_RESOLUTION 10 // duty最大可设置为2^10-1

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_hal_pwm_init(PWM_NUM, PWM_FREQ, PWM_DUTY);
    xf_hal_pwm_set_duty_resolution(PWM_NUM, PWM_DUTY_RESOLUTION);
    xf_hal_pwm_set_gpio(PWM_NUM, PWM_IO);
    xf_hal_pwm_enable(PWM_NUM);
}

/* ==================== [Static Functions] ================================== */
