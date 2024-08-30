/**
 * @file port_i2c.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-16
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_port.h"

#if XF_HAL_I2C_IS_ENABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver/i2c.h"
#include "esp_log.h"

#include "xf_utils.h"
#include "xf_init.h"
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

static const char *tag = "port_i2c";

/* ==================== [Macros] ============================================ */

#define XF_HAL_I2C_ERR(x) \
    if (x != 0) { \
        ESP_LOGD(tag, "error code:%d", x); \
        return x; \
    }

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_err_t port_i2c_open(xf_hal_dev_t *dev)
{
    port_i2c_t *i2c = (port_i2c_t *)xf_malloc(sizeof(port_i2c_t));
    if (i2c == NULL) {
        return XF_ERR_NO_MEM;
    }

    dev->platform_data = i2c;

    return XF_OK;
}

static xf_err_t port_i2c_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_i2c_config_t *i2c_config = (xf_hal_i2c_config_t *)config;
    port_i2c_t *i2c = (port_i2c_t *)dev->platform_data;

    if (cmd == XF_HAL_I2C_CMD_DEFAULT) {
        i2c_config->hosts           = XF_HAL_I2C_DEFAULT_HOSTS;
        i2c_config->enable          = XF_HAL_I2C_DEFAULT_ENABLE;
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

    if ((cmd & XF_HAL_I2C_CMD_HOSTS
            || cmd & XF_HAL_I2C_CMD_SCL_NUM
            || cmd & XF_HAL_I2C_CMD_SDA_NUM)
            && i2c_config->enable) {
        XF_HAL_I2C_ERR(i2c_set_pin(dev->id,
                                   i2c_config->sda_num, i2c_config->scl_num,
                                   GPIO_PULLUP_ENABLE,
                                   GPIO_PULLUP_ENABLE,
                                   i2c_config->hosts ? I2C_MODE_MASTER : I2C_MODE_SLAVE));
    }

    if (cmd & XF_HAL_I2C_CMD_SPEED && i2c_config->enable) {
        // 不支持动态设置，disable后设置后enable
        return XF_ERR_NOT_SUPPORTED;
    }

    if (cmd & XF_HAL_I2C_CMD_ADDRESS_WIDTH && i2c_config->enable) {
        // 不支持动态设置，disable后设置后enable
        return XF_ERR_NOT_SUPPORTED;
    }

    if (cmd & XF_HAL_I2C_CMD_ENABLE) {
        if (i2c_config->enable) {
            i2c_config_t conf = {
                .sda_io_num = i2c_config->sda_num,
                .scl_io_num = i2c_config->scl_num,
                .sda_pullup_en = GPIO_PULLUP_ENABLE,
                .scl_pullup_en = GPIO_PULLUP_ENABLE,
            };
            if (i2c_config->hosts == XF_HAL_I2C_HOSTS_MASTER) {
                conf.mode = I2C_MODE_MASTER;
                conf.master.clk_speed = i2c_config->speed;
                XF_HAL_I2C_ERR(i2c_param_config(dev->id, &conf));
                XF_HAL_I2C_ERR(i2c_driver_install(dev->id, conf.mode, 0, 0, 0));
                ESP_LOGD(tag, "i2c master init");
            } else {
                conf.mode = I2C_MODE_SLAVE;
                conf.slave.addr_10bit_en = i2c_config->address_width;
                conf.slave.slave_addr = i2c_config->address;
                conf.slave.maximum_speed = i2c_config->speed;
                XF_HAL_I2C_ERR(i2c_param_config(dev->id, &conf));
                XF_HAL_I2C_ERR(i2c_driver_install(dev->id, conf.mode, 1024, 1024, 0));
                ESP_LOGD(tag, "i2c slave init");
            }
        } else {
            XF_HAL_I2C_ERR(i2c_driver_delete(dev->id));
        }
    }

    return XF_OK;
}

