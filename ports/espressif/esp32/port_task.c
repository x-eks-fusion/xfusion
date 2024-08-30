/**
 * @file port_task.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-22
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "esp_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "xf_utils.h"
#include "xf_task.h"
#include "xf_init.h"

/* ==================== [Defines] =========================================== */

#define CLOCK_PER_SEC  1000UL

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_task_time_t xf_task_clock(void)
{
    return (xf_task_time_t)esp_timer_get_time() / 1000;
}

static void xf_task_on_idle(unsigned long int max_idle_ms)
{
    vTaskDelay(pdMS_TO_TICKS(max_idle_ms));
}

static int port_task_init(void)
{
    xf_task_tick_init(CLOCK_PER_SEC, xf_task_clock);
    xf_task_manager_default_init(xf_task_on_idle);

    return XF_OK;
}

XF_INIT_EXPORT_BOARD(port_task_init);
