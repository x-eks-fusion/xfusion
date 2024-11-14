/**
 * @file port_i2c.c
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_port.h"

#if XF_HAL_I2C_IS_ENABLE

#include "xf_init.h"
#include "xf_utils.h"
#include "cJSON.h"
#include "cJSON_Utils.h"
#include "port_utils.h"
#include "websocket.h"
#include "port_common.h"
#include "xf_heap.h"

/* ==================== [Defines] =========================================== */

#define XF_HAL_I2C_DEFAULT_HOSTS            XF_HAL_I2C_HOSTS_MASTER
#define XF_HAL_I2C_DEFAULT_ENABLE           false
#define XF_HAL_I2C_DEFAULT_ADDRESS_WIDTH    XF_HAL_I2C_ADDRESS_WIDTH_7BIT
#define XF_HAL_I2C_DEFAULT_ADDRESS          0x56
#define XF_HAL_I2C_DEFAULT_MEM_ADDR         0
#define XF_HAL_I2C_DEFAULT_MEM_ADDR_EN      false
#define XF_HAL_I2C_DEFAULT_MEM_ADDR_WIDTH   XF_HAL_I2C_MEM_ADDR_WIDTH_8BIT
#define XF_HAL_I2C_DEFAULT_SPEED            1000000
#define XF_HAL_I2C_DEFAULT_SCL_NUM          -1
#define XF_HAL_I2C_DEFAULT_SDA_NUM          -1
#define XF_HAL_I2C_DEFAULT_TIMEOUT          0

/* ==================== [Typedefs] ========================================== */

typedef struct _port_i2c_t {
    uint32_t id;
    cJSON *json;
    char *json_str;
    xf_hal_i2c_config_t *config;
} port_i2c_t;

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static xf_err_t port_i2c_open(xf_hal_dev_t *dev);
static xf_err_t port_i2c_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_i2c_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_i2c_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static xf_err_t port_i2c_close(xf_hal_dev_t *dev);

static int esp_i2c_read(xf_hal_dev_t *dev, xf_hal_i2c_config_t *i2c_config,
                        const void *buf, size_t count);
static int esp_i2c_read_mem(xf_hal_dev_t *dev, xf_hal_i2c_config_t *i2c_config,
                            const void *buf, size_t count);
static int esp_i2c_write(xf_hal_dev_t *dev, xf_hal_i2c_config_t *i2c_config,
                         const void *buf, size_t count);
static int esp_i2c_write_mem(xf_hal_dev_t *dev, xf_hal_i2c_config_t *i2c_config,
                             const void *buf, size_t count);

/* ==================== [Static Variables] ================================== */

#define TAG "port_i2c"

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_err_t port_i2c_open(xf_hal_dev_t *dev)
{
    port_i2c_t *i2c = (port_i2c_t *)xf_malloc(sizeof(port_i2c_t));
    if (i2c == NULL) {
        return XF_ERR_NO_MEM;
    }

    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        return XF_ERR_NO_MEM;
    }
    i2c->id = ID_WITH_TYPE_AND_INDEX(XF_HAL_I2C_TYPE, dev->id);
    i2c->json = json;
    i2c->json_str = NULL;
    dev->platform_data = i2c;

    cJSON_AddNumberToObject(json, "id",             i2c->id);
    cJSON_AddBoolToObject(  json, "enable",         0);
    cJSON_AddNumberToObject(json, "hosts",          0);
    cJSON_AddNumberToObject(json, "address_width",  0);
    cJSON_AddNumberToObject(json, "mem_addr",       0);
    cJSON_AddBoolToObject(  json, "mem_addr_en",    0);
    cJSON_AddNumberToObject(json, "mem_addr_width", 0);
    cJSON_AddNumberToObject(json, "speed",          0);
    cJSON_AddNumberToObject(json, "scl_num",        0);
    cJSON_AddNumberToObject(json, "sda_num",        0);
    cJSON_AddNumberToObject(json, "timeout_ms",     0);

    return XF_OK;
}

