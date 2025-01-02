/**
 * @file port_sys.c
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-05
 *
 * @copyright Copyright (c) 2024
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_sys.h"
#include "xf_init.h"

#include <sys/time.h>
#include <time.h>

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_err_t xf_sys_watchdog_enable(void)
{
    return XF_ERR_NOT_SUPPORTED;
}

xf_err_t xf_sys_watchdog_disable(void)
{
    return XF_ERR_NOT_SUPPORTED;
}

xf_err_t xf_sys_watchdog_kick(void)
{
    return XF_ERR_NOT_SUPPORTED;
}

void xf_sys_reboot(void)
{
}

/* ==================== [Static Functions] ================================== */

static xf_us_t _port_xf_sys_get_us(void)
{
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    return current_time.tv_sec * (1000 * 1000) + current_time.tv_nsec / 1000;
}

static int port_sys_init()
{
    xf_sys_time_init(_port_xf_sys_get_us);
    return XF_OK;
}

XF_INIT_EXPORT_BOARD(port_sys_init);
