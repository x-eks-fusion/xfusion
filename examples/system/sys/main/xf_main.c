/**
 * @file xf_main.c
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

/* ==================== [Defines] =========================================== */

#define TAG "main"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_ms_t ms_before = xf_sys_time_get_ms();
    xf_delay_ms(1000);
    xf_ms_t ms_after = xf_sys_time_get_ms();

    xf_us_t us_before = xf_sys_time_get_us();
    xf_delay_us(10);
    xf_us_t us_after = xf_sys_time_get_us();

    XF_LOGI(TAG, "tick:ms_before:%llu\tms_after:%llu\tms_diff:%llu",
            ms_before, ms_after, ms_after - ms_before);

    XF_LOGI(TAG, "tick:us_before:%llu\tus_after:%llu\tus_diff:%llu",
            us_before, us_after, us_after - us_before);
}

/* ==================== [Static Functions] ================================== */
