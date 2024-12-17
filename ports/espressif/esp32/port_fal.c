/**
 * @file port_fal.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-12-13
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"

#include "esp_partition.h"
#include "spi_flash_mmap.h"
#include "esp_log.h"
#include "esp_system.h"

#include "port_common.h"

#include "xf_utils.h"
#include "xf_init.h"
#include "xf_sys.h"
#include "xf_fal.h"

/* ==================== [Defines] =========================================== */

#define _ESP_PARTITION_LEN                  (~0)  /*!< 没什么用，只是为了防止 xf_fal 错误判断 */
#define _ESP_FLASH_DEV_PAGE_SIZE            (256)
#define _ESP_PARTITION_IO_SIZE              (1)

#if defined(CONFIG_PORT_PARTITION_NAME_FOR_XF_FAL)
#   define _ESP_PARTITION_PARTITION_NAME    CONFIG_PORT_PARTITION_NAME_FOR_XF_FAL
#else
#   define _ESP_PARTITION_PARTITION_NAME    "storage"
#endif

/* TODO 暂时没支持将所有 esp32 分区映射到 xf_fal */
#define _ESP_PARTITION_NUM          (1) /*!<  */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief 对接 xf_fal 上下文结构体。
 */
typedef struct _port_xf_fal_ctx_t {
    const esp_partition_t          *esp_partition;
    xf_fal_flash_dev_t              flash_dev;
    xf_fal_partition_t             *partition_table;
    size_t                          partition_table_len;
#if _ESP_PARTITION_NUM > 0
    xf_fal_partition_t              partition_table_cont[_ESP_PARTITION_NUM];
#endif
} port_xf_fal_ctx_t;

/* ==================== [Static Prototypes] ================================= */

static xf_err_t _esp_partition_init(void);
static xf_err_t _esp_partition_deinit(void);
static xf_err_t _esp_partition_read(size_t src_offset, void *dst, size_t size);
static xf_err_t _esp_partition_write(size_t dst_offset, const void *src, size_t size);
static xf_err_t _esp_partition_erase(size_t offset, size_t size);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "port_fal";

static port_xf_fal_ctx_t    s_port_xf_fal_ctx = {0};
static port_xf_fal_ctx_t   *sp_port_xf_fal_ctx = &s_port_xf_fal_ctx;
#define sp_ctx()            (sp_port_xf_fal_ctx)

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static int port_xf_fal(void)
{
#if CONFIG_PORT_XF_FAL_ENABLE
    sp_ctx()->esp_partition = esp_partition_find_first(
                                  ESP_PARTITION_TYPE_DATA,
                                  ESP_PARTITION_SUBTYPE_ANY,
                                  CONFIG_PORT_PARTITION_NAME_FOR_XF_FAL);
    if (NULL == sp_ctx()->esp_partition) {
        XF_LOGE(TAG, "No esp partition found: %s", CONFIG_PORT_PARTITION_NAME_FOR_XF_FAL);
        return XF_FAIL;
    }

    /* 准备默认 flash 设备 */
    /* TODO 暂未获取真实页大小 */
    sp_ctx()->flash_dev.name            = XF_FAL_DEFAULT_FLASH_DEVICE_NAME;
    sp_ctx()->flash_dev.addr            = 0;
    sp_ctx()->flash_dev.len             = _ESP_PARTITION_LEN;
    sp_ctx()->flash_dev.sector_size     = sp_ctx()->esp_partition->erase_size;
    sp_ctx()->flash_dev.page_size       = _ESP_FLASH_DEV_PAGE_SIZE;
    sp_ctx()->flash_dev.io_size         = _ESP_PARTITION_IO_SIZE;
    sp_ctx()->flash_dev.ops.init        = _esp_partition_init;
    sp_ctx()->flash_dev.ops.deinit      = _esp_partition_deinit;
    sp_ctx()->flash_dev.ops.read        = _esp_partition_read;
    sp_ctx()->flash_dev.ops.write       = _esp_partition_write;
    sp_ctx()->flash_dev.ops.erase       = _esp_partition_erase;

    xf_fal_register_flash_device(&sp_ctx()->flash_dev);

    /* 准备默认分区 */

    sp_ctx()->partition_table_cont[0].name          = XF_FAL_DEFAULT_PARTITION_NAME;
    sp_ctx()->partition_table_cont[0].flash_name    = XF_FAL_DEFAULT_FLASH_DEVICE_NAME;
    sp_ctx()->partition_table_cont[0].offset        = 0;
    sp_ctx()->partition_table_cont[0].len           = sp_ctx()->esp_partition->size;

#if _ESP_PARTITION_NUM > 0
    sp_ctx()->partition_table           = &sp_ctx()->partition_table_cont[0];
    sp_ctx()->partition_table_len       = _ESP_PARTITION_NUM;
#else
#   error "Dynamic generation of partitioned tables is required here and has not been implemented."
#endif

    xf_fal_register_partition_table(
        sp_ctx()->partition_table, sp_ctx()->partition_table_len);

    return XF_OK;
#else
    return XF_OK;
#endif
}
XF_INIT_EXPORT_BOARD(port_xf_fal);

static xf_err_t _esp_partition_init(void)
{
    return XF_OK;
}

static xf_err_t _esp_partition_deinit(void)
{
    return XF_OK;
}

static xf_err_t _esp_partition_read(size_t src_offset, void *dst, size_t size)
{
    xf_err_t xf_ret;
    pf_err_t pf_err;
    // if (src_offset + size > sizeof(_esp_partition_memory)) {
    //     return XF_FAIL;
    // }
    /* TODO src_offset 是加上分区偏移的地址，如果对接了多个分区则可能需要减除 */
    pf_err = esp_partition_read(sp_ctx()->esp_partition, src_offset, dst, size);
    xf_ret = port_convert_pf2xf_err(pf_err);
    return xf_ret;
}

static xf_err_t _esp_partition_write(size_t dst_offset, const void *src, size_t size)
{
    xf_err_t xf_ret;
    pf_err_t pf_err;
    // if (dst_offset + size > sizeof(_esp_partition_memory)) {
    //     return XF_FAIL;
    // }
    /* TODO dst_offset 是加上分区偏移的地址，如果对接了多个分区则可能需要减除 */
    pf_err = esp_partition_write(sp_ctx()->esp_partition, dst_offset, src, size);
    xf_ret = port_convert_pf2xf_err(pf_err);
    return xf_ret;
}

static xf_err_t _esp_partition_erase(size_t offset, size_t size)
{
    xf_err_t xf_ret;
    pf_err_t pf_err;
    // if (offset + size > sizeof(_esp_partition_memory)) {
    //     return XF_FAIL;
    // }
    /* TODO offset 是加上分区偏移的地址，如果对接了多个分区则可能需要减除 */
    pf_err = esp_partition_erase_range(sp_ctx()->esp_partition, offset, size);
    xf_ret = port_convert_pf2xf_err(pf_err);
    return xf_ret;
}
