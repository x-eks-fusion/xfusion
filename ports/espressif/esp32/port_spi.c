/**
 * @file port_spi.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-17
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_port.h"

#if XF_HAL_SPI_IS_ENABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver/spi_master.h"
#include "driver/spi_slave.h"
#include "soc/soc_caps.h"
#include "esp_log.h"

#include "xf_utils.h"
#include "xf_init.h"
#include "xf_heap.h"

/* ==================== [Defines] =========================================== */

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
    xf_hal_spi_config_t *spi_config;
    uint32_t port;
    uint32_t id;
    spi_device_handle_t handle;
    union {
        spi_transaction_t master_trans;
        spi_slave_transaction_t slave_trans;
    };
} port_spi_t;

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static xf_err_t port_spi_open(xf_hal_dev_t *dev);
static xf_err_t port_spi_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_spi_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_spi_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static xf_err_t port_spi_close(xf_hal_dev_t *dev);

static void spi_prev_master_transfer_callback(spi_transaction_t *t);
static void spi_post_master_transfer_callback(spi_transaction_t *t);

static void spi_prev_slave_transfer_callback(spi_slave_transaction_t *t);
static void spi_post_slave_transfer_callback(spi_slave_transaction_t *t);

/* ==================== [Static Variables] ================================== */

static bool spi_init[SOC_SPI_PERIPH_NUM] = {0};
static const char *tag = "port_spi";

/* ==================== [Macros] ============================================ */

#define XF_HAL_SPI_ERR(x) \
    if (x != 0) {\
        ESP_LOGD(tag, "error code:%d", x);\
        return x;\
    }

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_err_t port_spi_open(xf_hal_dev_t *dev)
{
    port_spi_t *spi = (port_spi_t *)xf_malloc(sizeof(port_spi_t));
    if (spi == NULL) {
        return XF_ERR_NO_MEM;
    }

    spi->port = dev->id % 10;
    spi->id = dev->id;

    dev->platform_data = spi;

    return XF_OK;
}

static xf_err_t port_spi_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_spi_config_t *spi_config = (xf_hal_spi_config_t *)config;
    port_spi_t *spi = (port_spi_t *)dev->platform_data;

    if (cmd == XF_HAL_SPI_CMD_DEFAULT) {
        spi_config->hosts               = XF_HAL_SPI_DEFAULT_HOSTS;
        spi_config->bit_order           = XF_HAL_SPI_DEFAULT_BIT_ORDER;
        spi_config->mode                = XF_HAL_SPI_DEFAULT_MODE;
        spi_config->data_width          = XF_HAL_SPI_DEFAULT_DATA_WIDTH;
        spi_config->timeout_ms             = XF_HAL_SPI_DEFAULT_TIMEOUT;
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
        spi->spi_config                 = spi_config;

        return XF_OK;
    }

    if (cmd == XF_HAL_SPI_CMD_ALL) {
        return XF_OK;
    }

    if (cmd & XF_HAL_SPI_CMD_ENABLE) {
        if (spi_config->enable) {
            if (spi_config->hosts == XF_HAL_SPI_HOSTS_MASTER) {
                spi_bus_config_t buscfg = {
                    .miso_io_num = spi_config->gpio.miso_num,
                    .mosi_io_num = spi_config->gpio.mosi_num,
                    .sclk_io_num = spi_config->gpio.sclk_num,
                    .quadwp_io_num = spi_config->gpio.quadwp_num,
                    .quadhd_io_num = spi_config->gpio.quadhd_num,
                    .max_transfer_sz = 0
                };
                spi_device_interface_config_t devcfg = {
                    .clock_speed_hz = spi_config->speed,
                    .mode = spi_config->mode,
                    .spics_io_num = spi_config->gpio.cs_num,
                    .queue_size = 7,
                    .pre_cb = spi_prev_master_transfer_callback,
                    .post_cb = spi_post_master_transfer_callback,
                };
                if (!spi_init[spi->port]) {
                    XF_HAL_SPI_ERR(spi_bus_initialize(spi->port, &buscfg, SPI_DMA_CH_AUTO));
                    spi_init[spi->port] = true;
                }
                XF_HAL_SPI_ERR(spi_bus_add_device(spi->port, &devcfg, &spi->handle));
                ESP_LOGD(tag, "spi init");
            } else {
                if (spi_init[spi->port]) {
                    // 被占用
                    return XF_FAIL;
                }
                spi_bus_config_t buscfg = {
                    .miso_io_num = spi_config->gpio.miso_num,
                    .mosi_io_num = spi_config->gpio.mosi_num,
                    .sclk_io_num = spi_config->gpio.sclk_num,
                    .quadwp_io_num = spi_config->gpio.quadwp_num,
                    .quadhd_io_num = spi_config->gpio.quadhd_num,
                    .max_transfer_sz = 0
                };

                spi_slave_interface_config_t slvcfg = {
                    .mode = spi_config->mode,
                    .spics_io_num = spi_config->gpio.cs_num,
                    .queue_size = 7,
                    .post_setup_cb = spi_prev_slave_transfer_callback,
                    .post_trans_cb = spi_post_slave_transfer_callback,
                };

                XF_HAL_SPI_ERR(spi_slave_initialize(spi->port, &buscfg, &slvcfg,
                                                    SPI_DMA_CH_AUTO));
            }
        }
    }

    return XF_OK;
}

