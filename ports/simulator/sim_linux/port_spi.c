/**
 * @file port_spi.c
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

#if XF_HAL_SPI_IS_ENABLE

#include "xf_init.h"
#include "xf_utils.h"
#include "cJSON.h"
#include "cJSON_Utils.h"
#include "port_utils.h"
#include "port_common.h"
#include "xf_heap.h"

/* ==================== [Defines] =========================================== */

#define TAG "port_spi"

#define XF_HAL_SPI_DEFAULT_HOSTS                1
#define XF_HAL_SPI_DEFAULT_BIT_ORDER            XF_HAL_SPI_BIT_ORDER_MSB_FIRST
#define XF_HAL_SPI_DEFAULT_MODE                 XF_HAL_SPI_HOSTS_MASTER
#define XF_HAL_SPI_DEFAULT_DATA_WIDTH           XF_HAL_SPI_DATA_WIDTH_8_BITS
#define XF_HAL_SPI_DEFAULT_TIMEOUT              1000
#define XF_HAL_SPI_DEFAULT_SPEED                1000*500
#define XF_HAL_SPI_DEFAULT_SCLK_NUM             1
#define XF_HAL_SPI_DEFAULT_CS_NUM               2
#define XF_HAL_SPI_DEFAULT_MOSI_NUM             3
#define XF_HAL_SPI_DEFAULT_MISO_NUM             4
#define XF_HAL_SPI_DEFAULT_QUADWP_NUM           XF_HAL_GPIO_NUM_NONE
#define XF_HAL_SPI_DEFAULT_QUADHD_NUM           XF_HAL_GPIO_NUM_NONE
#define XF_HAL_SPI_DEFAULT_PREV_CB_CALLBACK     NULL
#define XF_HAL_SPI_DEFAULT_PREV_CB_USER_DATA    NULL
#define XF_HAL_SPI_DEFAULT_POST_CB_CALLBACK     NULL
#define XF_HAL_SPI_DEFAULT_POST_CB_USER_DATA    NULL

/* ==================== [Typedefs] ========================================== */

typedef struct _port_spi_t {
    uint32_t id;
    cJSON *json;
    char *json_str;
} port_spi_t;

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static xf_err_t port_spi_open(xf_hal_dev_t *dev);
static xf_err_t port_spi_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_spi_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_spi_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static xf_err_t port_spi_close(xf_hal_dev_t *dev);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_err_t port_spi_open(xf_hal_dev_t *dev)
{
    port_spi_t *spi = (port_spi_t *)xf_malloc(sizeof(port_spi_t));
    if (spi == NULL) {
        return XF_ERR_NO_MEM;
    }

    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        return XF_ERR_NO_MEM;
    }
    spi->id = ID_WITH_TYPE_AND_INDEX(XF_HAL_SPI_TYPE, dev->id);
    spi->json = json;
    spi->json_str = NULL;
    dev->platform_data = spi;

    cJSON_AddNumberToObject(json, "id",                 spi->id);
    cJSON_AddBoolToObject(  json, "enable",             0);
    cJSON_AddNumberToObject(json, "bit_order",          0);
    cJSON_AddNumberToObject(json, "mode",               0);
    cJSON_AddNumberToObject(json, "data_width",         0);
    cJSON_AddNumberToObject(json, "timeout_ms",         0);
    cJSON_AddNumberToObject(json, "speed",              0);
    cJSON_AddNumberToObject(json, "gpio.sclk_num",      0);
    cJSON_AddNumberToObject(json, "gpio.cs_num",        0);
    cJSON_AddNumberToObject(json, "gpio.mosi_num",      0);
    cJSON_AddNumberToObject(json, "gpio.miso_num",      0);
    cJSON_AddNumberToObject(json, "gpio.quadhd_num",    0);
    cJSON_AddNumberToObject(json, "gpio.quadwp_num",    0);

    return XF_OK;
}

