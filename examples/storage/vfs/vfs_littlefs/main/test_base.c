/**
 * @file test_base.c
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
#include "xf_osal.h"
#include "xf_vfs.h"
#include "xf_vfs_littlefs.h"

/* ==================== [Defines] =========================================== */

#define TAG "test_base"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void test_base(void)
{
    /*
        注册你想使用的分区，
        本例中使用平台注册的默认分区（自动初始化中注册），
        因此可以不用注册。
     */

    /* 初始化 xf_fal */
    xf_fal_init();

    XF_LOGI(TAG, "Initializing LittleFS");

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

    size_t total = 0, used = 0;
    ret = xf_littlefs_info(conf.partition_label, &total, &used);
    if (ret != XF_OK) {
        XF_LOGE(TAG, "Failed to get LittleFS partition information (%s)", xf_err_to_name(ret));
    } else {
        XF_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    /* 使用 xf_vfs 接口来处理文件 */
    XF_LOGI(TAG, "Opening file");
    int fd = xf_vfs_open("/littlefs/hello.txt", XF_VFS_O_WRONLY | XF_VFS_O_CREAT | XF_VFS_O_TRUNC,
                         XF_VFS_S_IRUSR | XF_VFS_S_IWUSR);
    if (fd < 0) {
        XF_LOGE(TAG, "Failed to open file for writing");
        return;
    }

    const char *content = "LittleFS Rocks!\n";
    if (xf_vfs_write(fd, content, strlen(content)) < 0) {
        XF_LOGE(TAG, "Failed to write to file");
        xf_vfs_close(fd);
        return;
    }
    xf_vfs_close(fd);
    XF_LOGI(TAG, "File written");

    /* 重命名之前检查目标文件是否存在 */
    xf_vfs_stat_t st;
    if (xf_vfs_stat("/littlefs/foo.txt", &st) == 0) {
        /* 如果存在则删除 */
        if (xf_vfs_unlink("/littlefs/foo.txt") != 0) {
            XF_LOGE(TAG, "Failed to delete existing file");
            return;
        }
    }

    /* 重命名原始文件 */
    XF_LOGI(TAG, "Renaming file");
    if (xf_vfs_rename("/littlefs/hello.txt", "/littlefs/foo.txt") != 0) {
        XF_LOGE(TAG, "Rename failed");
        return;
    }

    /* 打开重命名的文件进行读取 */
    XF_LOGI(TAG, "Reading file");
    fd = xf_vfs_open("/littlefs/foo.txt", XF_VFS_O_RDONLY, 0);
    if (fd < 0) {
        XF_LOGE(TAG, "Failed to open file for reading");
        return;
    }

    char line[128];
    ssize_t bytesRead = xf_vfs_read(fd, line, sizeof(line) - 1);
    xf_vfs_close(fd);
    if (bytesRead < 0) {
        XF_LOGE(TAG, "Failed to read from file");
        return;
    }
    line[bytesRead] = '\0'; // 确保字符串以空字符结尾
    char *pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    XF_LOGI(TAG, "Read from file: '%s'", line);

    /* 全部完成，卸载分区并禁用 LittleFS */
    xf_vfs_littlefs_unregister(conf.partition_label);
    XF_LOGI(TAG, "LittleFS unmounted");
}

/* ==================== [Static Functions] ================================== */
