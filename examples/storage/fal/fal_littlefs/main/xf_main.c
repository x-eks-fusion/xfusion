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
 * @example{lineno} examples/storage/fal/fal_littlefs/main/xf_main.c
 * 基于xf_fal 的 littlefs 示例。
 */

/* ==================== [Includes] ========================================== */

#include "lfs.h"

#include "xf_lfs_by_fal.h"

/* ==================== [Defines] =========================================== */

#define TAG "littlefs_example"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

#define CHECK_LFS_ERR(_lfs_err) XF_CHECK((_lfs_err) != 0, XF_RETURN_VOID, TAG, "An error occurred.");

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_err_t    xf_ret;
    lfs_t      *p_lfs;
    lfs_file_t  lfs_file    = {0};
    lfs_file_t *p_lfs_file  = &lfs_file;
    int         lfs_err = 0;

    /* 初始化 */
    xf_ret = xf_lfs_init(NULL, true);
    if (xf_ret != XF_OK) {
        XF_LOGI(TAG, "xf_lfs init failed: %s", xf_err_to_name(xf_ret));
        return;
    }
    /* 获取 littlefs 句柄 */
    p_lfs = xf_lfs_get_lfs_handle();

    uint32_t boot_count = 0;
    lfs_err = lfs_file_open(p_lfs, p_lfs_file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    CHECK_LFS_ERR(lfs_err);
    lfs_file_read(p_lfs, p_lfs_file, &boot_count, sizeof(boot_count));
    /* 更新计数 */
    boot_count += 1;
    lfs_err = lfs_file_rewind(p_lfs, p_lfs_file);  // seek the file to begin
    CHECK_LFS_ERR(lfs_err);
    lfs_file_write(p_lfs, p_lfs_file, &boot_count, sizeof(boot_count));
    /* 在文件成功关闭之前，存储不会更新 */
    lfs_err = lfs_file_close(p_lfs, p_lfs_file);
    CHECK_LFS_ERR(lfs_err);

    XF_LOGI(TAG, "boot_count: %u", (unsigned int)boot_count);
}

/* ==================== [Static Functions] ================================== */
