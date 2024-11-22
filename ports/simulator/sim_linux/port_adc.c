/**
 * @file port_adc.c
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

#if XF_HAL_ADC_IS_ENABLE

#include "xf_init.h"
#include "xf_utils.h"
#include "cJSON.h"
#include "cJSON_Utils.h"
#include "port_utils.h"
#include "port_common.h"
#include "xf_heap.h"

/* ==================== [Defines] =========================================== */

#define TAG "port_adc"

#define XF_HAL_ADC_DEFAULT_ENABLE       false
#define XF_HAL_ADC_DEFAULT_RESOLUTION   12
#define XF_HAL_ADC_DEFAULT_SAMPLE_RATE  1000

/* ==================== [Typedefs] ========================================== */

typedef struct _port_adc_t {
    uint32_t id;
    cJSON *json;
    char *json_str;
} port_adc_t;

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static xf_err_t port_adc_open(xf_hal_dev_t *dev);
static xf_err_t port_adc_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_adc_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_adc_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static xf_err_t port_adc_close(xf_hal_dev_t *dev);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_err_t port_adc_open(xf_hal_dev_t *dev)
{
    port_adc_t *adc = (port_adc_t *)xf_malloc(sizeof(port_adc_t));
    if (adc == NULL) {
        return XF_ERR_NO_MEM;
    }

    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        return XF_ERR_NO_MEM;
    }
    adc->id = ID_WITH_TYPE_AND_INDEX(XF_HAL_ADC_TYPE, dev->id);
    adc->json = json;
    adc->json_str = NULL;
    dev->platform_data = adc;

    cJSON_AddNumberToObject(json, "id",         adc->id);
    cJSON_AddBoolToObject(  json, "enable",     0);
    cJSON_AddNumberToObject(json, "resolution", 0);
    cJSON_AddNumberToObject(json, "sample_rate",0);


    return XF_OK;
}

static xf_err_t port_adc_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    port_adc_t *adc = (port_adc_t *)dev->platform_data;
    xf_hal_adc_config_t *adc_config = (xf_hal_adc_config_t *)config;

    if (cmd == XF_HAL_ADC_CMD_DEFAULT) {
        adc_config->enable = XF_HAL_ADC_DEFAULT_ENABLE;
        adc_config->resolution = XF_HAL_ADC_DEFAULT_RESOLUTION;
        adc_config->sample_rate = XF_HAL_ADC_DEFAULT_SAMPLE_RATE;

        return XF_OK;
    }

    if (cmd == XF_HAL_ADC_CMD_ALL) {
        return XF_OK;
    }

    if (adc->json_str != NULL) {
        free(adc->json_str);
        adc->json_str = NULL;
    }

    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetBoolValue,   adc->json, adc_config, enable);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, adc->json, adc_config, resolution);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, adc->json, adc_config, sample_rate);
    
    adc->json_str = cJSON_PrintUnformatted(adc->json);
    unsigned int size = strlen(adc->json_str);
    websocket_send(XF_HAL_CONFIG_ID, adc->json_str, size);
    return XF_OK;
}

static int port_adc_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    port_adc_t *adc = (port_adc_t *)dev->platform_data;

    char req_read[64] = {0};
    // 因为上层使用的是 uint32_t ，所以长度为 sizeof(uint32_t)
    snprintf(req_read, sizeof(req_read),"{\"id\":%d,\"len\":%ld}", adc->id, sizeof(uint32_t));
    websocket_send(XF_HAL_GET_ID, req_read, strlen(req_read));
    websocket_recv(buf);
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

    if (adc->json_str != NULL) {
        free(adc->json_str);
        adc->json_str = NULL;
    }
    cJSON_Delete(adc->json);
    free(adc);
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

#endif  // XF_HAL_ADC_IS_ENABLE
