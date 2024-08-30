/**
 * @file port_uart.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-15
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_port.h"

#if XF_HAL_UART_IS_ENABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver/uart.h"
#include "esp_log.h"

#include "xf_utils.h"
#include "xf_init.h"
#include "xf_heap.h"

/* ==================== [Defines] =========================================== */

#define XF_HAL_UART_DEFAULT_DATA_BITS       XF_HAL_UART_DATA_BIT_8
#define XF_HAL_UART_DEFAULT_STOP_BITS       XF_HAL_UART_STOP_BIT_1
#define XF_HAL_UART_DEFAULT_PARITY_BITS     XF_HAL_UART_PARITY_BITS_NONE
#define XF_HAL_UART_DEFAULT_FLOW_CONTROL    XF_HAL_UART_FLOW_CONTROL_NONE
#define XF_HAL_UART_DEFAULT_BAUDRATE        115200
#define XF_HAL_UART_DEFAULT_TX_NUM          3
#define XF_HAL_UART_DEFAULT_RX_NUM          4
#define XF_HAL_UART_DEFAULT_RTS_NUM         -1
#define XF_HAL_UART_DEFAULT_CTS_NUM         -1

#define RX_BUF_SIZE 1024

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static xf_err_t port_uart_open(xf_hal_dev_t *dev);
static xf_err_t port_uart_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_uart_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_uart_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static xf_err_t port_uart_close(xf_hal_dev_t *dev);

/* ==================== [Static Variables] ================================== */

static const char *tag = "port_uart";

/* ==================== [Macros] ============================================ */

#define XF_HAL_UART_ERR(x) \
    if (x != 0) {\
        ESP_LOGD(tag, "error code:%d", x);\
        return x;\
    }

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_err_t port_uart_open(xf_hal_dev_t *dev)
{
    return XF_OK;
}

static xf_err_t port_uart_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_uart_config_t *uart_config = (xf_hal_uart_config_t *)config;

    if (cmd == XF_HAL_UART_CMD_DEFAULT) {
        uart_config->data_bits      = XF_HAL_UART_DEFAULT_DATA_BITS;
        uart_config->stop_bits      = XF_HAL_UART_DEFAULT_STOP_BITS;
        uart_config->parity_bits    = XF_HAL_UART_DEFAULT_PARITY_BITS;
        uart_config->flow_control   = XF_HAL_UART_DEFAULT_FLOW_CONTROL;
        uart_config->baudrate       = XF_HAL_UART_DEFAULT_BAUDRATE;
        uart_config->tx_num         = XF_HAL_UART_DEFAULT_TX_NUM;
        uart_config->rx_num         = XF_HAL_UART_DEFAULT_RX_NUM;
        uart_config->rts_num        = XF_HAL_GPIO_NUM_NONE;
        uart_config->cts_num        = XF_HAL_GPIO_NUM_NONE;

        return XF_OK;
    }

    if (cmd == XF_HAL_UART_CMD_ALL) {
        return XF_OK;
    }

    if ((cmd & XF_HAL_UART_CMD_BAUDRATE) && (uart_config->enable)) {
        XF_HAL_UART_ERR(uart_set_baudrate(dev->id, uart_config->baudrate));
    }

    if ((cmd & XF_HAL_UART_CMD_DATA_BITS) && (uart_config->enable)) {
        XF_HAL_UART_ERR(uart_set_word_length(dev->id, uart_config->data_bits));
    }

    if ((cmd & XF_HAL_UART_CMD_STOP_BITS) && (uart_config->enable)) {
        XF_HAL_UART_ERR(uart_set_stop_bits(dev->id, uart_config->stop_bits + 1));
    }

    if ((cmd & XF_HAL_UART_CMD_PARITY_BITS) && (uart_config->enable)) {
        XF_HAL_UART_ERR(uart_set_parity(dev->id, uart_config->parity_bits));
    }

    if ((cmd & XF_HAL_UART_CMD_FLOW_CONTROL) && (uart_config->enable)) {
        XF_HAL_UART_ERR(uart_set_hw_flow_ctrl(dev->id, uart_config->flow_control,
                                              SOC_UART_FIFO_LEN - 1));
    }

    if (((cmd & XF_HAL_UART_CMD_TX_NUM) || (cmd & XF_HAL_UART_CMD_RX_NUM) ||
            (cmd & XF_HAL_UART_CMD_RTS_NUM) || (cmd & XF_HAL_UART_CMD_CTS_NUM))
            && (uart_config->enable)) {
        XF_HAL_UART_ERR(uart_set_pin(dev->id, uart_config->tx_num, uart_config->rx_num,
                                     uart_config->rts_num, uart_config->cts_num));
    }

    if (cmd & XF_HAL_UART_CMD_ENABLE) {
        if (uart_config->enable) {
            const uart_config_t config = {
                .baud_rate = uart_config->baudrate,
                .data_bits = uart_config->data_bits,
                .parity = uart_config->parity_bits,
                .stop_bits = uart_config->stop_bits + 1,
                .flow_ctrl = uart_config->flow_control,
                .source_clk = UART_SCLK_DEFAULT,
            };
            if (!uart_is_driver_installed(dev->id)) {
                ESP_LOGD(tag, "uart enable tx:%ld, rx:%ld", uart_config->tx_num, uart_config->rx_num);
                XF_HAL_UART_ERR(uart_driver_install(dev->id, RX_BUF_SIZE * 2, 0, 0, NULL, 0));
            }
            XF_HAL_UART_ERR(uart_param_config(dev->id, &config));
            XF_HAL_UART_ERR(uart_set_pin(dev->id, uart_config->tx_num, uart_config->rx_num,
                                         uart_config->rts_num, uart_config->cts_num));
        } else {
            if (uart_is_driver_installed(dev->id)) {
                ESP_LOGD(tag, "uart disable");
                XF_HAL_UART_ERR(uart_driver_delete(dev->id));
            }
        }
    }

    return XF_OK;
}

static int port_uart_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    return uart_read_bytes(dev->id, buf, count, 0);
}

static int port_uart_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    return uart_write_bytes(dev->id, buf, count);
}

static xf_err_t port_uart_close(xf_hal_dev_t *dev)
{
    return XF_OK;
}

static int xf_hal_uart_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_uart_open,
        .ioctl = port_uart_ioctl,
        .write = port_uart_write,
        .read = port_uart_read,
        .close = port_uart_close,
    };
    xf_hal_uart_register(&ops);

    return XF_OK;
}

XF_INIT_EXPORT_PREV(xf_hal_uart_reg);

#endif