static int port_i2c_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    port_i2c_t *i2c = (port_i2c_t *)dev->platform_data;
    xf_hal_i2c_config_t *i2c_config = (xf_hal_i2c_config_t *)i2c->config;

    if (i2c_config->hosts == XF_HAL_I2C_HOSTS_MASTER
            && i2c_config->mem_addr_en == XF_HAL_I2C_MEM_ADDR_ENABLE) {
        return esp_i2c_read_mem(dev, i2c_config, buf, count);
    } else if (i2c_config->hosts == XF_HAL_I2C_HOSTS_MASTER
               && i2c_config->mem_addr_en != XF_HAL_I2C_MEM_ADDR_ENABLE) {
        return esp_i2c_read(dev, i2c_config, buf, count);

    } else if (i2c_config->hosts == XF_HAL_I2C_HOSTS_SLAVE) {
        return i2c_slave_read_buffer(dev->id, buf, count, pdMS_TO_TICKS(i2c_config->timeout_ms));
    }

    return -XF_ERR_NOT_SUPPORTED;
}

static int port_i2c_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    port_i2c_t *i2c = (port_i2c_t *)dev->platform_data;
    xf_hal_i2c_config_t *i2c_config = (xf_hal_i2c_config_t *)i2c->config;

    if (i2c_config->hosts == XF_HAL_I2C_HOSTS_MASTER
            && i2c_config->mem_addr_en == XF_HAL_I2C_MEM_ADDR_ENABLE) {
        return esp_i2c_write_mem(dev, i2c_config, buf, count);
    } else if (i2c_config->hosts == XF_HAL_I2C_HOSTS_MASTER
               && i2c_config->mem_addr_en != XF_HAL_I2C_MEM_ADDR_ENABLE) {
        return esp_i2c_write(dev, i2c_config, buf, count);
    } else if (i2c_config->hosts == XF_HAL_I2C_HOSTS_SLAVE) {
        return i2c_slave_write_buffer(dev->id, buf, count, pdMS_TO_TICKS(i2c_config->timeout_ms));
    }

    return -XF_ERR_NOT_SUPPORTED;
}

static xf_err_t port_i2c_close(xf_hal_dev_t *dev)
{
    port_i2c_t *i2c = (port_i2c_t *)dev->platform_data;

    xf_free(i2c);
    return XF_OK;
}

