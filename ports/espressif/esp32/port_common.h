/**
 * @file port_common.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-23
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __PORT_COMMON_H__
#define __PORT_COMMON_H__

/* ==================== [Includes] ========================================== */

#include "esp_log.h"
#include "esp_err.h"
#include "esp_wifi.h"

#include "xf_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define PORT_GPIO_TOPIC_ID  (uint32_t)(-1)
#define PORT_TIM_TOPIC_ID   (uint32_t)(-2)

/* ==================== [Typedefs] ========================================== */

typedef esp_err_t pf_err_t;

/* ==================== [Global Prototypes] ================================= */

xf_err_t port_convert_pf2xf_err(pf_err_t esp_err);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __PORT_COMMON_H__
