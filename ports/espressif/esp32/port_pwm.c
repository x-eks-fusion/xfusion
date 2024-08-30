/**
 * @file port_pwm.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-11
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_port.h"

#if XF_HAL_PWM_IS_ENABLE

#include <stdio.h>
#include <stdlib.h>

#include "driver/ledc.h"
#include "esp_log.h"

#include "xf_utils.h"
#include "xf_init.h"
#include "xf_heap.h"

/* ==================== [Defines] =========================================== */

#define XF_HAL_PWM_DEFAULT_ENABLE           false
#define XF_HAL_PWM_DEFAULT_FREQ             5000
#define XF_HAL_PWM_DEFAULT_DUTY             0
#define XF_HAL_PWM_DEFAULT_DUTY_RESOLUTION  10

#define ESP32_PWM_FREQ                      40*1000*1000

/* ==================== [Typedefs] ========================================== */

typedef struct _port_pwm_t {
    ledc_timer_t timer;
    ledc_channel_t channel;
    ledc_mode_t speed_mode;
} port_pwm_t;

/* ==================== [Static Prototypes] ================================= */

static xf_err_t port_pwm_open(xf_hal_dev_t *dev);
static xf_err_t port_pwm_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_pwm_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_pwm_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static xf_err_t port_pwm_close(xf_hal_dev_t *dev);

static int esp32_find_suitable_duty_resolution(uint32_t freq);

/* ==================== [Static Variables] ================================== */

static const char *tag = "port_pwm";

/* ==================== [Macros] ============================================ */

#define XF_HAL_PWM_ERR(x) \
    if (x != 0) {\
        ESP_LOGD(tag, "error code:%d", x);\
        return x;\
    }

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_err_t port_pwm_open(xf_hal_dev_t *dev)
{
    port_pwm_t *pwm = (port_pwm_t *)xf_malloc(sizeof(port_pwm_t));
    if (pwm == NULL) {
        return XF_ERR_NO_MEM;
    }

    pwm->timer = (dev->id / 2) %  4;
    pwm->channel = dev->id % 8;
    pwm->speed_mode = dev->id / 8;

    dev->platform_data = pwm;

    return XF_OK;
}

static xf_err_t port_pwm_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_pwm_config_t *pwm_config = (xf_hal_pwm_config_t *)config;
    port_pwm_t *pwm = (port_pwm_t *)dev->platform_data;

    if (cmd == XF_HAL_PWM_CMD_DEFAULT) {
        pwm_config->enable = XF_HAL_PWM_DEFAULT_ENABLE;
        pwm_config->freq = XF_HAL_PWM_DEFAULT_FREQ;
        pwm_config->duty = XF_HAL_PWM_DEFAULT_DUTY;
        pwm_config->duty_resolution = XF_HAL_PWM_DEFAULT_DUTY_RESOLUTION;
        pwm_config->io_num = dev->id;

        return XF_OK;
    }

    ESP_LOGD(tag, "cmd:%ld", cmd);
    ESP_LOGD(tag, "pwm enable:%d", pwm_config->enable);

    if ((cmd & XF_HAL_PWM_CMD_FREQ) && (pwm_config->enable)) {
        pwm_config->duty_resolution = esp32_find_suitable_duty_resolution(
                                          pwm_config->freq);
        XF_HAL_PWM_ERR(ledc_set_freq(pwm->speed_mode, pwm->timer, pwm_config->freq));
    }

    if ((cmd & XF_HAL_PWM_CMD_DUTY) && (pwm_config->enable)) {
        XF_HAL_PWM_ERR(ledc_set_duty(pwm->speed_mode, pwm->channel, pwm_config->duty));
        XF_HAL_PWM_ERR(ledc_update_duty(pwm->speed_mode, pwm->channel));
    }

    if ((cmd & XF_HAL_PWM_CMD_DUTY_RESOLUTION) && (pwm_config->enable)) {
        // 不支持动态修改， disable后修改再enable
        return XF_ERR_NOT_SUPPORTED;
    }

    if ((cmd & XF_HAL_PWM_CMD_IO_NUM) && (pwm_config->enable)) {
        XF_HAL_PWM_ERR(ledc_set_pin(pwm_config->io_num, pwm->speed_mode, pwm->channel));
    }

    if ((cmd & XF_HAL_PWM_CMD_ENABLE) && cmd != XF_HAL_PWM_CMD_ALL) {
        if (pwm_config->enable) {
            ledc_timer_config_t ledc_timer = {
                .speed_mode       = pwm->speed_mode,
                .timer_num        = pwm->timer,
                .duty_resolution  = pwm_config->duty_resolution,
                .freq_hz          = pwm_config->freq,
                .clk_cfg          = LEDC_AUTO_CLK
            };
            ledc_channel_config_t ledc_channel = {
                .speed_mode     = pwm->speed_mode,
                .channel        = pwm->channel,
                .timer_sel      = pwm->timer,
                .intr_type      = LEDC_INTR_DISABLE,
                .gpio_num       = pwm_config->io_num,
                .duty           = pwm_config->duty,
                .hpoint         = 0
            };
            XF_HAL_PWM_ERR(ledc_timer_config(&ledc_timer));
            XF_HAL_PWM_ERR(ledc_channel_config(&ledc_channel));
        } else {
            XF_HAL_PWM_ERR(ledc_stop(pwm->speed_mode, pwm->channel, 0));
        }
    }
    return XF_OK;
}

static int port_pwm_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    // no need
    return XF_ERR_NOT_SUPPORTED;
}

static int port_pwm_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    // no need
    return XF_ERR_NOT_SUPPORTED;
}

static xf_err_t port_pwm_close(xf_hal_dev_t *dev)
{
    port_pwm_t *pwm = (port_pwm_t *)dev->platform_data;
    xf_free(pwm);
    return XF_OK;
}

static int esp32_find_suitable_duty_resolution(uint32_t freq)
{
    int duty_resolution = 0;
    uint32_t freq_ = ESP32_PWM_FREQ;
    for (int i = 1; freq_ >=  freq; i++) {
        freq_ = freq_ / 2;
        duty_resolution = i;
    }
    return duty_resolution;
}

static int xf_hal_pwm_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_pwm_open,
        .ioctl = port_pwm_ioctl,
        .write = port_pwm_write,
        .read = port_pwm_read,
        .close = port_pwm_close,
    };
    xf_hal_pwm_register(&ops);
    return XF_OK;
}

XF_INIT_EXPORT_PREV(xf_hal_pwm_reg);

#endif
