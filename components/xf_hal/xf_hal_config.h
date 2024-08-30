/**
 * @file xf_hal_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief 使用 xfusion 菜单配置 xf_hal 内部配置。
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_CONFIG_H__
#define __XF_HAL_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_HAL_LOCK_DISABLE     CONFIG_XF_HAL_LOCK_DISABLE

#define XF_HAL_POSIX_DISABLE    CONFIG_XF_HAL_POSIX_DISABLE

#define XF_HAL_GPIO_ENABLE      CONFIG_XF_HAL_GPIO_ENABLE

#define XF_HAL_PWM_ENABLE       CONFIG_XF_HAL_PWM_ENABLE

#define XF_HAL_TIM_ENABLE       CONFIG_XF_HAL_TIM_ENABLE

#define XF_HAL_UART_ENABLE      CONFIG_XF_HAL_UART_ENABLE

#define XF_HAL_I2C_ENABLE       CONFIG_XF_HAL_I2C_ENABLE

#define XF_HAL_SPI_ENABLE       CONFIG_XF_HAL_SPI_ENABLE

#define XF_HAL_ADC_ENABLE       CONFIG_XF_HAL_ADC_ENABLE

#define XF_HAL_DAC_ENABLE       CONFIG_XF_HAL_DAC_ENABLE

#ifndef XF_LOG_LEVEL
#define XF_LOG_LEVEL            CONFIG_XF_HAL_LOG_LEVEL
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_HAL_CONFIG_H__