static int port_spi_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    esp_err_t err = ESP_OK;
    port_spi_t *spi = (port_spi_t *)dev->platform_data;

    if (spi->spi_config->hosts == XF_HAL_SPI_HOSTS_MASTER) {
        memset(&spi->master_trans, 0, sizeof(spi_transaction_t));
        spi->master_trans.length = count * 8;
        spi->master_trans.rx_buffer = buf;
        spi->master_trans.user = spi;
        err = spi_device_queue_trans(spi->handle, &spi->master_trans,
                               pdMS_TO_TICKS(spi->spi_config->timeout_ms));
    } else {
        memset(&spi->slave_trans, 0, sizeof(spi_slave_transaction_t));
        spi->slave_trans.length = count * 8;
        spi->slave_trans.rx_buffer = buf;
        spi->slave_trans.user = spi;
        err = spi_slave_transmit(spi->port, &spi->slave_trans,
                           pdMS_TO_TICKS(spi->spi_config->timeout_ms));
    }
    /* 如果无错 -> 返回传入到大小（因为此处方法无返回实际大小（读或写） */
    if(err == ESP_OK)
    {
        return count;
    }
    else if(err == ESP_FAIL)    // 有负数错误 -> 直接返回错误码
    {
        return err;
    }
    return -err;    // 有正数错误 -> 返回错误码的倒数（负数）
}

static int port_spi_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    esp_err_t err = ESP_OK;
    port_spi_t *spi = (port_spi_t *)dev->platform_data;

    if (spi->spi_config->hosts == XF_HAL_SPI_HOSTS_MASTER) {
        memset(&spi->master_trans, 0, sizeof(spi_transaction_t));
        spi->master_trans.length = count * 8;
        spi->master_trans.tx_buffer = buf;
        spi->master_trans.user = spi;
        ESP_LOGD(tag, "handle:%p", (spi->handle));
        err = spi_device_queue_trans(spi->handle, &spi->master_trans,
                               pdMS_TO_TICKS(spi->spi_config->timeout_ms));
    } else {
        memset(&spi->slave_trans, 0, sizeof(spi_slave_transaction_t));
        spi->slave_trans.length = count * 8;
        spi->slave_trans.tx_buffer = buf;
        spi->slave_trans.user = spi;

        err = spi_slave_transmit(spi->port, &spi->slave_trans,
                           pdMS_TO_TICKS(spi->spi_config->timeout_ms));
    }
    /* 如果无错 -> 返回传入到大小（因为此处方法无返回实际大小（读或写） */
    if(err == ESP_OK)
    {
        return count;
    }
    else if(err == ESP_FAIL)    // 有负数错误 -> 直接返回错误码
    {
        return err;
    }
    return -err;    // 有正数错误 -> 返回错误码的倒数（负数）
}

static xf_err_t port_spi_close(xf_hal_dev_t *dev)
{
    port_spi_t *spi = (port_spi_t *)dev->platform_data;
    spi_bus_remove_device(spi->handle);
    xf_free(spi);
    return XF_OK;
}

static void spi_prev_master_transfer_callback(spi_transaction_t *t)
{
    port_spi_t *spi = (port_spi_t *)t->user;
    if (spi->spi_config->prev_cb.callback) {
        spi->spi_config->prev_cb.callback(spi->id, spi->spi_config->prev_cb.user_data);
    }
}

static void spi_post_master_transfer_callback(spi_transaction_t *t)
{
    port_spi_t *spi = (port_spi_t *)t->user;
    if (spi->spi_config->post_cb.callback) {
        spi->spi_config->post_cb.callback(spi->id, spi->spi_config->post_cb.user_data);
    }
}

static void spi_prev_slave_transfer_callback(spi_slave_transaction_t *t)
{
    port_spi_t *spi = (port_spi_t *)t->user;
    if (spi->spi_config->prev_cb.callback) {
        spi->spi_config->prev_cb.callback(spi->id, spi->spi_config->prev_cb.user_data);
    }
}

static void spi_post_slave_transfer_callback(spi_slave_transaction_t *t)
{
    port_spi_t *spi = (port_spi_t *)t->user;
    if (spi->spi_config->post_cb.callback) {
        spi->spi_config->post_cb.callback(spi->id, spi->spi_config->post_cb.user_data);
    }
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

#endif
