/**
 * @file port_gpio.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-14
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_port.h"

#if XF_HAL_GPIO_IS_ENABLE

#include <stdio.h>
#include <stdlib.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_attr.h"

#include "port_common.h"

#include "xf_utils.h"
#include "xf_task.h"
#include "xf_init.h"
#include "xf_heap.h"

/* ==================== [Defines] =========================================== */

#define XF_HAL_GPIO_DEFAULT_DIRECTION       XF_HAL_GPIO_DIR_IN
#define XF_HAL_GPIO_DEFAULT_SPEED           1000000
#define XF_HAL_GPIO_DEFAULT_PULL            XF_HAL_GPIO_PULL_NONE
#define XF_HAL_GPIO_DEFAULT_INTR_ENABLE     0
#define XF_HAL_GPIO_DEFAULT_INTR_TYPE       XF_HAL_GPIO_INTR_TYPE_DISABLE
#define XF_HAL_GPIO_DEFAULT_CB_CALLBACK     NULL
#define XF_HAL_GPIO_DEFAULT_CB_USER_DATA    NULL
#define XF_HAL_GPIO_DEFAULT_ISR_CALLBACK    NULL
#define XF_HAL_GPIO_DEFAULT_ISR_USER_DATA   NULL

#define ESP32_GPIO_MAX                  GPIO_NUM_MAX

#define TAG "port_gpio"

/* ==================== [Typedefs] ========================================== */

typedef struct _port_gpio_t {
    uint32_t pin;
    bool isr_level;
    xf_hal_gpio_callback_t *isr;
} port_gpio_t;

/* ==================== [Static Prototypes] ================================= */

static xf_err_t port_gpio_open(xf_hal_dev_t *dev);
static xf_err_t port_gpio_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_gpio_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_gpio_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static xf_err_t port_gpio_close(xf_hal_dev_t *dev);

static void xf_isr_task_sub(const void *const data, void *user_data);
static void gpio_isr_handler(void *arg);

/* ==================== [Static Variables] ================================== */

static const char *tag = "port_gpio";

/* ==================== [Macros] ============================================ */

#define XF_HAL_GPIO_ERR(x) \
    if (x != 0) { \
        ESP_LOGI(tag, "error code:%d", x); \
        return x; \
    }

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_err_t port_gpio_open(xf_hal_dev_t *dev)
{
    port_gpio_t *gpio = (port_gpio_t *)xf_malloc(sizeof(port_gpio_t));
    if (gpio == NULL) {
        return XF_ERR_NO_MEM;
    }

    if (dev->id >= ESP32_GPIO_MAX) {
        return XF_ERR_INVALID_ARG;
    }

    gpio->pin = dev->id;

    dev->platform_data = gpio;

    ESP_LOGI(tag, "gpio open");

    return XF_OK;
}

