/**
 * @file xf_main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-23
 *
 * Copyright (c) 2023, CorAL. All rights reserved.
 *
 */

/**
 * @example{lineno} examples/storage/fal/fal_base/main/xf_main.c
 * xf_fal 基础示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_fal.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

#define TAG "xf_fal_example"

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    uint8_t read_buf[16] = {0};
    uint8_t write_buf[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    xf_err_t xf_ret;

    /*
        注册 xf_fal
        用户可以忽略
        已经在对接层做了，这里无需操作，
        对接部分可见 @ref ports/espressif/esp32/port_fal.c
     */

    /* 初始化 FAL */
    xf_ret = xf_fal_init();
    if (xf_ret != XF_OK) {
        XF_LOGI(TAG, "FAL init failed: %d", xf_ret);
        return;
    }

    /* 打印分区表 */
    xf_fal_show_part_table();

    /* 获取分区 */
    const xf_fal_partition_t *part = xf_fal_partition_find(XF_FAL_DEFAULT_PARTITION_NAME);
    if (!part) {
        XF_LOGI(TAG, "Partition not found!");
        return;
    }

    /* 写入之前要擦除数据 */
    xf_ret = xf_fal_partition_erase(part, 0, part->len);
    if (xf_ret != XF_OK) {
        XF_LOGI(TAG, "Partition erase failed: %d", xf_ret);
        return;
    }

    /* 读取数据并打印 */
    xf_ret = xf_fal_partition_read(part, 0, read_buf, sizeof(read_buf));
    if (xf_ret == XF_OK) {
        XF_LOGI(TAG, "The partition was successfully. Data: ");
        for (size_t i = 0; i < sizeof(read_buf); i++) {
            xf_log_printf("%02X ", read_buf[i]);
        }
        xf_log_printf("\n");
    } else {
        XF_LOGI(TAG, "Partition read after erase failed: %d", xf_ret);
    }

    /* 写入数据到分区 */
    xf_ret = xf_fal_partition_write(part, 0, write_buf, sizeof(write_buf));
    if (xf_ret != XF_OK) {
        XF_LOGI(TAG, "Partition write failed: %d", xf_ret);
        return;
    }

    /* 读取数据并打印 */
    xf_ret = xf_fal_partition_read(part, 0, read_buf, sizeof(read_buf));
    if (xf_ret == XF_OK) {
        XF_LOGI(TAG, "Partition read successful! Data: ");
        for (size_t i = 0; i < sizeof(read_buf); i++) {
            xf_log_printf("%02X ", read_buf[i]);
        }
        xf_log_printf("\n");
    } else {
        XF_LOGI(TAG, "Partition read failed: %d", xf_ret);
    }

    /*
        擦除数据
        数据只能以扇区为单位擦除，此处先获取 flash 设备查询扇区大小
     */
    const xf_fal_flash_dev_t *flash_dev = xf_fal_flash_device_find_by_part(part);
    if (NULL == flash_dev) {
        XF_LOGI(TAG, "flash device not found.");
        return;
    }

    xf_ret = xf_fal_partition_erase(part, 0, flash_dev->sector_size);
    if (xf_ret != XF_OK) {
        XF_LOGI(TAG, "Partition erase failed: %d", xf_ret);
        return;
    }

    /* 再次读取数据并打印 */
    xf_ret = xf_fal_partition_read(part, 0, read_buf, sizeof(read_buf));
    if (xf_ret == XF_OK) {
        XF_LOGI(TAG, "After erase, data: ");
        for (size_t i = 0; i < sizeof(read_buf); i++) {
            xf_log_printf("%02X ", read_buf[i]);
        }
        xf_log_printf("\n");
    } else {
        XF_LOGI(TAG, "Partition read after erase failed: %d", xf_ret);
    }
    XF_LOGI(TAG, "Done!");
}

/* ==================== [Static Functions] ================================== */
