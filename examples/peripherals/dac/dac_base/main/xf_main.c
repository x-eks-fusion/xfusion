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

#define DAC_NUM         0
#define DAC_OUTPUT_MV   3300
#define TAG "dac_base"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_hal_dac_init(DAC_NUM);
    xf_hal_dac_enable(DAC_NUM);
    xf_hal_dac_write_mv(DAC_NUM, DAC_OUTPUT_MV);
    XF_LOGI(TAG, "write: %dmv", DAC_OUTPUT_MV);
}

/* ==================== [Static Functions] ================================== */
