/**
 * @file port_adc.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-13
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_port.h"

#if XF_HAL_ADC_IS_ENABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

#include "xf_utils.h"
#include "xf_init.h"
#include "xf_heap.h"

/* ==================== [Defines] =========================================== */

#define XF_HAL_ADC_DEFAULT_ENABLE       false
#define XF_HAL_ADC_DEFAULT_RESOLUTION   ADC_BITWIDTH_DEFAULT
#define XF_HAL_ADC_DEFAULT_SAMPLE_RATE  1000

/* ==================== [Typedefs] ========================================== */

typedef struct _port_adc_t {
    adc_unit_t unit;
    adc_channel_t channel;
    adc_oneshot_unit_handle_t oneshot;
} port_adc_t;

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static xf_err_t port_adc_open(xf_hal_dev_t *dev);
static xf_err_t port_adc_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_adc_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_adc_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static xf_err_t port_adc_close(xf_hal_dev_t *dev);

/* ==================== [Static Variables] ================================== */

static const char *tag = "port_adc";

/* ==================== [Macros] ============================================ */

#define XF_HAL_ADC_ERR(x) \
    if (x != 0) { \
        ESP_LOGI(tag, "error code:%d", x); \
        return x; \
    }

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_err_t port_adc_open(xf_hal_dev_t *dev)
{
    port_adc_t *adc = (port_adc_t *)xf_malloc(sizeof(port_adc_t));
    if (adc == NULL) {
        return XF_ERR_NO_MEM;
    }
    memset(adc, 0, sizeof(port_adc_t));

    adc->unit = dev->id / 8;
    adc->channel = dev->id % 8;

    dev->platform_data = adc;

    return XF_OK;
}

static xf_err_t port_adc_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_adc_config_t *adc_config = (xf_hal_adc_config_t *)config;
    port_adc_t *adc = (port_adc_t *)dev->platform_data;

    ESP_LOGD(tag, "cmd:%ld", cmd);
    if (cmd == XF_HAL_ADC_CMD_DEFAULT) {
        adc_config->enable = XF_HAL_ADC_DEFAULT_ENABLE;
        adc_config->resolution = XF_HAL_ADC_DEFAULT_RESOLUTION;
        adc_config->sample_rate = XF_HAL_ADC_DEFAULT_SAMPLE_RATE;

        return XF_OK;
    }

    if (cmd == XF_HAL_ADC_CMD_ALL) {
        return XF_OK;
    }

    if (cmd & XF_HAL_ADC_CMD_ENABLE) {
        if (adc_config->enable) {
            adc_oneshot_unit_init_cfg_t init_config = {
                .unit_id = adc->unit,
            };
            adc_oneshot_new_unit(&init_config, &adc->oneshot);
            adc_oneshot_chan_cfg_t config = {
                .bitwidth = adc_config->resolution,
                .atten = ADC_ATTEN_DB_11,
            };
            adc_oneshot_config_channel(adc->oneshot, adc->channel, &config);
            ESP_LOGD(tag, "adc init");
        } else  {
            adc_oneshot_del_unit(adc->oneshot);
        }
    }

    if (cmd & XF_HAL_ADC_CMD_RESOLUTION && adc_config->enable) {
        // 不支持动态设置参数
        return XF_ERR_NOT_SUPPORTED;
    }

    if (cmd & XF_HAL_ADC_CMD_SAMPLE_RATE && adc_config->enable) {
        // 不支持动态设置参数
        return XF_ERR_NOT_SUPPORTED;
    }

    return XF_OK;
}

static int port_adc_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    port_adc_t *adc = (port_adc_t *)dev->platform_data;
    ESP_LOGD(tag, "handle:%p, buf:%p", adc->oneshot, buf);
    adc_oneshot_read(adc->oneshot, adc->channel, buf);
    return XF_OK;
}

static int port_adc_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    // no need
    return XF_ERR_NOT_SUPPORTED;
}

static xf_err_t port_adc_close(xf_hal_dev_t *dev)
{
    port_adc_t *adc = (port_adc_t *)dev->platform_data;
    xf_free(adc);
    return XF_OK;
}

static int xf_hal_adc_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_adc_open,
        .ioctl = port_adc_ioctl,
        .write = port_adc_write,
        .read = port_adc_read,
        .close = port_adc_close,
    };
    xf_hal_adc_register(&ops);
    return XF_OK;
}

XF_INIT_EXPORT_PREV(xf_hal_adc_reg);

#endif
