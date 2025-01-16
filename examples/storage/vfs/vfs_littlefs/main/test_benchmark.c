/**
 * @file test_benchmark.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2025-01-16
 *
 * @copyright Copyright (c) 2025
 *
 */

/**
 * Copyright 2020 Brian Pugh
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * Copyright (c) 2025, CorAL.
 * This file has been modified by CorAL under the terms of the Apache License, Version 2.0.
 *
 * Modifications:
 * - Modified by CorAL on 2025-01-15:
 *   1. modified the naming to prevent conflict with the original project.
 *   2. removed esp-idf related dependencies.
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sys.h"
#include "xf_vfs.h"
#include "xf_vfs_littlefs.h"

/* ==================== [Defines] =========================================== */

#define TAG "test_benchmark"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static int get_file_size(const char *fname);

/**
 * @brief Writes and deletes files
 * @param[in] mount_pt
 * @param[in] iter Number of files to write
 */
static void read_write_test_1(const char *mount_pt, uint32_t iter);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void test_benchmark(void)
{
    XF_LOGI(TAG, "TEST_BENCHMARK");
    xf_sys_watchdog_disable();

    /*
        注册你想使用的分区，
        本例中使用平台注册的默认分区（自动初始化中注册），
        因此可以不用注册。
     */

    /* 初始化 xf_fal */
    xf_fal_init();

    /* 配置 xf_vfs_littlefs 配置 */
    xf_vfs_littlefs_conf_t conf = {
        .base_path          = "/littlefs",
        .partition_label    = XF_FAL_DEFAULT_PARTITION_NAME,
        .format_if_mount_failed = true,
        .dont_mount             = false,
    };
    /*
        使用上面定义的设置来初始化和挂载 LittleFS 文件系统。
        注意：xf_vfs_littlefs_register 是一个一体化的便利函数。
     */
    xf_err_t ret = xf_vfs_littlefs_register(&conf);
    if (ret != XF_OK) {
        if (ret == XF_FAIL) {
            XF_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == XF_ERR_NOT_FOUND) {
            XF_LOGE(TAG, "Failed to find LittleFS partition");
        } else {
            XF_LOGE(TAG, "Failed to initialize LittleFS (%s)", xf_err_to_name(ret));
        }
        return;
    }

    XF_LOGI(TAG, "TEST LittleFS:\n");
    read_write_test_1("/littlefs", 5);

    /* 全部完成，卸载分区并禁用 LittleFS */
    xf_vfs_littlefs_unregister(conf.partition_label);
}

/* ==================== [Static Functions] ================================== */

static int get_file_size(const char *fname)
{
    xf_vfs_stat_t st;
    if (0 != xf_vfs_stat(fname, &st)) {
        return -1;
    }
    return st.st_size;
}

/**
 * @brief Writes and deletes files
 * @param[in] mount_pt
 * @param[in] iter Number of files to write
 */
static void read_write_test_1(const char *mount_pt, uint32_t iter)
{
    char fmt_fn[64] = { 0 };
    char fname[128] = { 0 };
    uint64_t t_write = 0;
    uint64_t t_read = 0;
    uint64_t t_delete = 0;

    int n_write = 0;
    int n_read = 0;
    int n_delete = 0;

    strcat(fmt_fn, mount_pt);
    if (fmt_fn[xf_strlen(fmt_fn) - 1] != '/') {
        strcat(fmt_fn, "/");
    }
    strcat(fmt_fn, "%d.txt");

    const char *content = "All work and no play makes Jack a dull boy.\n";
    size_t content_len = xf_strlen(content);

    /* WRITE */
    for (uint8_t i = 0; i < iter; i++) {
        xf_snprintf(fname, sizeof(fname), fmt_fn, i);
        uint64_t t_start = xf_sys_time_get_us();
        int fd = xf_vfs_open(fname, XF_VFS_O_WRONLY | XF_VFS_O_CREAT,
                             XF_VFS_S_IRUSR | XF_VFS_S_IWUSR);
        if (fd < 0) {
            XF_LOGE(TAG, "Failed to open file %d for writing", i);
            continue;
        }
        for (uint32_t j = 0; j < 2000; j++) {
            xf_vfs_write(fd, content, content_len);
        }
        xf_vfs_close(fd);
        uint64_t t_end = xf_sys_time_get_us();
        int fsize = get_file_size(fname);
        xf_log_printf("%d bytes written in %lld us\n", fsize, (t_end - t_start));
        t_write += (t_end - t_start);
        n_write += fsize;
    }

    xf_log_printf("------------\n");

    /* READ */
    for (uint8_t i = 0; i < iter; i++) {
        int fsize = 0;
        xf_snprintf(fname, sizeof(fname), fmt_fn, i);
        uint64_t t_start = xf_sys_time_get_us();
        int fd = xf_vfs_open(fname, XF_VFS_O_RDONLY, 0);
        if (fd < 0) {
            XF_LOGE(TAG, "Failed to open file %d for reading", i);
            continue;
        }

        char ch;
        xf_vfs_ssize_t bytesRead = xf_vfs_read(fd, &ch, 1);
        while (bytesRead > 0) {
            fsize ++;
            bytesRead = xf_vfs_read(fd, &ch, 1);
        }

        xf_vfs_close(fd);
        uint64_t t_end = xf_sys_time_get_us();
        xf_log_printf("%d bytes read in %lld us\n", fsize, (t_end - t_start));
        t_read += (t_end - t_start);
        n_read += fsize;
    }

    xf_log_printf("------------\n");

    /* DELETE */
    for (uint8_t i = 0; i < iter; i++) {
        xf_snprintf(fname, sizeof(fname), fmt_fn, i);

        int fsize = get_file_size(fname);
        if (fsize < 0) {
            continue;
        }

        uint64_t t_start = xf_sys_time_get_us();
        xf_snprintf(fname, sizeof(fname), fmt_fn, i);
        xf_vfs_unlink(fname);
        uint64_t t_end = xf_sys_time_get_us();
        xf_log_printf("deleted file %d in %lld us\n", i, (t_end - t_start));
        t_delete += (t_end - t_start);
        n_delete += fsize;
    }

    xf_log_printf("------------\n");

    xf_log_printf("Total (%d) Write: %lld us\n", n_write, t_write);
    xf_log_printf("Total (%d) Read: %lld us\n", n_read, t_read);
    xf_log_printf("Total (%d) Delete: %lld us\n", n_delete, t_delete);
    xf_log_printf("\n");
}
