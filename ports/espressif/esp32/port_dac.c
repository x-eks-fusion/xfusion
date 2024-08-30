/**
 * @file port_dac.c
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

#if XF_HAL_DAC_IS_ENABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver/dac.h"
#include "esp_log.h"

#include "xf_utils.h"
#include "xf_init.h"
#include "xf_heap.h"

/* ==================== [Defines] =========================================== */

#define XF_HAL_DAC_DEFAULT_ENABLE       false
#define XF_HAL_DAC_DEFAULT_RESOLUTION   10
#define XF_HAL_DAC_DEFAULT_SPEED        1000000
#define XF_HAL_DAC_DEFAULT_VALUE_MAX    255
#define XF_HAL_DAC_DEFAULT_VERF_MV      3300

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static xf_err_t port_dac_open(xf_hal_dev_t *dev);
static xf_err_t port_dac_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_dac_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_dac_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static xf_err_t port_dac_close(xf_hal_dev_t *dev);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_err_t port_dac_open(xf_hal_dev_t *dev)
{
    return XF_OK;
}

static xf_err_t port_dac_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_dac_config_t *dac_config = (xf_hal_dac_config_t *)config;

    if (cmd == XF_HAL_DAC_CMD_DEFAULT) {
        dac_config->enable      = XF_HAL_DAC_DEFAULT_ENABLE;
        dac_config->resolution  = XF_HAL_DAC_DEFAULT_RESOLUTION;
        dac_config->speed       = XF_HAL_DAC_DEFAULT_SPEED;
        dac_config->value_max   = XF_HAL_DAC_DEFAULT_VALUE_MAX;
        dac_config->verf_mv     = XF_HAL_DAC_DEFAULT_VERF_MV;
    }

    if (cmd == XF_HAL_DAC_CMD_ALL) {
        return XF_OK;
    }

    if (cmd & XF_HAL_DAC_CMD_ENABLE) {
        if (dac_config->enable) {
            dac_output_enable(dev->id);
        } else {
            dac_output_disable(dev->id);
        }
    }

    if (cmd & XF_HAL_DAC_CMD_RESOLUTION) {
        // 不支持设置
        return XF_ERR_NOT_SUPPORTED;
    }

    if (cmd & XF_HAL_DAC_CMD_SPEED) {
        // 不支持设置
        return XF_ERR_NOT_SUPPORTED;
    }

    return XF_OK;
}

static int port_dac_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    // no need
    return XF_ERR_NOT_SUPPORTED;
}

static int port_dac_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    uint32_t value = *(uint32_t *)buf;

    dac_output_voltage(dev->id, value);
    return XF_OK;
}

static xf_err_t port_dac_close(xf_hal_dev_t *dev)
{
    return XF_OK;
}

static int xf_hal_dac_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_dac_open,
        .ioctl = port_dac_ioctl,
        .write = port_dac_write,
        .read = port_dac_read,
        .close = port_dac_close,
    };
    xf_hal_dac_register(&ops);
    return XF_OK;
}

XF_INIT_EXPORT_PREV(xf_hal_dac_reg);

#endif