static xf_err_t port_gpio_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_gpio_config_t *io_config = (xf_hal_gpio_config_t *)config;
    port_gpio_t *gpio = (port_gpio_t *)dev->platform_data;

    if (cmd == XF_HAL_GPIO_CMD_DEFAULT) {
        io_config->direction        = XF_HAL_GPIO_DEFAULT_DIRECTION;
        io_config->speed            = XF_HAL_GPIO_DEFAULT_SPEED;
        io_config->pull             = XF_HAL_GPIO_DEFAULT_PULL;
        io_config->intr_enable      = XF_HAL_GPIO_DEFAULT_INTR_ENABLE;
        io_config->intr_type        = XF_HAL_GPIO_DEFAULT_INTR_TYPE;
        io_config->cb.callback      = XF_HAL_GPIO_DEFAULT_CB_CALLBACK;
        io_config->cb.user_data     = XF_HAL_GPIO_DEFAULT_CB_USER_DATA;
        io_config->isr.callback     = XF_HAL_GPIO_DEFAULT_ISR_CALLBACK;
        io_config->isr.user_data    = XF_HAL_GPIO_DEFAULT_ISR_USER_DATA;
        gpio->isr                   = &io_config->isr;
        ESP_LOGI(tag, "gpio set default");
        return XF_OK;
    }

    if (cmd == XF_HAL_GPIO_CMD_ALL) {
        gpio_config_t io_conf = {0};

        io_conf.intr_type = io_config->intr_type;
        io_conf.pin_bit_mask = 1ULL << dev->id;
        io_conf.mode = io_config->direction + 1;
        io_conf.pull_down_en = (io_config->pull == XF_HAL_GPIO_PULL_DOWN ? 1 : 0);
        io_conf.pull_up_en = (io_config->pull == XF_HAL_GPIO_PULL_UP ? 1 : 0);
        XF_HAL_GPIO_ERR(gpio_config(&io_conf));

        ESP_LOGI(tag, "gpio init");
        return XF_OK;
    }

    if (cmd & XF_HAL_GPIO_CMD_DIRECTION) {
        XF_HAL_GPIO_ERR(gpio_set_direction(dev->id, io_config->direction + 1));
        ESP_LOGI(tag, "gpio set direction:%d", io_config->direction + 1);
    }

    if (cmd & XF_HAL_GPIO_CMD_PULL) {
        switch (io_config->pull) {
        case XF_HAL_GPIO_PULL_NONE:
            XF_HAL_GPIO_ERR(gpio_set_pull_mode(dev->id, GPIO_FLOATING));
            ESP_LOGI(tag, "gpio set floating");
            break;
        case XF_HAL_GPIO_PULL_UP:
            XF_HAL_GPIO_ERR(gpio_set_pull_mode(dev->id, GPIO_PULLUP_ONLY));
            ESP_LOGI(tag, "gpio set pull up");
            break;
        case XF_HAL_GPIO_PULL_DOWN:
            XF_HAL_GPIO_ERR(gpio_set_pull_mode(dev->id, GPIO_PULLUP_PULLDOWN));
            ESP_LOGI(tag, "gpio set pull down");
            break;
        default:
            break;
        }

    }

    if (cmd & XF_HAL_GPIO_CMD_INTR_TYPE) {
        XF_HAL_GPIO_ERR(gpio_set_intr_type(dev->id, io_config->intr_type));
        ESP_LOGI(tag, "gpio set intrrupt:%d", io_config->intr_type);
    }

    if (cmd & XF_HAL_GPIO_CMD_INTR_ENABLE) {
        if (io_config->intr_enable) {
            gpio_install_isr_service(0);
            gpio_isr_handler_add(dev->id, gpio_isr_handler, gpio);
            xf_task_mbus_sub(PORT_GPIO_TOPIC_ID, xf_isr_task_sub, &io_config->cb);
            ESP_LOGI(tag, "gpio install isr");
        } else {
            xf_task_mbus_unsub(PORT_GPIO_TOPIC_ID, xf_isr_task_sub);
            gpio_isr_handler_remove(dev->id);
            gpio_uninstall_isr_service();
            ESP_LOGI(tag, "gpio uninstall isr");
        }
    }

    return XF_OK;
}

static int port_gpio_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    bool *level = (bool *)buf;
    *level = gpio_get_level(dev->id);

    return XF_OK;
}

static int port_gpio_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    bool level = *(bool *)buf;
    esp_err_t ret = gpio_set_level(dev->id, level);
    if (ret != XF_OK) {
        return -ret;
    }

    return XF_OK;
}

static xf_err_t port_gpio_close(xf_hal_dev_t *dev)
{
    port_gpio_t *gpio = (port_gpio_t *)dev->platform_data;
    XF_HAL_GPIO_ERR(gpio_reset_pin(dev->id));
    xf_free(gpio);
    return XF_OK;
}

static void xf_isr_task_sub(const void *const data, void *user_data)
{
    port_gpio_t *gpio = (port_gpio_t *) data;
    ESP_LOGI(tag, "task handle isr_level:%d", gpio->isr_level);
    xf_hal_gpio_callback_t *cb = (xf_hal_gpio_callback_t *)user_data;
    if (cb->callback) {
        cb->callback(gpio->pin, gpio->isr_level, cb->user_data);
    }
}

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    port_gpio_t *gpio = (port_gpio_t *) arg;
    gpio->isr_level = gpio_get_level(gpio->pin);
    if (gpio->isr->callback) {
        gpio->isr->callback(gpio->pin, gpio->isr_level, gpio->isr->user_data);
    }
    ESP_EARLY_LOGD(TAG, "send mbus");
    xf_task_mbus_pub_async(PORT_GPIO_TOPIC_ID, (void *)gpio);
}

static int xf_hal_gpio_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_gpio_open,
        .ioctl = port_gpio_ioctl,
        .write = port_gpio_write,
        .read = port_gpio_read,
        .close = port_gpio_close,
    };
    xf_hal_gpio_register(&ops);
    xf_task_mbus_reg_topic(PORT_GPIO_TOPIC_ID, sizeof(port_gpio_t));
    return XF_OK;
}

XF_INIT_EXPORT_PREV(xf_hal_gpio_reg);

#endif
