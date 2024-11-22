/**
 * @file port_dac.c
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_port.h"

#if XF_HAL_DAC_IS_ENABLE

#include "xf_init.h"
#include "xf_utils.h"
#include "cJSON.h"
#include "cJSON_Utils.h"
#include "port_utils.h"
#include "port_common.h"
#include "xf_heap.h"

/* ==================== [Defines] =========================================== */

#define TAG "port_dac"

#define XF_HAL_DAC_DEFAULT_ENABLE       false
#define XF_HAL_DAC_DEFAULT_RESOLUTION   10
#define XF_HAL_DAC_DEFAULT_SPEED        1000000
#define XF_HAL_DAC_DEFAULT_VALUE_MAX    255
#define XF_HAL_DAC_DEFAULT_VERF_MV      3300

/* ==================== [Typedefs] ========================================== */

typedef struct _port_dac_t {
    uint32_t id;
    cJSON *json;
    char *json_str;
} port_dac_t;

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
    port_dac_t *dac = (port_dac_t *)xf_malloc(sizeof(port_dac_t));
    if (dac == NULL) {
        return XF_ERR_NO_MEM;
    }

    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        return XF_ERR_NO_MEM;
    }
    dac->id = ID_WITH_TYPE_AND_INDEX(XF_HAL_DAC_TYPE, dev->id);
    dac->json = json;
    dac->json_str = NULL;
    dev->platform_data = dac;

    cJSON_AddNumberToObject(json, "id",         dac->id);
    cJSON_AddBoolToObject(  json, "enable",     0);
    cJSON_AddNumberToObject(json, "resolution", 0);
    cJSON_AddNumberToObject(json, "speed",      0);
    cJSON_AddNumberToObject(json, "value_max",  0);
    cJSON_AddNumberToObject(json, "verf_mv",    0);

    return XF_OK;
}

static xf_err_t port_dac_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    port_dac_t *dac = (port_dac_t *)dev->platform_data;
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

    if (dac->json_str != NULL) {
        free(dac->json_str);
        dac->json_str = NULL;
    }

    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetBoolValue,   dac->json, dac_config, enable);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, dac->json, dac_config, resolution);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, dac->json, dac_config, speed);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, dac->json, dac_config, value_max);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, dac->json, dac_config, verf_mv);
    
    dac->json_str = cJSON_PrintUnformatted(dac->json);
    unsigned int size = strlen(dac->json_str);
    websocket_send(XF_HAL_CONFIG_ID, dac->json_str, size);
    return XF_OK;
}

static int port_dac_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    // no need
    return XF_ERR_NOT_SUPPORTED;
}

static int port_dac_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    port_dac_t *dac = (port_dac_t *)dev->platform_data;

    uint32_t value = *(uint32_t *)buf;
    websocket_send(dac->id, (unsigned char *)&value, sizeof(value));
    return XF_OK;
}

static xf_err_t port_dac_close(xf_hal_dev_t *dev)
{
    port_dac_t *dac = (port_dac_t *)dev->platform_data;

    if (dac->json_str != NULL) {
        free(dac->json_str);
        dac->json_str = NULL;
    }
    cJSON_Delete(dac->json);
    free(dac);
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

#endif  // XF_HAL_UART_IS_ENABLE
