/**
 * @file port_sys.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-08
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_sys.h"
#include "esp_timer.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"
#include "esp_task_wdt.h"
#include "port_common.h"
#include "xf_init.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_err_t xf_sys_watchdog_enable(void)
{
    esp_task_wdt_config_t config = {
        .timeout_ms = 5 * 1000,
        .idle_core_mask = 0x03,
        .trigger_panic = false,
    };
    return port_convert_pf2xf_err(esp_task_wdt_init(&config));
}

xf_err_t xf_sys_watchdog_disable(void)
{
    return port_convert_pf2xf_err(esp_task_wdt_deinit());
}

xf_err_t xf_sys_watchdog_kick(void)
{
    return port_convert_pf2xf_err(esp_task_wdt_reset());
}

void xf_sys_reboot(void)
{
    esp_restart();
}

/* ==================== [Static Functions] ================================== */

static xf_us_t _port_xf_sys_get_us(void)
{
    int timer = esp_timer_get_time();
    return timer;
}

static xf_err_t port_sys_init()
{
    xf_sys_time_init(_port_xf_sys_get_us);
    return XF_OK;
}

XF_INIT_EXPORT_BOARD(port_sys_init);
