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
#include "xf_osal.h"

/* ==================== [Defines] =========================================== */

#define TAG "main"

#define XF_OSAL_CHECK(x) if (x != XF_OK)\
{\
    XF_LOGE(TAG, "xf check error: %d", x);\
    return;\
}

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_osal_version_t version;

    char id_buf[32] = {0};
    XF_OSAL_CHECK(xf_osal_kernel_get_info(&version, id_buf, sizeof(id_buf)));
    XF_LOGI(TAG, "Kernel version: %s", id_buf);
    xf_osal_state_t state = xf_osal_kernel_get_state();
    XF_LOGI(TAG, "Kernel state: %d", state);
    XF_OSAL_CHECK(xf_osal_kernel_lock());
    XF_OSAL_CHECK(xf_osal_kernel_unlock());
    uint32_t tick_count = xf_osal_kernel_get_tick_count();
    uint32_t tick_freq = xf_osal_kernel_get_tick_freq();
    XF_LOGI(TAG, "Kernel tick count: %ld", tick_count);
    XF_LOGI(TAG, "Kernel tick freq: %ld", tick_freq);
    uint32_t ticks = 0;
    uint32_t ms = 0;
    ticks = 1000;
    ms = xf_osal_kernel_ticks_to_ms(ticks);
    XF_LOGI(TAG, "Kernel ticks(%ld) to ms(%ld)", ticks, ms);
    ms = 1000;
    ticks = xf_osal_kernel_ms_to_ticks(ms);
    XF_LOGI(TAG, "Kernel ms(%ld) to ticks(%ld)", ms, ticks);
}

/* ==================== [Static Functions] ================================== */
