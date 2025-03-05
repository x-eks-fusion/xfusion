/**
 * @file port_xf_ota_client.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-12-19
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "string.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_flash_partitions.h"
#include "esp_partition.h"

#include "esp_app_desc.h"
#include "bootloader_common.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "xf_ota_client.h"

/* ==================== [Defines] =========================================== */

#define HASH_LEN 32 /* SHA-256 digest length */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

static const char *const TAG = "port_xf_ota_client";

/* ==================== [Macros] ============================================ */

#if !defined(min)
#   define min(x, y)                (((x) < (y)) ? (x) : (y))
#endif

/* ==================== [Global Functions] ================================== */

xf_err_t xf_ota_init(void)
{
    return XF_OK;
}

xf_err_t xf_ota_deinit(void)
{
    return XF_OK;
}

xf_err_t xf_ota_get_running_partition(xf_ota_partition_t *p_part_hdl)
{
    XF_CHECK(NULL == p_part_hdl, XF_ERR_INVALID_ARG,
             TAG, "p_part_hdl:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    p_part_hdl->platform_data = (void *)esp_ota_get_running_partition();
    p_part_hdl->is_xf_fal_part = false;
    return XF_OK;
}

xf_err_t xf_ota_get_next_update_partition(
    xf_ota_partition_t *p_start_part_hdl, xf_ota_partition_t *p_next_part_hdl)
{
    const esp_partition_t *start_from = NULL;
    XF_CHECK(NULL == p_next_part_hdl, XF_ERR_INVALID_ARG,
             TAG, "p_next_part_hdl:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    if (p_start_part_hdl) {
        start_from = (const esp_partition_t *)p_start_part_hdl->platform_data;
    }
    p_next_part_hdl->platform_data = (void *)esp_ota_get_next_update_partition(start_from);
    p_next_part_hdl->is_xf_fal_part = false;
    if (NULL == p_next_part_hdl->platform_data) {
        return XF_FAIL;
    }
    return XF_OK;
}

xf_err_t xf_ota_get_partition_info(
    const xf_ota_partition_t *p_part_hdl, xf_ota_partition_info_t *p_info)
{
    const esp_partition_t *esp_partition = NULL;
    XF_CHECK(NULL == p_part_hdl, XF_ERR_INVALID_ARG,
             TAG, "p_part_hdl:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    XF_CHECK(NULL == p_part_hdl->platform_data, XF_ERR_INVALID_ARG,
             TAG, "platform_data:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    XF_CHECK(NULL == p_info, XF_ERR_INVALID_ARG,
             TAG, "p_info:%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    esp_partition = (const esp_partition_t *)p_part_hdl->platform_data;
    p_info->unit_size = 1;  /*!< 可以以 1 个字节为单位写入 */
    p_info->size = esp_partition->size;

    return XF_OK;
}

size_t xf_ota_get_platform_app_desc_size(void)
{
    return sizeof(esp_app_desc_t);
}

xf_err_t xf_ota_get_platform_app_desc_block(
    xf_ota_partition_t *p_part_hdl, void *p_desc_out, size_t buff_size)
{
    const esp_partition_t *esp_partition = NULL;
    esp_err_t esp_ret;
    esp_app_desc_t esp_app_desc = {0};
    size_t cpy_size = 0;

    XF_CHECK(NULL == p_part_hdl, XF_ERR_INVALID_ARG,
             TAG, "p_part_hdl:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    XF_CHECK(NULL == p_part_hdl->platform_data, XF_ERR_INVALID_ARG,
             TAG, "platform_data:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    XF_CHECK(NULL == p_desc_out, XF_ERR_INVALID_ARG,
             TAG, "p_desc_out:%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    esp_partition = (const esp_partition_t *)p_part_hdl->platform_data;
    esp_ret = esp_ota_get_partition_description(esp_partition, &esp_app_desc);
    if (esp_ret != ESP_OK) {
        return XF_FAIL;
    }
    cpy_size = min(buff_size, sizeof(esp_app_desc_t));
    xf_memcpy((char *)p_desc_out, &esp_app_desc, cpy_size);

    return XF_OK;
}

size_t xf_ota_get_platform_app_digest_size(void)
{
    return HASH_LEN;
}

xf_err_t xf_ota_get_platform_app_digest_block(
    xf_ota_partition_t *p_part_hdl, void *p_digest_out, size_t buff_size)
{
    const esp_partition_t *esp_partition = NULL;
    esp_err_t esp_ret;
    uint8_t sha_256[HASH_LEN] = {0};
    size_t cpy_size = 0;

    XF_CHECK(NULL == p_part_hdl, XF_ERR_INVALID_ARG,
             TAG, "p_part_hdl:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    XF_CHECK(NULL == p_part_hdl->platform_data, XF_ERR_INVALID_ARG,
             TAG, "platform_data:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    XF_CHECK(NULL == p_digest_out, XF_ERR_INVALID_ARG,
             TAG, "p_desc_out:%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    esp_partition = (const esp_partition_t *)p_part_hdl->platform_data;
    esp_ret = esp_partition_get_sha256(esp_partition, sha_256);
    if (esp_ret != ESP_OK) {
        return XF_FAIL;
    }
    cpy_size = min(buff_size, HASH_LEN);
    xf_memcpy((char *)p_digest_out, sha_256, cpy_size);

    return XF_OK;
}

// xf_err_t xf_ota_get_xf_app_desc(
//     xf_ota_partition_t *p_part_hdl, xf_app_desc_t *p_desc_out);

xf_err_t xf_ota_start(
    xf_ota_partition_t *p_part_hdl,
    uint32_t package_len, bool sequential_write, xf_ota_t *p_hdl)
{
    const esp_partition_t *esp_partition = NULL;
    esp_err_t esp_ret;
    size_t image_size = 0;
    esp_ota_handle_t update_handle = {0};

    XF_CHECK(NULL == p_part_hdl, XF_ERR_INVALID_ARG,
             TAG, "p_part_hdl:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    XF_CHECK(NULL == p_part_hdl->platform_data, XF_ERR_INVALID_ARG,
             TAG, "platform_data:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    XF_CHECK(NULL == p_hdl, XF_ERR_INVALID_ARG,
             TAG, "p_hdl:%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    esp_partition = (const esp_partition_t *)p_part_hdl->platform_data;
    if (sequential_write) {
        image_size = OTA_WITH_SEQUENTIAL_WRITES;
    } else {
        image_size = package_len;
    }

    esp_ret = esp_ota_begin(esp_partition, image_size, &update_handle);
    if (esp_ret != ESP_OK) {
        return XF_FAIL;
    }
    p_hdl->platform_data = (void *)(uintptr_t)update_handle;

    return XF_OK;
}

xf_err_t xf_ota_abort(xf_ota_t *p_hdl)
{
    esp_err_t esp_ret;
    esp_ota_handle_t update_handle = 0;

    XF_CHECK(NULL == p_hdl, XF_ERR_INVALID_ARG,
             TAG, "p_hdl:%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    update_handle = (esp_ota_handle_t)(uintptr_t)p_hdl->platform_data;
    esp_ret = esp_ota_abort(update_handle);
    if (esp_ret != ESP_OK) {
        return XF_FAIL;
    }
    p_hdl->platform_data = NULL;

    return XF_OK;
}

xf_err_t xf_ota_end(xf_ota_t *p_hdl)
{
    esp_err_t esp_ret;
    esp_ota_handle_t update_handle = 0;

    XF_CHECK(NULL == p_hdl, XF_ERR_INVALID_ARG,
             TAG, "p_hdl:%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    update_handle = (esp_ota_handle_t)(uintptr_t)p_hdl->platform_data;
    esp_ret = esp_ota_end(update_handle);
    if (esp_ret != ESP_OK) {
        switch (esp_ret) {
        case ESP_ERR_OTA_VALIDATE_FAILED:
            return XF_ERR_INVALID_CHECK;
            break;
        case ESP_ERR_NOT_FOUND:
        case ESP_ERR_INVALID_ARG:
        default:
            return XF_FAIL;
            break;
        }
    }
    p_hdl->platform_data = NULL;

    return XF_OK;
}

xf_err_t xf_ota_upgrade(xf_ota_partition_t *p_part_hdl, bool reboot)
{
    const esp_partition_t *esp_partition = NULL;
    esp_err_t esp_ret;

    XF_CHECK(NULL == p_part_hdl, XF_ERR_INVALID_ARG,
             TAG, "p_part_hdl:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    XF_CHECK(NULL == p_part_hdl->platform_data, XF_ERR_INVALID_ARG,
             TAG, "platform_data:%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    esp_partition = (const esp_partition_t *)p_part_hdl->platform_data;
    esp_ret = esp_ota_set_boot_partition(esp_partition);
    if (esp_ret != ESP_OK) {
        return XF_FAIL;
    }

    if (reboot) {
        esp_restart();
    }

    return XF_OK;
}

xf_err_t xf_ota_write(xf_ota_t *p_hdl, const void *src, size_t size)
{
    esp_err_t esp_ret;
    esp_ota_handle_t update_handle = 0;

    XF_CHECK(NULL == p_hdl, XF_ERR_INVALID_ARG,
             TAG, "p_hdl:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    XF_CHECK(NULL == p_hdl->platform_data, XF_ERR_INVALID_ARG,
             TAG, "platform_data:%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    update_handle = (esp_ota_handle_t)(uintptr_t)p_hdl->platform_data;
    esp_ret = esp_ota_write(update_handle, src, size);
    if (esp_ret != ESP_OK) {
        return XF_FAIL;
    }

    return XF_OK;
}

xf_err_t xf_ota_write_to(
    xf_ota_t *p_hdl, size_t dst_offset, const void *src, size_t size)

{
    esp_err_t esp_ret;
    esp_ota_handle_t update_handle = 0;

    XF_CHECK(NULL == p_hdl, XF_ERR_INVALID_ARG,
             TAG, "p_hdl:%s", xf_err_to_name(XF_ERR_INVALID_ARG));
    XF_CHECK(NULL == p_hdl->platform_data, XF_ERR_INVALID_ARG,
             TAG, "platform_data:%s", xf_err_to_name(XF_ERR_INVALID_ARG));

    update_handle = (esp_ota_handle_t)(uintptr_t)p_hdl->platform_data;
    esp_ret = esp_ota_write_with_offset(update_handle, src, size, (uint32_t)dst_offset);
    if (esp_ret != ESP_OK) {
        return XF_FAIL;
    }

    return XF_OK;
}

/* ==================== [Static Functions] ================================== */
