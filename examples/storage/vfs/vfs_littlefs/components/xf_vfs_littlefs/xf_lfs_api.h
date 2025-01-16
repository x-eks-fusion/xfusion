/**
 * @file xf_lfs_api.h
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

#ifndef __XF_LFS_API_H__
#define __XF_LFS_API_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_fal.h"
#include "xf_osal.h"

#include "xf_vfs.h"

#include "lfs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief a file descriptor
 * That's also a singly linked list used for keeping tracks of all opened file descriptor 
 *
 * Shortcomings/potential issues of 32-bit hash (when CONFIG_LITTLEFS_USE_ONLY_HASH) listed here:
 *     * unlink - If a different file is open that generates a hash collision, it will report an
 *                error that it cannot unlink an open file.
 *     * rename - If a different file is open that generates a hash collision with
 *                src or dst, it will report an error that it cannot rename an open file.
 * Potential consequences:
 *    1. A file cannot be deleted while a collision-geneating file is open.
 *       Worst-case, if the other file is always open during the lifecycle
 *       of your app, it's collision file cannot be deleted, which in the 
 *       worst-case could cause storage-capacity issues.
 *    2. Same as (1), but for renames
 */
typedef struct _vfs_littlefs_file_t {
    lfs_file_t file;
    uint32_t   hash;
    struct _vfs_littlefs_file_t * next;       /*!< Pointer to next file in Singly Linked List */
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
    char     * path;
#endif
} vfs_littlefs_file_t;

/**
 * @brief littlefs definition structure
 */
typedef struct {
    lfs_t *fs;                                /*!< Handle to the underlying littlefs */
    /* 此处原本是信号量，xf_osal 信号量不支持递归，改为互斥锁 */
    xf_osal_mutex_t lock;                       /*!< 要求递归互斥锁  */

    const xf_fal_partition_t* partition;         /*!< The partition on which littlefs is located */
    char base_path[XF_VFS_PATH_MAX+1];       /*!< Mount point */

    struct lfs_config cfg;                    /*!< littlefs Mount configuration */

    vfs_littlefs_file_t *file;                /*!< Singly Linked List of files */

    vfs_littlefs_file_t **cache;              /*!< A cache of pointers to the opened files */
    uint16_t             cache_size;          /*!< The cache allocated size (in pointers) */
    uint16_t             fd_count;            /*!< The count of opened file descriptor used to speed up computation */
    bool                 read_only;           /*!< Filesystem is read-only */
} xf_littlefs_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief Read a region in a block.
 *
 * Negative error codes are propogated to the user.
 *
 * @return errorcode. 0 on success.
 */
int littlefs_xf_part_read(const struct lfs_config *c, lfs_block_t block,
                           lfs_off_t off, void *buffer, lfs_size_t size);

/**
 * @brief Program a region in a block.
 *
 * The block must have previously been erased. 
 * Negative error codes are propogated to the user.
 * May return LFS_ERR_CORRUPT if the block should be considered bad.
 *
 * @return errorcode. 0 on success.
 */
int littlefs_xf_part_write(const struct lfs_config *c, lfs_block_t block,
                            lfs_off_t off, const void *buffer, lfs_size_t size);

/**
 * @brief Erase a block.
 *
 * A block must be erased before being programmed.
 * The state of an erased block is undefined.
 * Negative error codes are propogated to the user.
 * May return LFS_ERR_CORRUPT if the block should be considered bad.
 * @return errorcode. 0 on success.
 */
int littlefs_xf_part_erase(const struct lfs_config *c, lfs_block_t block);

/**
 * @brief Sync the state of the underlying block device.
 *
 * Negative error codes are propogated to the user.
 *
 * @return errorcode. 0 on success.
 */
int littlefs_xf_part_sync(const struct lfs_config *c);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_LFS_API_H__ */
