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
 * @example{lineno} examples/peripherals/gpio/interrupt/main/xf_main.c
 * xf_hal gpio 外部中断示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"
#include "xf_task.h"


/* ==================== [Defines] =========================================== */

#define OUTPUT_IO           5
#define INTERRUPT_INPUT_IO  0
#define INPUT_IO_MODE       XF_HAL_GPIO_PULL_UP
#define INPUT_IO_TRIGGER_TYPE   XF_HAL_GPIO_INTR_TYPE_ANY

#define TASK_PRIORITY       5
#define TASK_DELAY_MS       500

#define TAG                 "intrrupt"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void xf_hal_gpio_irq_cb(xf_gpio_num_t gpio_num, bool level, void *user_data);
static void gpio_task(xf_task_t task);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_hal_gpio_init(OUTPUT_IO, XF_HAL_GPIO_DIR_OUT);

    xf_hal_gpio_init(INTERRUPT_INPUT_IO, XF_HAL_GPIO_DIR_IN);
    xf_hal_gpio_set_pull(INTERRUPT_INPUT_IO, INPUT_IO_MODE);
    xf_hal_gpio_set_intr_type(INTERRUPT_INPUT_IO, INPUT_IO_TRIGGER_TYPE);
    xf_hal_gpio_set_intr_cb(INTERRUPT_INPUT_IO, xf_hal_gpio_irq_cb, NULL);

    xf_hal_gpio_set_intr_enable(INTERRUPT_INPUT_IO);

    xf_ntask_create_loop(gpio_task, NULL, TASK_PRIORITY, TASK_DELAY_MS);
}

/* ==================== [Static Functions] ================================== */

static void xf_hal_gpio_irq_cb(xf_gpio_num_t gpio_num, bool level, void *user_data)
{
    XF_LOGI(TAG, "gpio:%d", level);
}

static void gpio_task(xf_task_t task)
{
    static bool level = 0;
    level = !level;
    xf_hal_gpio_set_level(OUTPUT_IO, level);
}