static xf_err_t port_spi_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    port_spi_t *spi = (port_spi_t *)dev->platform_data;
    xf_hal_spi_config_t *spi_config = (xf_hal_spi_config_t *)config;

    if (cmd == XF_HAL_SPI_CMD_DEFAULT) {
        spi_config->hosts               = XF_HAL_SPI_DEFAULT_HOSTS;
        spi_config->bit_order           = XF_HAL_SPI_DEFAULT_BIT_ORDER;
        spi_config->mode                = XF_HAL_SPI_DEFAULT_MODE;
        spi_config->data_width          = XF_HAL_SPI_DEFAULT_DATA_WIDTH;
        spi_config->timeout_ms            = XF_HAL_SPI_DEFAULT_TIMEOUT;
        spi_config->speed               = XF_HAL_SPI_DEFAULT_SPEED;
        spi_config->gpio.sclk_num       = XF_HAL_SPI_DEFAULT_SCLK_NUM;
        spi_config->gpio.cs_num         = XF_HAL_SPI_DEFAULT_CS_NUM;
        spi_config->gpio.mosi_num       = XF_HAL_SPI_DEFAULT_MOSI_NUM;
        spi_config->gpio.miso_num       = XF_HAL_SPI_DEFAULT_MISO_NUM;
        spi_config->gpio.quadhd_num     = XF_HAL_SPI_DEFAULT_QUADWP_NUM;
        spi_config->gpio.quadwp_num     = XF_HAL_SPI_DEFAULT_QUADHD_NUM;
        spi_config->prev_cb.callback    = XF_HAL_SPI_DEFAULT_PREV_CB_CALLBACK;
        spi_config->prev_cb.user_data   = XF_HAL_SPI_DEFAULT_PREV_CB_USER_DATA;
        spi_config->post_cb.callback    = XF_HAL_SPI_DEFAULT_POST_CB_CALLBACK;
        spi_config->post_cb.user_data   = XF_HAL_SPI_DEFAULT_POST_CB_USER_DATA;

        return XF_OK;
    }

    if (cmd == XF_HAL_SPI_CMD_ALL) {
        return XF_OK;
    }

    if (spi->json_str != NULL) {
        free(spi->json_str);
        spi->json_str = NULL;
    }

    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetBoolValue,   spi->json, spi_config, enable);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, spi->json, spi_config, bit_order);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, spi->json, spi_config, mode);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, spi->json, spi_config, data_width);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, spi->json, spi_config, timeout_ms);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, spi->json, spi_config, speed);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, spi->json, spi_config, gpio.sclk_num);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, spi->json, spi_config, gpio.cs_num);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, spi->json, spi_config, gpio.mosi_num);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, spi->json, spi_config, gpio.miso_num);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, spi->json, spi_config, gpio.quadhd_num);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, spi->json, spi_config, gpio.quadwp_num);

    spi->json_str = cJSON_PrintUnformatted(spi->json);
    unsigned int size = strlen(spi->json_str);
    websocket_send(XF_HAL_CONFIG_ID, spi->json_str, size);
    return XF_OK;
}

static int port_spi_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    port_spi_t *spi = (port_spi_t *)dev->platform_data;
    
    char req_read[64] = {0};
    snprintf(req_read, sizeof(req_read),"{\"id\":%d,\"len\":%ld}", spi->id, count);
    websocket_send(XF_HAL_GET_ID, req_read, strlen(req_read));
    count = websocket_recv(buf);
    return count;
}

static int port_spi_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    port_spi_t *spi = (port_spi_t *)dev->platform_data;

    websocket_send(spi->id, (unsigned char *)buf, count);
    return count;
}

static xf_err_t port_spi_close(xf_hal_dev_t *dev)
{
    port_spi_t *spi = (port_spi_t *)dev->platform_data;

    if (spi->json_str != NULL) {
        free(spi->json_str);
        spi->json_str = NULL;
    }
    cJSON_Delete(spi->json);
    free(spi);
    return XF_OK;
}

static int xf_hal_spi_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_spi_open,
        .ioctl = port_spi_ioctl,
        .write = port_spi_write,
        .read = port_spi_read,
        .close = port_spi_close,
    };
    xf_hal_spi_register(&ops);
    return XF_OK;
}

XF_INIT_EXPORT_PREV(xf_hal_spi_reg);

#endif  // XF_HAL_SPI_IS_ENABLE