static xf_err_t port_i2c_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    port_i2c_t *i2c = (port_i2c_t *)dev->platform_data;
    xf_hal_i2c_config_t *i2c_config = (xf_hal_i2c_config_t *)config;

    if (cmd == XF_HAL_I2C_CMD_DEFAULT) {
        i2c_config->enable          = XF_HAL_I2C_DEFAULT_ENABLE;
        i2c_config->hosts           = XF_HAL_I2C_DEFAULT_HOSTS;
        i2c_config->address_width   = XF_HAL_I2C_DEFAULT_ADDRESS_WIDTH;
        i2c_config->address         = XF_HAL_I2C_DEFAULT_ADDRESS;
        i2c_config->mem_addr        = XF_HAL_I2C_DEFAULT_MEM_ADDR;
        i2c_config->mem_addr_en     = XF_HAL_I2C_DEFAULT_MEM_ADDR_EN;
        i2c_config->mem_addr_width  = XF_HAL_I2C_DEFAULT_MEM_ADDR_WIDTH;
        i2c_config->speed           = XF_HAL_I2C_DEFAULT_SPEED;
        i2c_config->scl_num         = XF_HAL_I2C_DEFAULT_SCL_NUM;
        i2c_config->sda_num         = XF_HAL_I2C_DEFAULT_SDA_NUM;
        i2c_config->timeout_ms         = XF_HAL_I2C_DEFAULT_TIMEOUT;
        i2c->config                 = config;

        return XF_OK;
    }

    if (cmd == XF_HAL_I2C_CMD_ALL) {
        return XF_OK;
    }

    if (i2c->json_str != NULL) {
        free(i2c->json_str);
        i2c->json_str = NULL;
    }

    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetBoolValue,   i2c->json, i2c_config, enable         );
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, i2c->json, i2c_config, hosts          );
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, i2c->json, i2c_config, address_width  );
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, i2c->json, i2c_config, address        );
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, i2c->json, i2c_config, mem_addr       );
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetBoolValue,   i2c->json, i2c_config, mem_addr_en    );
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, i2c->json, i2c_config, mem_addr_width );
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, i2c->json, i2c_config, speed          );
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, i2c->json, i2c_config, scl_num        );
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, i2c->json, i2c_config, sda_num        );
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, i2c->json, i2c_config, timeout_ms     );

    i2c->json_str = cJSON_PrintUnformatted(i2c->json);
    unsigned int size = strlen(i2c->json_str);
    websocket_send(XF_HAL_CONFIG_ID, i2c->json_str, size);
    return XF_OK;
}

static int port_i2c_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    port_i2c_t *i2c = (port_i2c_t *)dev->platform_data;

    /* 是读寄存器内容的情况需要先发寄存器地址 */
    if (i2c->config->mem_addr_en == XF_HAL_I2C_MEM_ADDR_ENABLE) {
        uint8_t addr_size = (i2c->config->mem_addr_width + 1)*8;
        websocket_send(i2c->id, (unsigned char *)&i2c->config->mem_addr, addr_size);
    }

    char req_read[64] = {0};
    snprintf(req_read, sizeof(req_read),"{\"id\":%u,\"len\":%u}", i2c->id, count);
    websocket_send(XF_HAL_GET_ID, req_read, strlen(req_read));
    count = websocket_recv(buf);
    return count;
}

static int port_i2c_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    port_i2c_t *i2c = (port_i2c_t *)dev->platform_data;

    /* 是写寄存器内容的情况需要先发寄存器地址 */
    if (i2c->config->mem_addr_en == XF_HAL_I2C_MEM_ADDR_ENABLE) {
        uint8_t addr_size = (i2c->config->mem_addr_width + 1)*8;
        websocket_send(i2c->id, (unsigned char *)&i2c->config->mem_addr, addr_size);
    }

    websocket_send(i2c->id, (unsigned char *)buf, count);
    return count;
}

static xf_err_t port_i2c_close(xf_hal_dev_t *dev)
{
    port_i2c_t *i2c = (port_i2c_t *)dev->platform_data;

    if (i2c->json_str != NULL) {
        free(i2c->json_str);
        i2c->json_str = NULL;
    }
    cJSON_Delete(i2c->json);
    free(i2c);
    return XF_OK;
}

static int xf_hal_i2c_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_i2c_open,
        .ioctl = port_i2c_ioctl,
        .write = port_i2c_write,
        .read = port_i2c_read,
        .close = port_i2c_close,
    };
    xf_hal_i2c_register(&ops);
    return XF_OK;
}

XF_INIT_EXPORT_PREV(xf_hal_i2c_reg);

#endif  // XF_HAL_I2C_IS_ENABLE
