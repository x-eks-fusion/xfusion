/**
 * @file port_pwm.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-11
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

#define XF_HAL_PWM_DEFAULT_ENABLE           false
#define XF_HAL_PWM_DEFAULT_FREQ             5000
#define XF_HAL_PWM_DEFAULT_DUTY             0
#define XF_HAL_PWM_DEFAULT_DUTY_RESOLUTION  10
#define XF_HAL_PWM_DEFAULT_IO_NUM           0

/* ==================== [Typedefs] ========================================== */

typedef struct _port_pwm_t {
    uint32_t id;
    cJSON *json;
    char *json_str;
} port_pwm_t;

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static int port_pwm_open(xf_hal_dev_t *dev)
{
    port_pwm_t *pwm = (port_pwm_t *)malloc(sizeof(port_pwm_t));
    if (pwm == NULL) {
        return XF_ERR_NO_MEM;
    }

    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        return XF_ERR_NO_MEM;
    }
    pwm->id = XF_HAL_PWM_TYPE << 24 | dev->id;
    pwm->json = json;
    pwm->json_str = NULL;
    dev->platform_data = pwm;

    cJSON_AddNumberToObject(json, "id", pwm->id);
    cJSON_AddBoolToObject(json, "enable", 0);
    cJSON_AddNumberToObject(json, "io_num", 0);
    cJSON_AddNumberToObject(json, "freq", 0);
    cJSON_AddNumberToObject(json, "duty", 0);
    cJSON_AddNumberToObject(json, "duty_resolution", 0);

    return XF_OK;
}

static int port_pwm_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_pwm_config_t *pwm_config = (xf_hal_pwm_config_t *)config;
    port_pwm_t *pwm = (port_pwm_t *)dev->platform_data;

    if (cmd == XF_HAL_PWM_CMD_DEFAULT) {
        pwm_config->enable = XF_HAL_PWM_DEFAULT_ENABLE;
        pwm_config->freq = XF_HAL_PWM_DEFAULT_FREQ;
        pwm_config->duty = XF_HAL_PWM_DEFAULT_DUTY;
        pwm_config->duty_resolution = XF_HAL_PWM_DEFAULT_DUTY_RESOLUTION;
        pwm_config->io_num = XF_HAL_PWM_DEFAULT_IO_NUM;
        return XF_OK;
    }

    if (cmd ==  XF_HAL_PWM_CMD_ALL)
    {
        return XF_OK;
    }

    if (cmd & XF_HAL_PWM_CMD_ENABLE) {
        if (pwm->json_str != NULL) {
            cJSON_free(pwm->json_str);
            pwm->json_str = NULL;
        }

        cJSON_SetBoolValue(cJSONUtils_GetPointer(pwm->json, "/enable"), pwm_config->enable);
        cJSON_SetNumberValue(cJSONUtils_GetPointer(pwm->json, "/freq"), pwm_config->freq);
        cJSON_SetNumberValue(cJSONUtils_GetPointer(pwm->json, "/duty"), pwm_config->duty);
        cJSON_SetNumberValue(cJSONUtils_GetPointer(pwm->json, "/duty_resolution"), pwm_config->duty_resolution);
        cJSON_SetNumberValue(cJSONUtils_GetPointer(pwm->json, "/io_num"), pwm_config->io_num);
        pwm->json_str = cJSON_PrintUnformatted(pwm->json);
        unsigned int size = strlen(pwm->json_str);

        tcp_send(XF_HAL_CONFIG_ID, pwm->json_str, size);

        return XF_OK;
    }

    return XF_OK;
}

static int port_pwm_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    // no need
    return -XF_ERR_NOT_SUPPORTED;
}

static int port_pwm_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    // no need
    return -XF_ERR_NOT_SUPPORTED;
}

static int port_pwm_close(xf_hal_dev_t *dev)
{
    port_pwm_t *pwm = (port_pwm_t *)dev->platform_data;
    if (pwm->json_str != NULL) {
        cJSON_free(pwm->json_str);
        pwm->json_str = NULL;
    }
    cJSON_Delete(pwm->json);
    free(pwm);
    return XF_OK;
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
    return 0;
}

XF_INIT_EXPORT_PREV(xf_hal_pwm_reg);
