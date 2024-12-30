/**
 * @file port_uart.c
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

#if XF_HAL_UART_IS_ENABLE

#include "xf_init.h"
#include "xf_utils.h"
#include "cJSON.h"
#include "cJSON_Utils.h"
#include "port_utils.h"
#include "tcp.h"
#include "port_common.h"
#include "xf_heap.h"

/* ==================== [Defines] =========================================== */

#define TAG "port_uart"

#define XF_HAL_UART_DEFAULT_DATA_BITS       XF_HAL_UART_DATA_BIT_8
#define XF_HAL_UART_DEFAULT_STOP_BITS       XF_HAL_UART_STOP_BIT_1
#define XF_HAL_UART_DEFAULT_PARITY_BITS     XF_HAL_UART_PARITY_BITS_NONE
#define XF_HAL_UART_DEFAULT_FLOW_CONTROL    XF_HAL_UART_FLOW_CONTROL_NONE
#define XF_HAL_UART_DEFAULT_BAUDRATE        115200
#define XF_HAL_UART_DEFAULT_TX_NUM          3
#define XF_HAL_UART_DEFAULT_RX_NUM          4
#define XF_HAL_UART_DEFAULT_RTS_NUM         -1
#define XF_HAL_UART_DEFAULT_CTS_NUM         -1

#define RX_BUF_SIZE 1024

/* ==================== [Typedefs] ========================================== */

typedef struct _port_uart_t {
    uint32_t id;
    cJSON *json;
    char *json_str;
} port_uart_t;

/* ==================== [Static Prototypes] ================================= */

static xf_err_t port_uart_open(xf_hal_dev_t *dev);
static xf_err_t port_uart_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_uart_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_uart_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static xf_err_t port_uart_close(xf_hal_dev_t *dev);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_err_t port_uart_open(xf_hal_dev_t *dev)
{
    port_uart_t *uart = (port_uart_t *)xf_malloc(sizeof(port_uart_t));
    if (uart == NULL) {
        return XF_ERR_NO_MEM;
    }

    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        return XF_ERR_NO_MEM;
    }
    uart->id = ID_WITH_TYPE_AND_INDEX(XF_HAL_UART_TYPE, dev->id);
    uart->json = json;
    uart->json_str = NULL;
    dev->platform_data = uart;

    cJSON_AddNumberToObject(json, "id",             uart->id);
    cJSON_AddBoolToObject(  json, "enable",         0);
    cJSON_AddNumberToObject(json, "data_bits",      0);
    cJSON_AddNumberToObject(json, "stop_bits",      0);
    cJSON_AddNumberToObject(json, "parity_bits",    0);
    cJSON_AddNumberToObject(json, "flow_control",   0);
    cJSON_AddNumberToObject(json, "baudrate",       0);
    cJSON_AddNumberToObject(json, "tx_num",         0);
    cJSON_AddNumberToObject(json, "rx_num",         0);
    cJSON_AddNumberToObject(json, "rts_num",        0);
    cJSON_AddNumberToObject(json, "cts_num",        0);

    return XF_OK;
}

static xf_err_t port_uart_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    port_uart_t *uart = (port_uart_t *)dev->platform_data;
    xf_hal_uart_config_t *uart_config = (xf_hal_uart_config_t *)config;

    if (cmd == XF_HAL_UART_CMD_DEFAULT) {
        uart_config->data_bits      = XF_HAL_UART_DEFAULT_DATA_BITS;
        uart_config->stop_bits      = XF_HAL_UART_DEFAULT_STOP_BITS;
        uart_config->parity_bits    = XF_HAL_UART_DEFAULT_PARITY_BITS;
        uart_config->flow_control   = XF_HAL_UART_DEFAULT_FLOW_CONTROL;
        uart_config->baudrate       = XF_HAL_UART_DEFAULT_BAUDRATE;
        uart_config->tx_num         = XF_HAL_UART_DEFAULT_TX_NUM;
        uart_config->rx_num         = XF_HAL_UART_DEFAULT_RX_NUM;
        uart_config->rts_num        = XF_HAL_GPIO_NUM_NONE;
        uart_config->cts_num        = XF_HAL_GPIO_NUM_NONE;

        return XF_OK;
    }

    if (cmd == XF_HAL_UART_CMD_ALL) {
        return XF_OK;
    }

    if (uart->json_str != NULL) {
        free(uart->json_str);
        uart->json_str = NULL;
    }

    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetBoolValue,   uart->json, uart_config, enable);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, uart->json, uart_config, data_bits);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, uart->json, uart_config, stop_bits);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, uart->json, uart_config, parity_bits);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, uart->json, uart_config, flow_control);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, uart->json, uart_config, baudrate);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, uart->json, uart_config, tx_num);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, uart->json, uart_config, rx_num);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, uart->json, uart_config, rts_num);
    CJSON_SET_VAL_WITH_STRUCT(cJSON_SetNumberValue, uart->json, uart_config, cts_num);

    uart->json_str = cJSON_PrintUnformatted(uart->json);
    unsigned int size = strlen(uart->json_str);
    tcp_send(XF_HAL_CONFIG_ID, uart->json_str, size);
    return XF_OK;
}

static int port_uart_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    port_uart_t *uart = (port_uart_t *)dev->platform_data;

    size_t size = tcp_get(uart->id, buf, count);
    return size;
}

static int port_uart_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    port_uart_t *uart = (port_uart_t *)dev->platform_data;

    tcp_send(uart->id, (unsigned char *)buf, count);
    return count;
}

static xf_err_t port_uart_close(xf_hal_dev_t *dev)
{
    port_uart_t *uart = (port_uart_t *)dev->platform_data;

    if (uart->json_str != NULL) {
        free(uart->json_str);
        uart->json_str = NULL;
    }
    cJSON_Delete(uart->json);
    free(uart);
    return XF_OK;
}

static int xf_hal_uart_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_uart_open,
        .ioctl = port_uart_ioctl,
        .write = port_uart_write,
        .read = port_uart_read,
        .close = port_uart_close,
    };
    xf_hal_uart_register(&ops);

    return XF_OK;
}

XF_INIT_EXPORT_PREV(xf_hal_uart_reg);

#endif  // XF_HAL_UART_IS_ENABLE
