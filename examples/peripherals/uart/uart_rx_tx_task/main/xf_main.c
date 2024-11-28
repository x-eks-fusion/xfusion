/**
 * @file xf_main.c
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-16
 *
 * @copyright Copyright (c) 2024
 *
 */

/**
 * @example{lineno} examples/peripherals/uart/uart_rx_tx_task/main/xf_main.c
 * xf_hal uart 读写示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"
#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define UART_NUM        1
#define UART_BAUDRATE   115200

#define UART_TX_NUM     15
#define UART_RX_NUM     16

#define TASK_PRIORITY   5
#define TASK_DELAY_MS   2000

#define RX_SIZE_DEFAULT (64)

#define TAG "example_uart"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void uart_rx_task(xf_task_t task);
static void uart_tx_task(xf_task_t task);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_hal_uart_init(UART_NUM, UART_BAUDRATE);
    xf_hal_uart_set_gpio(UART_NUM, UART_TX_NUM, UART_RX_NUM);
    xf_hal_uart_enable(UART_NUM);
    xf_ntask_create_loop(uart_tx_task, NULL, TASK_PRIORITY, TASK_DELAY_MS);
    xf_ntask_create_loop(uart_rx_task, NULL, TASK_PRIORITY, TASK_DELAY_MS);
}

/* ==================== [Static Functions] ================================== */

static void uart_rx_task(xf_task_t task)
{
    UNUSED(task);
    uint8_t data[RX_SIZE_DEFAULT] = {0};
    int len = xf_hal_uart_read(UART_NUM, data, RX_SIZE_DEFAULT);
    if (len > 0) {
        XF_LOGI(TAG, ">>READ:size:%d,%s", len, data);
    } else {
        XF_LOGI(TAG, ">>READ:NONE");
    }
}

static void uart_tx_task(xf_task_t task)
{
    UNUSED(task);
    uint8_t data[] = "I M UART TX Task";
    int len = xf_hal_uart_write(UART_NUM, (const uint8_t *) data, sizeof(data));
    if (len > 0) {
        XF_LOGI(TAG, ">>WRITE:size:%d", len);
    } else {
        XF_LOGI(TAG, ">>WRITE:NONE");
    }
}
