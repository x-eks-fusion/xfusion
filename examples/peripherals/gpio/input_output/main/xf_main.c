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

/**
 * @example{lineno} examples/peripherals/gpio/input_output/main/xf_main.c
 * xf_hal gpio 输入输出示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"
#include "xf_task.h"
#include "xf_utils.h"

/* ==================== [Defines] =========================================== */

#define TAG "exm_gpio"

#define OUTPUT_IO       5
#define INPUT_IO        3
#define INPUT_IO_MODE   XF_HAL_GPIO_PULL_DOWN

#define TASK_PRIORITY   5
#define TASK_DELAY_MS   500

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void gpio_task(xf_task_t task);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    XF_LOGI(TAG, "gpio example");
    xf_hal_gpio_init(OUTPUT_IO, XF_HAL_GPIO_DIR_OUT);
    xf_hal_gpio_init(INPUT_IO, XF_HAL_GPIO_DIR_IN);
    xf_hal_gpio_set_pull(INPUT_IO, INPUT_IO_MODE);

    xf_ntask_create_loop(gpio_task, NULL, TASK_PRIORITY, TASK_DELAY_MS);
}

/* ==================== [Static Functions] ================================== */

static void gpio_task(xf_task_t task)
{
    UNUSED(task);
    bool level = xf_hal_gpio_get_level(INPUT_IO);
    XF_LOGI(TAG, "gpio_task get level:%d", level);
    xf_hal_gpio_set_level(OUTPUT_IO, level);
}
