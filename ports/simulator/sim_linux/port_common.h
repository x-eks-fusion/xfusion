/**
 * @file port_common.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-12
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __PORT_COMMON_H__
#define __PORT_COMMON_H__

/* ==================== [Includes] ========================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_HAL_GPIO_TYPE 1
#define XF_HAL_PWM_TYPE 2
#define XF_HAL_TIM_TYPE 3
#define XF_HAL_UART_TYPE 4
#define XF_HAL_I2C_TYPE 5
#define XF_HAL_SPI_TYPE 6
#define XF_HAL_ADC_TYPE 7
#define XF_HAL_DAC_TYPE 8

#define XF_HAL_CONFIG_ID 0
#define XF_HAL_GET_ID 1
#define XF_HAL_ISR_ID 2

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __PORT_COMMON_H__
