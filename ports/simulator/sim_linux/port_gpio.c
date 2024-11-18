/**
 * @file port_gpio.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-12
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_port.h"
#include "xf_init.h"
#include "xf_utils.h"
#include "cJSON.h"
#include "cJSON_Utils.h"
#include "tcp.h"
#include "port_common.h"

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

/* ==================== [Typedefs] ========================================== */

typedef struct _port_gpio_t {
    uint32_t id;
    cJSON *json;
    char *json_str;
} port_gpio_t;

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static int port_gpio_open(xf_hal_dev_t *dev)
{
    port_gpio_t *gpio = (port_gpio_t *)malloc(sizeof(port_gpio_t));
    if (gpio == NULL) {
        return XF_ERR_NO_MEM;
    }
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        return XF_ERR_NO_MEM;
    }
    gpio->id = XF_HAL_GPIO_TYPE << 24 | dev->id;
    gpio->json = json;
    gpio->json_str = NULL;
    dev->platform_data = gpio;

    cJSON_AddNumberToObject(json, "id", gpio->id);
    cJSON_AddNumberToObject(json, "direction", 0);
    cJSON_AddNumberToObject(json, "pull", 0);
    cJSON_AddNumberToObject(json, "speed", 0);
    cJSON_AddNumberToObject(json, "intr_enable", 0);
    cJSON_AddNumberToObject(json, "intr_type", 0);

    return XF_OK;
}

static int port_gpio_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
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
    }

    if (cmd == XF_HAL_GPIO_CMD_ALL) {
        return XF_OK;
    }

    if (gpio->json_str != NULL) {
        cJSON_free(gpio->json_str);
        gpio->json_str = NULL;
    }

    cJSON_SetNumberValue(cJSONUtils_GetPointer(gpio->json, "/direction"), io_config->direction);
    cJSON_SetNumberValue(cJSONUtils_GetPointer(gpio->json, "/pull"), io_config->pull);
    cJSON_SetNumberValue(cJSONUtils_GetPointer(gpio->json, "/speed"), io_config->speed);
    cJSON_SetNumberValue(cJSONUtils_GetPointer(gpio->json, "/intr_enable"), io_config->intr_enable);
    cJSON_SetNumberValue(cJSONUtils_GetPointer(gpio->json, "/intr_type"), io_config->intr_type);
    gpio->json_str = cJSON_PrintUnformatted(gpio->json);
    unsigned int size = strlen(gpio->json_str);

    tcp_send(XF_HAL_CONFIG_ID, gpio->json_str, size);

    return XF_OK;
}

static int port_gpio_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    unsigned char msg[128];
    port_gpio_t *gpio = (port_gpio_t *)dev->platform_data;
    size_t size = tcp_get(gpio->id, buf, count);
    return count;
}

static int port_gpio_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    port_gpio_t *gpio = (port_gpio_t *)dev->platform_data;
    tcp_send(gpio->id, (unsigned char *)buf, count);
    return count;
}

static int port_gpio_close(xf_hal_dev_t *dev)
{
    port_gpio_t *gpio = (port_gpio_t *)dev->platform_data;
    if (gpio->json_str != NULL) {
        cJSON_free(gpio->json_str);
        gpio->json_str = NULL;
    }
    cJSON_Delete(gpio->json);
    free(gpio);
    return XF_OK;
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

    return 0;
}

XF_INIT_EXPORT_PREV(xf_hal_gpio_reg);

