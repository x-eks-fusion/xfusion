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
 * @example{lineno} examples/peripherals/uart/uart_echo/main/xf_main.c
 * xf_hal uart 回显示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"
#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define UART_NUM        1
#define UART_BAUDRATE   115200
#define UART_TX_NUM     4
#define UART_RX_NUM     5

#define TASK_PRIORITY   5
#define TASK_DELAY_MS   10

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void uart_task(xf_task_t task);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_hal_uart_init(UART_NUM, UART_BAUDRATE);
    xf_hal_uart_set_gpio(UART_NUM, UART_TX_NUM, UART_RX_NUM);
    xf_hal_uart_enable(UART_NUM);
    xf_hal_uart_write(UART_NUM, (const uint8_t *)"uart echo\n", 11);
    xf_ntask_create_loop(uart_task, NULL, TASK_PRIORITY, TASK_DELAY_MS);
}

/* ==================== [Static Functions] ================================== */
static void uart_task(xf_task_t task)
{
    UNUSED(task);
    uint8_t data[1024] = {0};
    size_t size = 1024;
    int len = xf_hal_uart_read(UART_NUM, data, size - 1);
    if (len > 0) {
        xf_hal_uart_write(UART_NUM, (const uint8_t *) data, len);
    }
}