static int esp_i2c_read(xf_hal_dev_t *dev, xf_hal_i2c_config_t *i2c_config,
                        const void *buf, size_t count)
{
    esp_err_t err = ESP_OK;

    i2c_cmd_handle_t handle = i2c_cmd_link_create();

    err = i2c_master_start(handle);
    if (err != ESP_OK) {
        goto end;
    }

    if (!i2c_config->address_width) {
        err = i2c_master_write_byte(handle,
                                    i2c_config->address << 1 | I2C_MASTER_READ,
                                    true);
    } else {
        uint8_t first_byte = (0xF0 | ((i2c_config->address >> 7) & 0x06));
        uint8_t second_byte = i2c_config->address & 0xFF;
        err |= i2c_master_write_byte(handle, first_byte | I2C_MASTER_READ, true);
        err |= i2c_master_write_byte(handle, second_byte, true);
    }

    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_read(handle, (uint8_t *)buf, count, I2C_MASTER_LAST_NACK);
    if (err != ESP_OK) {
        goto end;
    }

    i2c_master_stop(handle);
    err = i2c_master_cmd_begin(dev->id, handle, pdMS_TO_TICKS(i2c_config->timeout_ms));

end:
    i2c_cmd_link_delete(handle);
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

static int esp_i2c_read_mem(xf_hal_dev_t *dev, xf_hal_i2c_config_t *i2c_config,
                            const void *buf, size_t count)
{
    esp_err_t err = ESP_OK;

    i2c_cmd_handle_t handle = i2c_cmd_link_create();

    err = i2c_master_start(handle);
    if (err != ESP_OK) {
        goto end;
    }

    if (!i2c_config->address_width) {
        err = i2c_master_write_byte(handle,
                                    i2c_config->address << 1 | I2C_MASTER_WRITE,
                                    true);
    } else {
        uint8_t first_byte = (0xF0 | ((i2c_config->address >> 7) & 0x06));
        uint8_t second_byte = i2c_config->address & 0xFF;
        err |= i2c_master_write_byte(handle, first_byte | I2C_MASTER_WRITE, true);
        err |= i2c_master_write_byte(handle, second_byte, true);
    }

    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_write(handle, (uint8_t *)&i2c_config->mem_addr,
                           i2c_config->mem_addr_width + 1, true);
    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_start(handle);
    if (err != ESP_OK) {
        goto end;
    }

    if (!i2c_config->address_width) {
        err = i2c_master_write_byte(handle,
                                    i2c_config->address << 1 | I2C_MASTER_READ,
                                    true);
    } else {
        uint8_t first_byte = (0xF0 | ((i2c_config->address >> 7) & 0x06));
        uint8_t second_byte = i2c_config->address & 0xFF;
        err |= i2c_master_write_byte(handle, first_byte | I2C_MASTER_READ, true);
        err |= i2c_master_write_byte(handle, second_byte, true);
    }

    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_read(handle, (uint8_t *)buf, count, I2C_MASTER_LAST_NACK);
    if (err != ESP_OK) {
        goto end;
    }

    i2c_master_stop(handle);
    err = i2c_master_cmd_begin(dev->id, handle, pdMS_TO_TICKS(i2c_config->timeout_ms));

end:
    i2c_cmd_link_delete(handle);
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

static int esp_i2c_write(xf_hal_dev_t *dev, xf_hal_i2c_config_t *i2c_config,
                         const void *buf, size_t count)
{
    esp_err_t err = ESP_OK;

    i2c_cmd_handle_t handle = i2c_cmd_link_create();

    ESP_LOGD(tag, "i2c write");

    err = i2c_master_start(handle);
    if (err != ESP_OK) {
        ESP_LOGD(tag, "err:%d\n", __LINE__);
        goto end;
    }

    if (!i2c_config->address_width) {
        ESP_LOGD(tag, "7bit\n");
        err = i2c_master_write_byte(handle,
                                    i2c_config->address << 1 | I2C_MASTER_WRITE,
                                    true);
    } else {
        uint8_t first_byte = (0xF0 | ((i2c_config->address >> 7) & 0x06));
        uint8_t second_byte = i2c_config->address & 0xFF;
        err |= i2c_master_write_byte(handle, first_byte | I2C_MASTER_WRITE, true);
        err |= i2c_master_write_byte(handle, second_byte, true);
    }

    if (err != ESP_OK) {
        ESP_LOGD(tag, "err:%d\n", __LINE__);
        goto end;
    }

    err = i2c_master_write(handle, buf, count, true);
    if (err != ESP_OK) {
        ESP_LOGD(tag, "err:%d\n", __LINE__);
        goto end;
    }
    ESP_LOGD(tag, "id:%ld,handle:%p,timeout_ms:%ld\n", dev->id, handle,
             i2c_config->timeout_ms);
    err = i2c_master_stop(handle);
    ESP_LOGD(tag, "err:%d\n", err);
    err = i2c_master_cmd_begin(dev->id, handle, pdMS_TO_TICKS(i2c_config->timeout_ms));
    ESP_LOGD(tag, "err:%d\n", err);

end:
    i2c_cmd_link_delete(handle);
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

static int esp_i2c_write_mem(xf_hal_dev_t *dev, xf_hal_i2c_config_t *i2c_config,
                             const void *buf, size_t count)
{
    esp_err_t err = ESP_OK;

    i2c_cmd_handle_t handle = i2c_cmd_link_create();

    err = i2c_master_start(handle);
    if (err != ESP_OK) {
        goto end;
    }

    if (!i2c_config->address_width) {
        err = i2c_master_write_byte(handle,
                                    i2c_config->address << 1 | I2C_MASTER_WRITE,
                                    true);
    } else {
        uint8_t first_byte = (0xF0 | ((i2c_config->address >> 7) & 0x06));
        uint8_t second_byte = i2c_config->address & 0xFF;
        err |= i2c_master_write_byte(handle, first_byte | I2C_MASTER_WRITE, true);
        err |= i2c_master_write_byte(handle, second_byte, true);
    }

    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_write(handle, (uint8_t *)&i2c_config->mem_addr,
                           i2c_config->mem_addr_width + 1, true);
    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_write(handle, buf, count, true);
    if (err != ESP_OK) {
        goto end;
    }

    i2c_master_stop(handle);
    err = i2c_master_cmd_begin(dev->id, handle, pdMS_TO_TICKS(i2c_config->timeout_ms));

end:
    i2c_cmd_link_delete(handle);
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

#endif
