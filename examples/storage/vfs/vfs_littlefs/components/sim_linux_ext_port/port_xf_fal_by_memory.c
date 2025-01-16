/**
 * @file xf_fal_port.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include <stdio.h>
#include <string.h>

#include "xf_fal.h"
#include "xf_init.h"

/* ==================== [Defines] =========================================== */

#define MOCK_FLASH_NAME                 XF_FAL_DEFAULT_FLASH_DEVICE_NAME
#define MOCK_FLASH_START_ADDR           (0)
#define MOCK_FLASH_LEN                  (8 * 1024 * 1024)
#define MOCK_FLASH_SECTOR_SIZE          (4 * 1024)
#define MOCK_FLASH_PAGE_SIZE            (256)
#define MOCK_FLASH_IO_SIZE              (1)

#define MOCK_FLASH_PART_TABLE                                           \
    {                                                                   \
        {XF_FAL_DEFAULT_PARTITION_NAME, XF_FAL_DEFAULT_FLASH_DEVICE_NAME, 0, MOCK_FLASH_LEN},     \
    }

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/**
 * @name mock_flash_ops
 * @brief mock_flash 的操作。
 * @{
 */
static xf_err_t mock_flash_init(void);
static xf_err_t mock_flash_deinit(void);
static xf_err_t mock_flash_read(size_t src_offset, void *dst, size_t size);
static xf_err_t mock_flash_write(size_t dst_offset, const void *src, size_t size);
static xf_err_t mock_flash_erase(size_t offset, size_t size);
/**
 * End of mock_flash_ops
 * @}
 */

/* ==================== [Static Variables] ================================== */

/**
 * @brief 用于模拟 flash 的内存。
 * 提供给 mock_flash_read... 函数操作。可以换成文件。
 */
static uint8_t mock_flash_memory[MOCK_FLASH_LEN] = {0};

/**
 * @brief mock flash 设备描述。
 * 用于 Linux 上模拟 flash 的行为。
 */
static const xf_fal_flash_dev_t mock_flash_dev = {
    .name           = MOCK_FLASH_NAME,
    .addr           = MOCK_FLASH_START_ADDR,
    .len            = MOCK_FLASH_LEN,
    .sector_size    = MOCK_FLASH_SECTOR_SIZE,
    .page_size      = MOCK_FLASH_PAGE_SIZE,
    .io_size        = MOCK_FLASH_IO_SIZE,
    .ops.init       = mock_flash_init,
    .ops.deinit     = mock_flash_deinit,
    .ops.read       = mock_flash_read,
    .ops.write      = mock_flash_write,
    .ops.erase      = mock_flash_erase,
};

/**
 * @brief 提供给 xf_fal 使用的分区设备表。
 *
 * 描述了各分区的分区名、分区在哪个 flash、分区在 flash 上的偏移地址和长度。
 *
 * @note @b 可以 是常量, xf_fal 不会修改其中内容。
 */
static const xf_fal_partition_t mock_flash_partition_table[] = MOCK_FLASH_PART_TABLE;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

int mock_flash_register_to_xf_fal(void)
{
    xf_fal_register_flash_device(&mock_flash_dev);
    xf_fal_register_partition_table(
        mock_flash_partition_table, ARRAY_SIZE(mock_flash_partition_table));
    return 0;
}
XF_INIT_EXPORT_SETUP(mock_flash_register_to_xf_fal);

/* ==================== [Static Functions] ================================== */

static xf_err_t mock_flash_init(void)
{
    xf_log_printf("Mock flash initialized.\n");
    memset(mock_flash_memory, 0xFF, sizeof(mock_flash_memory));
    return XF_OK;
}

static xf_err_t mock_flash_deinit(void)
{
    xf_log_printf("Mock flash deinitialized.\n");
    // memset(mock_flash_memory, 0xFF, sizeof(mock_flash_memory));
    return XF_OK;
}

static xf_err_t mock_flash_read(size_t src_offset, void *dst, size_t size)
{
    if (src_offset + size > sizeof(mock_flash_memory)) {
        return XF_FAIL;
    }
    xf_memcpy(dst, &mock_flash_memory[mock_flash_dev.addr + src_offset], size);
    return XF_OK;
}

static xf_err_t mock_flash_write(size_t dst_offset, const void *src, size_t size)
{
    if (dst_offset + size > sizeof(mock_flash_memory)) {
        return XF_FAIL;
    }

    /* 不使用 memcpy */
    // memcpy(&mock_flash_memory[dst_offset], src, size);

    uint8_t *dst_u8;
    const uint8_t *src_u8;
    size_t i;

    /* 模拟 nor flash 的行为 */
    dst_u8 = mock_flash_memory + mock_flash_dev.addr + dst_offset;
    src_u8 = src;
    for (i = 0; i < size; i++) {
        *(dst_u8 + i) &= *(src_u8 + i);
    }
    return XF_OK;
}

static xf_err_t mock_flash_erase(size_t offset, size_t size)
{
    if (offset + size > sizeof(mock_flash_memory)) {
        return XF_FAIL;
    }
    /* TODO 对齐到扇区大小擦除 */
    // if ((offset % mock_flash_dev.sector_size != 0)
    //         || ((size % mock_flash_dev.sector_size != 0))) {
    //     return XF_FAIL;
    // }
    xf_memset(&mock_flash_memory[mock_flash_dev.addr + offset], 0xFF, size);
    return XF_OK;
}
