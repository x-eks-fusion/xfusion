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
#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define TAG "i2c_master"

#define DEFAULT_I2C_NUM         1
#define DEFAULT_I2C_SCL_IO_NUM  16
#define DEFAULT_I2C_SDA_IO_NUM  15
#define DEFAULT_I2C_DEV_ADDR    0x23
#define DEFAULT_I2C_FREQ        400*1000

#define TASK_PRIORITY           5
#define TASK_DELAY_MS           2000

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void i2c_master_init(void);
static void i2c_task(xf_task_t task);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    i2c_master_init();
    xf_ntask_create_loop(i2c_task, NULL, TASK_PRIORITY, TASK_DELAY_MS);
}

/* ==================== [Static Functions] ================================== */

static void i2c_master_init(void)
{
    xf_hal_i2c_init(DEFAULT_I2C_NUM, XF_HAL_I2C_HOSTS_MASTER, DEFAULT_I2C_FREQ);
    xf_hal_i2c_set_gpio(DEFAULT_I2C_NUM, 
        DEFAULT_I2C_SCL_IO_NUM, DEFAULT_I2C_SDA_IO_NUM);
    xf_hal_i2c_set_address(DEFAULT_I2C_NUM, DEFAULT_I2C_DEV_ADDR);
    xf_hal_i2c_enable(DEFAULT_I2C_NUM);
}

static void i2c_task(xf_task_t task)
{
    UNUSED(task);
    int ret = 0;
    uint8_t wbuf[] = "I M Master";
    uint8_t rbuf[32] = {0};
    ret = xf_hal_i2c_write(DEFAULT_I2C_NUM, wbuf, sizeof(wbuf), 200);
    if(ret < 0)
    {
        return;
    }
    XF_LOGI(TAG, "WRITE CMPL");
    ret = xf_hal_i2c_read(DEFAULT_I2C_NUM, rbuf, sizeof(rbuf), 2000);
    if(ret < 0)
    {
        return;
    }
    XF_LOGI(TAG, "READ CMPL:%s", rbuf);
}

