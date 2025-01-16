/**
 * @file xf_vfs_littlefs.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief 
 * @version 1.0
 * @date 2025-01-15
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

#ifndef __XF_VFS_LITTLEFS_H__
#define __XF_VFS_LITTLEFS_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sys.h"
#include "xf_vfs.h"
#include "xf_fal.h"
#include "xf_lfs_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_LITTLEFS_TAG "xf_littlefs"

#define XF_LITTLEFS_ENABLE_FTRUNCATE

/* ==================== [Typedefs] ========================================== */

/**
 *Configuration structure for xf_vfs_littlefs_register.
 */
typedef struct {
    const char *base_path;            /**< Mounting point. */
    const char *partition_label;      /**< Label of partition to use. */
    const xf_fal_partition_t* partition; /**< partition to use if partition_label is NULL */

    uint8_t format_if_mount_failed:1; /**< Format the file system if it fails to mount. */
    uint8_t read_only : 1;            /**< Mount the partition as read-only. */
    uint8_t dont_mount:1;             /**< Don't attempt to mount.*/
    uint8_t grow_on_mount:1;          /**< Grow filesystem to match partition size on mount.*/
} xf_vfs_littlefs_conf_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * Register and mount (if configured to) littlefs to VFS with given path prefix.
 *
 * @param   conf                      Pointer to xf_vfs_littlefs_conf_t configuration structure
 *
 * @return  
 *          - XF_OK                  if success
 *          - XF_ERR_NO_MEM          if objects could not be allocated
 *          - XF_ERR_INVALID_STATE   if already mounted or partition is encrypted
 *          - XF_ERR_NOT_FOUND       if partition for littlefs was not found
 *          - XF_FAIL                if mount or format fails
 */
xf_err_t xf_vfs_littlefs_register(const xf_vfs_littlefs_conf_t * conf);

/**
 * Unregister and unmount littlefs from VFS
 *
 * @param partition_label  Label of the partition to unregister.
 *
 * @return  
 *          - XF_OK if successful
 *          - XF_ERR_INVALID_STATE already unregistered
 */
xf_err_t xf_vfs_littlefs_unregister(const char* partition_label);

/**
 * Unregister and unmount littlefs from VFS
 *
 * @param partition  partition to unregister.
 *
 * @return  
 *          - XF_OK if successful
 *          - XF_ERR_INVALID_STATE already unregistered
 */
xf_err_t xf_vfs_littlefs_unregister_partition(const xf_fal_partition_t* partition);

/**
 * Check if littlefs is mounted
 *
 * @param partition_label  Label of the partition to check.
 *
 * @return  
 *          - true    if mounted
 *          - false   if not mounted
 */
bool xf_littlefs_mounted(const char* partition_label);

/**
 * Check if littlefs is mounted
 *
 * @param partition  partition to check.
 *
 * @return  
 *          - true    if mounted
 *          - false   if not mounted
 */
bool xf_littlefs_partition_mounted(const xf_fal_partition_t* partition);

/**
 * Format the littlefs partition
 *
 * @param partition_label  Label of the partition to format.
 * @return  
 *          - XF_OK      if successful
 *          - XF_FAIL    on error
 */
xf_err_t xf_littlefs_format(const char* partition_label);

/**
 * Format the littlefs partition
 *
 * @param partition  partition to format.
 * @return
 *          - XF_OK      if successful
 *          - XF_FAIL    on error
 */
xf_err_t xf_littlefs_format_partition(const xf_fal_partition_t* partition);

/**
 * Get information for littlefs
 *
 * @param partition_label           Optional, label of the partition to get info for.
 * @param[out] total_bytes          Size of the file system
 * @param[out] used_bytes           Current used bytes in the file system
 *
 * @return
 *          - XF_OK                  if success
 *          - XF_ERR_INVALID_STATE   if not mounted
 */
xf_err_t xf_littlefs_info(const char* partition_label, size_t* total_bytes, size_t* used_bytes);

/**
 * Get information for littlefs
 *
 * @param parition                  the partition to get info for.
 * @param[out] total_bytes          Size of the file system
 * @param[out] used_bytes           Current used bytes in the file system
 *
 * @return  
 *          - XF_OK                  if success
 *          - XF_ERR_INVALID_STATE   if not mounted
 */
xf_err_t xf_littlefs_partition_info(const xf_fal_partition_t* partition, size_t *total_bytes, size_t *used_bytes);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_VFS_LITTLEFS_H__ */
