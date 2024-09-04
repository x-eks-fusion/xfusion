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
#include <string.h>

/* ==================== [Defines] =========================================== */

#define TAG "spi_self"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void spi_echo(void);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_hal_spi_gpio_t master_gpio = {
        .mosi_num = 12,
        .miso_num = 13,
        .sclk_num = 16,
        .cs_num = 14
    };

    xf_hal_spi_gpio_t slave_gpio = {
        .mosi_num = 4,
        .miso_num = 5,
        .sclk_num = 18,
        .cs_num = 23
    };

    xf_hal_spi_init(1, XF_HAL_SPI_HOSTS_MASTER, 5000000);
    xf_hal_spi_set_gpio(1, &master_gpio);
    xf_hal_spi_set_mode(1, 0);
    xf_hal_spi_enable(1);

    xf_hal_spi_init(2, XF_HAL_SPI_HOSTS_SLAVE, 5000000);
    xf_hal_spi_set_gpio(2, &slave_gpio);
    xf_hal_spi_set_mode(2, 0);
    xf_hal_spi_enable(2);

    spi_echo();
}

/* ==================== [Static Functions] ================================== */

static void spi_echo(void)
{
    uint8_t msg[] = "hello";
    static uint8_t recv[sizeof(msg)] = {0};
    xf_hal_spi_write(2, msg, sizeof(msg), 10);
    xf_hal_spi_read(1, recv, sizeof(recv), 10);
    XF_LOGI(TAG, "recv:%s", recv);
}
