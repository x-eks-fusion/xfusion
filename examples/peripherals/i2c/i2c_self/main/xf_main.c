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

#define TAG "i2c_self"

#define DEFAULT_I2C_MASTER_NUM          1
#define DEFAULT_I2C_MASTER_SCL_IO_NUM   19
#define DEFAULT_I2C_MASTER_SDA_IO_NUM   18
#define DEFAULT_I2C_MASTER_DEV_ADDR     0x23
#define DEFAULT_I2C_MASTER_FREQ         400*1000

#define DEFAULT_I2C_SLAVE_NUM          0
#define DEFAULT_I2C_SLAVE_SCL_IO_NUM   5
#define DEFAULT_I2C_SLAVE_SDA_IO_NUM   4
#define DEFAULT_I2C_SLAVE_DEV_ADDR     0x23
#define DEFAULT_I2C_SLAVE_FREQ         400*1000

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void i2c_slave_init(void);
static void i2c_master_init(void);
static void i2c_echo(void);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    i2c_slave_init();
    i2c_master_init();
    i2c_echo();
}

/* ==================== [Static Functions] ================================== */

static void i2c_slave_init(void)
{
    xf_hal_i2c_init(DEFAULT_I2C_SLAVE_NUM, XF_HAL_I2C_HOSTS_SLAVE, DEFAULT_I2C_SLAVE_FREQ);
    xf_hal_i2c_set_gpio(DEFAULT_I2C_SLAVE_NUM,
        DEFAULT_I2C_SLAVE_SCL_IO_NUM, DEFAULT_I2C_SLAVE_SDA_IO_NUM);
    xf_hal_i2c_set_address(DEFAULT_I2C_SLAVE_NUM, DEFAULT_I2C_SLAVE_DEV_ADDR);
    xf_hal_i2c_enable(DEFAULT_I2C_SLAVE_NUM);
}

static void i2c_master_init(void)
{
    xf_hal_i2c_init(DEFAULT_I2C_MASTER_NUM, XF_HAL_I2C_HOSTS_MASTER, DEFAULT_I2C_MASTER_FREQ);
    xf_hal_i2c_set_gpio(DEFAULT_I2C_MASTER_NUM,
        DEFAULT_I2C_MASTER_SCL_IO_NUM, DEFAULT_I2C_MASTER_SDA_IO_NUM);
    xf_hal_i2c_set_address(DEFAULT_I2C_MASTER_NUM, DEFAULT_I2C_MASTER_DEV_ADDR);
    xf_hal_i2c_enable(DEFAULT_I2C_MASTER_NUM);
}

static void i2c_echo(void)
{
    uint8_t msg[] = "hello";
    uint8_t recv[sizeof(msg)] = {0};
    xf_hal_i2c_write(DEFAULT_I2C_MASTER_NUM, msg, sizeof(msg), 0);
    xf_hal_i2c_read(DEFAULT_I2C_SLAVE_NUM, recv, sizeof(msg), 0);
    XF_LOGI(TAG, "recv:%s", recv);
}

