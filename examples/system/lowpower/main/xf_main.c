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
#include "xf_lp.h"
#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define TAG "lp"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void task(xf_task_t task);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_lp_config_t lp_config = {
        .is_auto_sleep = true,
    };
    XF_LOGI(TAG, "hello");
    xf_ttask_create_loop(task, (void *)1, 0, 500);
    xf_lp_configure(&lp_config);
}

/* ==================== [Static Functions] ================================== */

static void task(xf_task_t task)
{
	XF_LOGI(TAG, "hello");
}
