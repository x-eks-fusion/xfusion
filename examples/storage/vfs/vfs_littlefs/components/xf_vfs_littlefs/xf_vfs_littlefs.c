/**
 * @file xf_vfs_littlefs.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2025-01-15
 */

/**
 * Copyright 2020 Brian Pugh
 *
 * Permission is hereby granted, xf_free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
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

/* 不要格式化，文件太长，格式化后会出错。 */
/* *INDENT-OFF* */

/* ==================== [Includes] ========================================== */

#include "xf_vfs_littlefs.h"
#include "xf_lfs_api.h"

#include "xf_osal.h"

#include "littlefs/lfs.h"

/* ==================== [Defines] =========================================== */

#define MAXPATHLEN 256

/* File Descriptor Caching Params */
#define CONFIG_LITTLEFS_FD_CACHE_REALLOC_FACTOR 2  /* Amount to resize FD cache by */
#define CONFIG_LITTLEFS_FD_CACHE_MIN_SIZE 4  /* Minimum size of FD cache */
#define CONFIG_LITTLEFS_FD_CACHE_HYST 4  /* When shrinking, leave this many trailing FD slots available */

/**
 * @brief Last Modified Time
 *
 * Use 't' for LITTLEFS_ATTR_MTIME to match example:
 *     https://github.com/ARMmbed/littlefs/issues/23#issuecomment-482293539
 * And to match other external tools such as:
 *     https://github.com/earlephilhower/mklittlefs
 */
#define LITTLEFS_ATTR_MTIME ((uint8_t) 't')

/* ==================== [Typedefs] ========================================== */

/**
 * @brief littlefs xf_vfs_dir_t structure
 */
typedef struct {
    xf_vfs_dir_t dir;            /*!< VFS xf_vfs_dir_t struct */
    lfs_dir_t d;        /*!< littlefs xf_vfs_dir_t struct */
    xf_vfs_dirent_t e;    /*!< Last open dirent */
    long offset;        /*!< Offset of the current dirent */
    char *path;         /*!< Requested directory name */
} vfs_littlefs_dir_t;

/* ==================== [Static Prototypes] ================================= */

static int       vfs_littlefs_open(void *ctx, const char *path, int flags, int mode);
static xf_vfs_ssize_t   vfs_littlefs_write(void *ctx, int fd, const void *data, size_t size);
static xf_vfs_ssize_t   vfs_littlefs_read(void *ctx, int fd, void *dst, size_t size);
static xf_vfs_ssize_t   vfs_littlefs_pwrite(void *ctx, int fd, const void *src, size_t size, xf_vfs_off_t offset);
static xf_vfs_ssize_t   vfs_littlefs_pread(void *ctx, int fd, void *dst, size_t size, xf_vfs_off_t offset);
static int       vfs_littlefs_close(void *ctx, int fd);
static xf_vfs_off_t     vfs_littlefs_lseek(void *ctx, int fd, xf_vfs_off_t offset, int mode);
static int       vfs_littlefs_fsync(void *ctx, int fd);
static xf_vfs_t vfs_littlefs_create_struct(bool writeable);

#ifdef XF_VFS_SUPPORT_DIR_IS_ENABLE
static int     vfs_littlefs_stat(void *ctx, const char *path, xf_vfs_stat_t *st);
static int     vfs_littlefs_unlink(void *ctx, const char *path);
static int     vfs_littlefs_rename(void *ctx, const char *src, const char *dst);
static xf_vfs_dir_t    *vfs_littlefs_opendir(void *ctx, const char *name);
static int     vfs_littlefs_closedir(void *ctx, xf_vfs_dir_t *pdir);
static xf_vfs_dirent_t *vfs_littlefs_readdir(void *ctx, xf_vfs_dir_t *pdir);
static int     vfs_littlefs_readdir_r(void *ctx, xf_vfs_dir_t *pdir,
                                      xf_vfs_dirent_t *entry, xf_vfs_dirent_t **out_dirent);
static long    vfs_littlefs_telldir(void *ctx, xf_vfs_dir_t *pdir);
static void    vfs_littlefs_seekdir(void *ctx, xf_vfs_dir_t *pdir, long offset);
static int     vfs_littlefs_mkdir(void *ctx, const char *name, xf_vfs_mode_t mode);
static int     vfs_littlefs_rmdir(void *ctx, const char *name);
static xf_vfs_ssize_t vfs_littlefs_truncate(void *ctx, const char *path, xf_vfs_off_t size);

#ifdef XF_LITTLEFS_ENABLE_FTRUNCATE
static int vfs_littlefs_ftruncate(void *ctx, int fd, xf_vfs_off_t size);
#endif // XF_LITTLEFS_ENABLE_FTRUNCATE

static void      xf_littlefs_dir_free(vfs_littlefs_dir_t *dir);
#endif

static void      xf_littlefs_take_efs_lock(void);
static xf_err_t xf_littlefs_init_efs(xf_littlefs_t **efs, const xf_fal_partition_t *partition, bool read_only);
static xf_err_t xf_littlefs_init(const xf_vfs_littlefs_conf_t *conf);

static xf_err_t xf_littlefs_by_label(const char *label, int *index);
static xf_err_t xf_littlefs_by_partition(const xf_fal_partition_t *part, int *index);

static xf_err_t xf_littlefs_get_empty(int *index);
static void      xf_littlefs_free(xf_littlefs_t **efs);
static int       xf_littlefs_flags_conv(int m);

#if CONFIG_LITTLEFS_USE_MTIME
static int       vfs_littlefs_utime(void *ctx, const char *path, const xf_vfs_utimbuf_t *times);
static void      vfs_littlefs_update_mtime(xf_littlefs_t *efs, const char *path);
static int       vfs_littlefs_update_mtime_value(xf_littlefs_t *efs, const char *path, xf_vfs_time_t t);
static xf_vfs_time_t    vfs_littlefs_get_mtime(xf_littlefs_t *efs, const char *path);
#endif

#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
/* The only way in LittleFS to get info is via a path (lfs_stat), so it cannot
 * be done if the path isn't stored. */
static int     vfs_littlefs_fstat(void *ctx, int fd, xf_vfs_stat_t *st);
#endif

#if CONFIG_LITTLEFS_SPIFFS_COMPAT
static void mkdirs(xf_littlefs_t *efs, const char *dir);
static void rmdirs(xf_littlefs_t *efs, const char *dir);
#endif  // CONFIG_LITTLEFS_SPIFFS_COMPAT

static int vfs_littlefs_fcntl(void *ctx, int fd, int cmd, int arg);

static int sem_take(xf_littlefs_t *efs);
static int sem_give(xf_littlefs_t *efs);
static xf_err_t format_from_efs(xf_littlefs_t *efs);
static void get_total_and_used_bytes(xf_littlefs_t *efs, size_t *total_bytes, size_t *used_bytes);

/********************
 * Helper Functions *
 ********************/

#if CONFIG_LITTLEFS_HUMAN_READABLE
/**
 * @brief converts an enumerated lfs error into a string.
 * @param lfs_errno The enumerated littlefs error.
 */
static const char *xf_littlefs_errno(enum lfs_error lfs_errno);
#endif

/* ==================== [Static Variables] ================================== */

static xf_osal_semaphore_t _efs_lock = NULL;
static xf_littlefs_t *_efs[CONFIG_LITTLEFS_MAX_PARTITIONS] = { 0 };

/* ==================== [Macros] ============================================ */

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

#define _ASSERT(expr) \
    do { \
        if (!(expr)) { \
            XF_LOGE(XF_LITTLEFS_TAG, "_ASSERT err"); \
            while (1); \
        } \
    } while (0)

#if !defined(xf_strnlen)
#   include <string.h>
static size_t my_strnlen(const char *str, size_t maxlen) 
{
    const char *end = str;
    while (maxlen-- > 0 && *end != '\0') {
        end++;
    }
    return end - str;
}
#   define xf_strnlen(s, maxlen)    my_strnlen((s), (maxlen))
#endif

#if !defined(xf_strncpy)
static char* my_strncpy(char *dst, const char *src, size_t n)
{
    char *dst_start = dst;
    while (n > 0 && *src != '\0') {
        *dst++ = *src++;
        n--;
    }
    while (n > 0) {
        *dst++ = '\0';
        n--;
    }
    return dst_start;
}
#   define xf_strncpy(d, s, len)    my_strncpy((d), (s), (len))
#endif

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static inline void *xf_littlefs_calloc(size_t __nmemb, size_t __size)
{
    /* Used internally by this wrapper only */
#if defined(CONFIG_LITTLEFS_MALLOC_STRATEGY_INTERNAL)
    return heap_caps_calloc(__nmemb, __size, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
#elif defined(CONFIG_LITTLEFS_MALLOC_STRATEGY_SPIRAM)
    return heap_caps_calloc(__nmemb, __size, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
#else /* CONFIG_LITTLEFS_MALLOC_STRATEGY_DISABLE, CONFIG_LITTLEFS_MALLOC_STRATEGY_DEFAULT or not defined */
    // return calloc(__nmemb, __size);
    void *buf = xf_malloc(__nmemb * __size);
    if (buf != NULL) {
        xf_memset(buf, 0, __nmemb * __size);
    }
    return buf;
#endif
}

static void xf_littlefs_free_fds(xf_littlefs_t *efs)
{
    /* Need to xf_free all files that were opened */
    while (efs->file) {
        vfs_littlefs_file_t *next = efs->file->next;
        xf_free(efs->file);
        efs->file = next;
    }
    xf_free(efs->cache);
    efs->cache = 0;
    efs->cache_size = efs->fd_count = 0;
}

static int lfs_errno_remap(enum lfs_error err)
{
    switch (err) {
    case LFS_ERR_OK:
        return 0;
    case LFS_ERR_IO:
        return EIO;
    case LFS_ERR_CORRUPT:
        return EBADMSG;  // This is a bit opinionated.
    case LFS_ERR_NOENT:
        return ENOENT;
    case LFS_ERR_EXIST:
        return EEXIST;
    case LFS_ERR_NOTDIR:
        return ENOTDIR;
    case LFS_ERR_ISDIR:
        return EISDIR;
    case LFS_ERR_NOTEMPTY:
        return ENOTEMPTY;
    case LFS_ERR_BADF:
        return EBADF;
    case LFS_ERR_FBIG:
        return EFBIG;
    case LFS_ERR_INVAL:
        return EINVAL;
    case LFS_ERR_NOSPC:
        return ENOSPC;
    case LFS_ERR_NOMEM:
        return ENOMEM;
    case LFS_ERR_NOATTR:
        return ENODATA;
    case LFS_ERR_NAMETOOLONG:
        return ENAMETOOLONG;
    }
    return EINVAL;  // Need some default vlaue
}

xf_err_t format_from_efs(xf_littlefs_t *efs)
{
    _ASSERT(efs);
    bool was_mounted = false;

    /* Unmount if mounted */
    if (efs->cache_size > 0) {
        int res;
        XF_LOGV(XF_LITTLEFS_TAG, "Partition was mounted. Unmounting...");
        was_mounted = true;
        res = lfs_unmount(efs->fs);
        if (res != LFS_ERR_OK) {
            XF_LOGE(XF_LITTLEFS_TAG, "Failed to unmount.");
            return XF_FAIL;
        }
        xf_littlefs_free_fds(efs);
    }

    /* Format */
    {
        xf_err_t res = XF_OK;
        XF_LOGV(XF_LITTLEFS_TAG, "Formatting filesystem");

        /* Need to write explicit block_count to cfg; but skip if it's the SD card */
        {
            efs->cfg.block_count = efs->partition->len / efs->cfg.block_size;
            res = lfs_format(efs->fs, &efs->cfg);
            efs->cfg.block_count = 0;
        }

        if (res != LFS_ERR_OK) {
            XF_LOGE(XF_LITTLEFS_TAG, "Failed to format filesystem");
            return XF_FAIL;
        }
    }

    /* Mount filesystem */
    if (was_mounted) {
        int res;
        /* Remount the partition */
        XF_LOGV(XF_LITTLEFS_TAG, "Remounting formatted partition");
        res = lfs_mount(efs->fs, &efs->cfg);
        if (res != LFS_ERR_OK) {
            XF_LOGE(XF_LITTLEFS_TAG, "Failed to re-mount filesystem");
            return XF_FAIL;
        }
        efs->cache_size = CONFIG_LITTLEFS_FD_CACHE_MIN_SIZE;  // Initial size of cache; will resize ondemand
        efs->cache = xf_littlefs_calloc(sizeof(*efs->cache), efs->cache_size);
    }
    XF_LOGV(XF_LITTLEFS_TAG, "Format Success!");

    return XF_OK;
}

void get_total_and_used_bytes(xf_littlefs_t *efs, size_t *total_bytes, size_t *used_bytes)
{
    sem_take(efs);
    size_t total_bytes_local = efs->cfg.block_size * efs->fs->block_count;
    if (total_bytes) {
        *total_bytes = total_bytes_local;
    }

    /* lfs_fs_size may return a size larger than the actual filesystem size.
     * https://github.com/littlefs-project/littlefs/blob/9c7e232086f865cff0bb96fe753deb66431d91fd/lfs.h#L658
     */
    if (used_bytes) {
        *used_bytes = MIN(total_bytes_local, efs->cfg.block_size * lfs_fs_size(efs->fs));
    }
    sem_give(efs);
}

/********************
 * Public Functions *
 ********************/

bool xf_littlefs_mounted(const char *partition_label)
{
    int index;
    xf_err_t err;

    err = xf_littlefs_by_label(partition_label, &index);
    if (err != XF_OK) {
        return false;
    }
    return _efs[index]->cache_size > 0;
}

bool xf_littlefs_partition_mounted(const xf_fal_partition_t *partition)
{
    int index;
    xf_err_t err = xf_littlefs_by_partition(partition, &index);

    if (err != XF_OK) {
        return false;
    }
    return _efs[index]->cache_size > 0;
}

xf_err_t xf_littlefs_info(const char *partition_label, size_t *total_bytes, size_t *used_bytes)
{
    int index;
    xf_err_t err;

    err = xf_littlefs_by_label(partition_label, &index);
    if (err != XF_OK) {
        return err;
    }
    get_total_and_used_bytes(_efs[index], total_bytes, used_bytes);

    return XF_OK;
}

xf_err_t xf_littlefs_partition_info(const xf_fal_partition_t *partition, size_t *total_bytes, size_t *used_bytes)
{
    int index;
    xf_err_t err;

    err = xf_littlefs_by_partition(partition, &index);
    if (err != XF_OK) {
        return err;
    }
    get_total_and_used_bytes(_efs[index], total_bytes, used_bytes);

    return XF_OK;
}

static size_t my_strlcat(char *dst, const char *src, size_t dst_size) 
{
    size_t dst_len = xf_strnlen(dst, dst_size);
    size_t src_len = xf_strlen(src);
    if (dst_len >= dst_size) {
        return dst_size + src_len; // dst已经满了，返回假定足够大时的长度
    }
    size_t copy_len = dst_size - dst_len - 1; // 剩余空间减去1（留给NULL终止符）
    if (copy_len > 0) {
        xf_strncpy(dst + dst_len, src, copy_len);
        dst[dst_len + copy_len] = '\0'; // 确保NULL终止符
    }
    return dst_len + src_len; // 返回理论上的拼接后总长度
}

xf_err_t xf_vfs_littlefs_register(const xf_vfs_littlefs_conf_t *conf)
{
    _ASSERT(conf->base_path);
    const xf_vfs_t vfs = vfs_littlefs_create_struct(!conf->read_only);

    xf_err_t err = xf_littlefs_init(conf);
    if (err != XF_OK) {
        XF_LOGE(XF_LITTLEFS_TAG, "Failed to initialize LittleFS");
        return err;
    }

    int index;

    if (conf->partition_label) {
        if (xf_littlefs_by_label(conf->partition_label, &index) != XF_OK) {
            XF_LOGE(XF_LITTLEFS_TAG, "Unable to find partition \"%s\"", conf->partition_label);
            return XF_ERR_NOT_FOUND;
        }
    } else {
        if (xf_littlefs_by_partition(conf->partition, &index) != XF_OK) {
            XF_LOGE(XF_LITTLEFS_TAG, "Unable to find partition \"0x%08"PRIX32"\"", conf->partition->offset);
            return XF_ERR_NOT_FOUND;
        }
    }

    my_strlcat(_efs[index]->base_path, conf->base_path, XF_VFS_PATH_MAX + 1);
    err = xf_vfs_register(conf->base_path, &vfs, _efs[index]);
    if (err != XF_OK) {
        xf_littlefs_free(&_efs[index]);
        XF_LOGE(XF_LITTLEFS_TAG, "Failed to register Littlefs to \"%s\"", conf->base_path);
        return err;
    }

    XF_LOGV(XF_LITTLEFS_TAG, "Successfully registered LittleFS to \"%s\"", conf->base_path);
    return XF_OK;
}

xf_err_t xf_vfs_littlefs_unregister(const char *partition_label)
{
    _ASSERT(partition_label);
    int index;
    if (xf_littlefs_by_label(partition_label, &index) != XF_OK) {
        XF_LOGE(XF_LITTLEFS_TAG, "Partition was never registered.");
        return XF_ERR_INVALID_STATE;
    }
    XF_LOGV(XF_LITTLEFS_TAG, "Unregistering \"%s\"", partition_label);
    xf_err_t err = xf_vfs_unregister(_efs[index]->base_path);
    if (err != XF_OK) {
        XF_LOGE(XF_LITTLEFS_TAG, "Failed to unregister \"%s\"", partition_label);
        return err;
    }
    xf_littlefs_free(&_efs[index]);
    _efs[index] = NULL;
    return XF_OK;
}

xf_err_t xf_vfs_littlefs_unregister_partition(const xf_fal_partition_t *partition)
{
    _ASSERT(partition);
    int index;
    if (xf_littlefs_by_partition(partition, &index) != XF_OK) {
        XF_LOGE(XF_LITTLEFS_TAG, "Partition was never registered.");
        return XF_ERR_INVALID_STATE;
    }
    XF_LOGV(XF_LITTLEFS_TAG, "Unregistering \"0x%08"PRIX32"\"", partition->offset);
    xf_err_t err = xf_vfs_unregister(_efs[index]->base_path);
    if (err != XF_OK) {
        XF_LOGE(XF_LITTLEFS_TAG, "Failed to unregister \"0x%08"PRIX32"\"", partition->offset);
        return err;
    }
    xf_littlefs_free(&_efs[index]);
    _efs[index] = NULL;
    return XF_OK;
}

xf_err_t xf_littlefs_format(const char *partition_label)
{
    _ASSERT(partition_label);

    bool efs_free = false;
    int index = -1;
    xf_err_t err;

    XF_LOGV(XF_LITTLEFS_TAG, "Formatting \"%s\"", partition_label);

    /* Get a context */
    err = xf_littlefs_by_label(partition_label, &index);

    if (err != XF_OK) {
        /* Create a tmp context */
        XF_LOGV(XF_LITTLEFS_TAG, "Temporarily creating EFS context.");
        efs_free = true;
        const xf_vfs_littlefs_conf_t conf = {
            /* base_name not necessary for initializing */
            .dont_mount = true,
            .partition_label = partition_label,
        };
        err = xf_littlefs_init(&conf);
        if (err != XF_OK) {
            XF_LOGE(XF_LITTLEFS_TAG, "Failed to initialize to format.");
            goto exit;
        }

        err = xf_littlefs_by_label(partition_label, &index);
        if (err != XF_OK) {
            XF_LOGE(XF_LITTLEFS_TAG, "Error obtaining context.");
            goto exit;
        }
    }

    err = format_from_efs(_efs[index]);

exit:
    if (efs_free && index >= 0) {
        xf_littlefs_free(&_efs[index]);
    }
    return err;
}

xf_err_t xf_littlefs_format_partition(const xf_fal_partition_t *partition)
{
    _ASSERT(partition);

    bool efs_free = false;
    int index = -1;
    xf_err_t err;

    XF_LOGV(XF_LITTLEFS_TAG, "Formatting partition at \"0x%08"PRIX32"\"", partition->address);

    /* Get a context */
    err = xf_littlefs_by_partition(partition, &index);

    if (err != XF_OK) {
        /* Create a tmp context */
        XF_LOGV(XF_LITTLEFS_TAG, "Temporarily creating EFS context.");
        efs_free = true;
        const xf_vfs_littlefs_conf_t conf = {
            /* base_name not necessary for initializing */
            .dont_mount = true,
            .partition_label = NULL,
            .partition = partition,
        };
        err = xf_littlefs_init(&conf);
        if (err != XF_OK) {
            XF_LOGE(XF_LITTLEFS_TAG, "Failed to initialize to format.");
            goto exit;
        }

        err = xf_littlefs_by_partition(partition, &index);
        if (err != XF_OK) {
            XF_LOGE(XF_LITTLEFS_TAG, "Error obtaining context.");
            goto exit;
        }
    }

    err = format_from_efs(_efs[index]);

exit:
    if (efs_free && index >= 0) {
        xf_littlefs_free(&_efs[index]);
    }
    return err;
}

/********************
 * Static Functions *
 ********************/

/*** Helpers ***/

#if CONFIG_LITTLEFS_HUMAN_READABLE
/**
 * @brief converts an enumerated lfs error into a string.
 * @param lfs_error The littlefs error.
 */
static const char *xf_littlefs_errno(enum lfs_error lfs_errno)
{
    switch (lfs_errno) {
    case LFS_ERR_OK:
        return "LFS_ERR_OK";
    case LFS_ERR_IO:
        return "LFS_ERR_IO";
    case LFS_ERR_CORRUPT:
        return "LFS_ERR_CORRUPT";
    case LFS_ERR_NOENT:
        return "LFS_ERR_NOENT";
    case LFS_ERR_EXIST:
        return "LFS_ERR_EXIST";
    case LFS_ERR_NOTDIR:
        return "LFS_ERR_NOTDIR";
    case LFS_ERR_ISDIR:
        return "LFS_ERR_ISDIR";
    case LFS_ERR_NOTEMPTY:
        return "LFS_ERR_NOTEMPTY";
    case LFS_ERR_BADF:
        return "LFS_ERR_BADF";
    case LFS_ERR_FBIG:
        return "LFS_ERR_FBIG";
    case LFS_ERR_INVAL:
        return "LFS_ERR_INVAL";
    case LFS_ERR_NOSPC:
        return "LFS_ERR_NOSPC";
    case LFS_ERR_NOMEM:
        return "LFS_ERR_NOMEM";
    case LFS_ERR_NOATTR:
        return "LFS_ERR_NOATTR";
    case LFS_ERR_NAMETOOLONG:
        return "LFS_ERR_NAMETOOLONG";
    default:
        return "LFS_ERR_UNDEFINED";
    }
    return "";
}
#else
#define xf_littlefs_errno(x) ""
#endif

static xf_vfs_t vfs_littlefs_create_struct(bool writeable)
{
    xf_vfs_t vfs = {
        .flags       = XF_VFS_FLAG_CONTEXT_PTR,
        .write_p     = &vfs_littlefs_write,
        .pwrite_p    = &vfs_littlefs_pwrite,
        .lseek_p     = &vfs_littlefs_lseek,
        .read_p      = &vfs_littlefs_read,
        .pread_p     = &vfs_littlefs_pread,
        .open_p      = &vfs_littlefs_open,
        .close_p     = &vfs_littlefs_close,
        .fsync_p     = &vfs_littlefs_fsync,
        .fcntl_p     = &vfs_littlefs_fcntl,
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        .fstat_p     = &vfs_littlefs_fstat,
#endif
#ifdef XF_VFS_SUPPORT_DIR_IS_ENABLE
        .stat_p      = &vfs_littlefs_stat,
        .link_p      = NULL, /* Not Supported */
        .unlink_p    = &vfs_littlefs_unlink,
        .rename_p    = &vfs_littlefs_rename,
        .opendir_p   = &vfs_littlefs_opendir,
        .readdir_p   = &vfs_littlefs_readdir,
        .readdir_r_p = &vfs_littlefs_readdir_r,
        .telldir_p   = &vfs_littlefs_telldir,
        .seekdir_p   = &vfs_littlefs_seekdir,
        .closedir_p  = &vfs_littlefs_closedir,
        .mkdir_p     = &vfs_littlefs_mkdir,
        .rmdir_p     = &vfs_littlefs_rmdir,
        // access_p
        .truncate_p  = &vfs_littlefs_truncate,
#ifdef XF_LITTLEFS_ENABLE_FTRUNCATE
        .ftruncate_p = &vfs_littlefs_ftruncate,
#endif // XF_LITTLEFS_ENABLE_FTRUNCATE
#if CONFIG_LITTLEFS_USE_MTIME
        .utime_p     = &vfs_littlefs_utime,
#endif // CONFIG_LITTLEFS_USE_MTIME
#endif // XF_VFS_SUPPORT_DIR_IS_ENABLE
    };
    if (!writeable) {
        vfs.write_p  = NULL;
        vfs.pwrite_p = NULL;
        vfs.fsync_p  = NULL;
        vfs.link_p   = NULL;
        vfs.unlink_p = NULL;
        vfs.rename_p = NULL;
        vfs.mkdir_p  = NULL;
        vfs.rmdir_p  = NULL;
    }
    return vfs;
}

/**
 * @brief Free and clear a littlefs definition structure.
 * @param efs Pointer to pointer to struct. Done this way so we can also zero
 *            out the pointer.
 */
static void xf_littlefs_free(xf_littlefs_t **efs)
{
    xf_littlefs_t *e = *efs;
    if (e == NULL) {
        return;
    }
    *efs = NULL;

    if (e->fs) {
        if (e->cache_size > 0) {
            lfs_unmount(e->fs);
        }
        xf_free(e->fs);
    }
    if (e->lock) {
        xf_osal_mutex_delete(e->lock);
    }
    xf_littlefs_free_fds(e);
    xf_free(e);
}

#ifdef XF_VFS_SUPPORT_DIR_IS_ENABLE
/**
 * @brief Free a vfs_littlefs_dir_t struct.
 */
static void xf_littlefs_dir_free(vfs_littlefs_dir_t *dir)
{
    if (dir == NULL) {
        return;
    }
    if (dir->path) {
        xf_free(dir->path);
    }
    xf_free(dir);
}
#endif

/**
 * Get a mounted littlefs filesystem by label.
 * @param[in] label
 * @param[out] index index into _efs
 * @return XF_OK on success
 */

static xf_err_t xf_littlefs_by_partition(const xf_fal_partition_t *part, int *index)
{
    int i;
    xf_littlefs_t *p;

    if (!part || !index) {
        return XF_ERR_INVALID_ARG;
    }

    XF_LOGV(XF_LITTLEFS_TAG, "Searching for existing filesystem for partition \"0x%08"PRIX32"\"", part->address);

    for (i = 0; i < CONFIG_LITTLEFS_MAX_PARTITIONS; i++) {
        p = _efs[i];
        if (!p) {
            continue;
        }
        if (!p->partition) {
            continue;
        }
        if (part->offset == p->partition->offset) {
            *index = i;
            XF_LOGV(XF_LITTLEFS_TAG, "Found existing filesystem \"0x%08"PRIX32"\" at index %d", part->address, *index);
            return XF_OK;
        }
    }

    XF_LOGV(XF_LITTLEFS_TAG, "Existing filesystem \"0x%08"PRIX32"\" not found", part->address);
    return XF_ERR_NOT_FOUND;
}

static xf_err_t xf_littlefs_by_label(const char *label, int *index)
{
    int i;
    xf_littlefs_t *p;

    if (!label || !index) {
        return XF_ERR_INVALID_ARG;
    }

    XF_LOGV(XF_LITTLEFS_TAG, "Searching for existing filesystem for partition \"%s\"", label);

    for (i = 0; i < CONFIG_LITTLEFS_MAX_PARTITIONS; i++) {
        p = _efs[i];
        if (!p) {
            continue;
        }
        if (!p->partition) {
            continue;
        }
        if (strncmp(label, p->partition->name, 17) == 0) {
            *index = i;
            XF_LOGV(XF_LITTLEFS_TAG, "Found existing filesystem \"%s\" at index %d", label, *index);
            return XF_OK;
        }
    }

    XF_LOGV(XF_LITTLEFS_TAG, "Existing filesystem \"%s\" not found", label);
    return XF_ERR_NOT_FOUND;
}

/**
 * @brief Get the index of an unallocated LittleFS slot.
 * @param[out] index Indexd of xf_free LittleFS slot
 * @return XF_OK on success
 */
static xf_err_t xf_littlefs_get_empty(int *index)
{
    _ASSERT(index);
    for (uint8_t i = 0; i < CONFIG_LITTLEFS_MAX_PARTITIONS; i++) {
        if (_efs[i] == NULL) {
            *index = i;
            return XF_OK;
        }
    }
    XF_LOGE(XF_LITTLEFS_TAG, "No more xf_free partitions available.");
    return XF_FAIL;
}

/**
 * @brief Convert fcntl flags to littlefs flags
 * @param m fcntl flags
 * @return lfs flags
 */
static int xf_littlefs_flags_conv(int m)
{
    int lfs_flags = 0;

    // Mask out unsupported flags; can cause internal LFS issues.
    m &= (XF_VFS_O_APPEND | XF_VFS_O_WRONLY | XF_VFS_O_RDWR | XF_VFS_O_EXCL | XF_VFS_O_CREAT | XF_VFS_O_TRUNC);

    // XF_VFS_O_RDONLY is 0 and not a flag, so must be explicitly checked
    if (m == XF_VFS_O_RDONLY)  {
        XF_LOGV(XF_LITTLEFS_TAG, "XF_VFS_O_RDONLY");
        lfs_flags |= LFS_O_RDONLY;
    }

    if (m & XF_VFS_O_APPEND)  {
        XF_LOGV(XF_LITTLEFS_TAG, "XF_VFS_O_APPEND");
        lfs_flags |= LFS_O_APPEND;
    }
    if (m & XF_VFS_O_WRONLY)  {
        XF_LOGV(XF_LITTLEFS_TAG, "XF_VFS_O_WRONLY");
        lfs_flags |= LFS_O_WRONLY;
    }
    if (m & XF_VFS_O_RDWR)    {
        XF_LOGV(XF_LITTLEFS_TAG, "XF_VFS_O_RDWR");
        lfs_flags |= LFS_O_RDWR;
    }
    if (m & XF_VFS_O_EXCL)    {
        XF_LOGV(XF_LITTLEFS_TAG, "XF_VFS_O_EXCL");
        lfs_flags |= LFS_O_EXCL;
    }
    if (m & XF_VFS_O_CREAT)   {
        XF_LOGV(XF_LITTLEFS_TAG, "XF_VFS_O_CREAT");
        lfs_flags |= LFS_O_CREAT;
    }
    if (m & XF_VFS_O_TRUNC)   {
        XF_LOGV(XF_LITTLEFS_TAG, "XF_VFS_O_TRUNC");
        lfs_flags |= LFS_O_TRUNC;
    }
    return lfs_flags;
}

static void xf_littlefs_take_efs_lock(void)
{
    if (_efs_lock == NULL) {
        // static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
        // portENTER_CRITICAL(&mux);
        // if( _efs_lock == NULL ){
        //     _efs_lock = xSemaphoreCreateMutex();
        //     _ASSERT(_efs_lock);
        // }
        // portEXIT_CRITICAL(&mux);

        xf_osal_semaphore_attr_t sem_attr = {
            .name = "sem",
        };
        _efs_lock = xf_osal_semaphore_create(1, 1, &sem_attr);
    }

    xf_osal_semaphore_acquire(_efs_lock, XF_OSAL_WAIT_FOREVER);
}

static xf_err_t xf_littlefs_init_efs(xf_littlefs_t **efs, const xf_fal_partition_t *partition, bool read_only)
{
    /* Allocate Context */
    *efs = xf_littlefs_calloc(1, sizeof(xf_littlefs_t));
    if (*efs == NULL) {
        XF_LOGE(XF_LITTLEFS_TAG, "xf_littlefs could not be malloced");
        return XF_ERR_NO_MEM;
    }
    (*efs)->partition = partition;

    { /* LittleFS Configuration */
        (*efs)->cfg.context = *efs;
        (*efs)->read_only = read_only;

        // block device operations
        (*efs)->cfg.read  = littlefs_xf_part_read;
        (*efs)->cfg.prog  = littlefs_xf_part_write;
        (*efs)->cfg.erase = littlefs_xf_part_erase;
        (*efs)->cfg.sync  = littlefs_xf_part_sync;

        /* 原本的 */
        // // block device configuration
        // (*efs)->cfg.read_size = CONFIG_LITTLEFS_READ_SIZE;
        // (*efs)->cfg.prog_size = CONFIG_LITTLEFS_WRITE_SIZE;
        // (*efs)->cfg.block_size = CONFIG_LITTLEFS_BLOCK_SIZE;
        // (*efs)->cfg.block_count = 0;  // Autodetect ``block_count``
        (*efs)->cfg.cache_size = CONFIG_LITTLEFS_CACHE_SIZE;
        (*efs)->cfg.lookahead_size = CONFIG_LITTLEFS_LOOKAHEAD_SIZE;
        (*efs)->cfg.block_cycles = CONFIG_LITTLEFS_BLOCK_CYCLES;

        const xf_fal_partition_t   *p_part = partition;
#if defined(CONFIG_PORT_LFS_CUSTOM_BLOCK_ENABLE) && (CONFIG_PORT_LFS_CUSTOM_BLOCK_ENABLE)
        (*efs)->cfg.block_size       = CONFIG_PORT_LFS_CUSTOM_BLOCK_SIZE;
        (*efs)->cfg.block_count      = CONFIG_PORT_LFS_CUSTOM_BLOCK_COUNT;
        (*efs)->cfg.read_size        = CONFIG_PORT_LFS_READ_SIZE;
        (*efs)->cfg.prog_size        = CONFIG_PORT_LFS_WRITE_SIZE;
#else
        const xf_fal_flash_dev_t *p_flash_dev = xf_fal_flash_device_find_by_part(p_part);
        if (NULL == p_flash_dev) {
            XF_LOGE(XF_LITTLEFS_TAG, "flash device not found.");
            return XF_ERR_NOT_FOUND;
        }
        if ((p_flash_dev->sector_size == 0)
                || (p_flash_dev->page_size == 0)
        ) {
            XF_LOGE(XF_LITTLEFS_TAG, "Flash device parameter error.");
            XF_LOGE(XF_LITTLEFS_TAG, "sector_size:%d", (int)p_flash_dev->sector_size);
            XF_LOGE(XF_LITTLEFS_TAG, "page_size:%d", (int)p_flash_dev->page_size);
            return XF_FAIL;
        }
        (*efs)->cfg.block_size       = p_flash_dev->sector_size;
        (*efs)->cfg.block_count      = p_part->len / p_flash_dev->sector_size;
        if ((*efs)->cfg.block_count == 0) {
            XF_LOGE(XF_LITTLEFS_TAG, "The partition length is too short.");
            return XF_FAIL;
        }
        (*efs)->cfg.read_size        = p_flash_dev->page_size;
        (*efs)->cfg.prog_size        = p_flash_dev->page_size;
#endif

#if CONFIG_LITTLEFS_MULTIVERSION
#if CONFIG_LITTLEFS_DISK_VERSION_MOST_RECENT
        (*efs)->cfg.disk_version = 0;
#elif CONFIG_LITTLEFS_DISK_VERSION_2_1
        (*efs)->cfg.disk_version = 0x00020001;
#elif CONFIG_LITTLEFS_DISK_VERSION_2_0
        (*efs)->cfg.disk_version = 0x00020000;
#else
#error "CONFIG_LITTLEFS_MULTIVERSION enabled but no or unknown disk version selected!"
#endif
#endif
    }

    xf_osal_mutex_attr_t mutex_attr = {
        .name = "mutex",
        .attr_bits = XF_OSAL_MUTEX_RECURSIVE,
    };
    (*efs)->lock = xf_osal_mutex_create(&mutex_attr);
    if ((*efs)->lock == NULL) {
        XF_LOGE(XF_LITTLEFS_TAG, "mutex lock could not be created");
        return XF_ERR_NO_MEM;
    }

    (*efs)->fs = xf_littlefs_calloc(1, sizeof(lfs_t));
    if ((*efs)->fs == NULL) {
        XF_LOGE(XF_LITTLEFS_TAG, "littlefs could not be malloced");
        return XF_ERR_NO_MEM;
    }

    return XF_OK;
}

/**
 * @brief Initialize and mount littlefs
 * @param[in] conf Filesystem Configuration
 * @return XF_OK on success
 */
static xf_err_t xf_littlefs_init(const xf_vfs_littlefs_conf_t *conf)
{
    int index = -1;
    xf_err_t err = XF_FAIL;
    const xf_fal_partition_t *partition = NULL;
    xf_littlefs_t *efs = NULL;

    xf_littlefs_take_efs_lock();

    if (xf_littlefs_get_empty(&index) != XF_OK) {
        XF_LOGE(XF_LITTLEFS_TAG, "max mounted partitions reached");
        err = XF_ERR_INVALID_STATE;
        goto exit;
    }

    if (conf->partition_label) {
        /* Input and Environment Validation */
        if (xf_littlefs_by_label(conf->partition_label, &index) == XF_OK) {
            XF_LOGE(XF_LITTLEFS_TAG, "Partition already used");
            err = XF_ERR_INVALID_STATE;
            goto exit;
        }
        partition = xf_fal_partition_find(conf->partition_label);
        if (!partition) {
            XF_LOGE(XF_LITTLEFS_TAG, "partition \"%s\" could not be found", conf->partition_label);
            err = XF_ERR_NOT_FOUND;
            goto exit;
        }

    } else if (conf->partition) {
        if (xf_littlefs_by_partition(conf->partition, &index) == XF_OK) {
            XF_LOGE(XF_LITTLEFS_TAG, "Partition already used");
            err = XF_ERR_INVALID_STATE;
            goto exit;
        }
        partition = conf->partition;
    } else {
        XF_LOGE(XF_LITTLEFS_TAG, "No partition specified in configuration");
        err = XF_ERR_INVALID_ARG;
        goto exit;
    }
    {
        const xf_fal_flash_dev_t *p_flash_dev = xf_fal_flash_device_find_by_part(partition);
        if (NULL == p_flash_dev) {
            XF_LOGE(XF_LITTLEFS_TAG, "flash device not found.");
            return XF_ERR_NOT_FOUND;
        }
        uint32_t flash_page_size = p_flash_dev->page_size;
#define CONFIG_LITTLEFS_PAGE_SIZE 256
        uint32_t log_page_size = CONFIG_LITTLEFS_PAGE_SIZE;
        if (log_page_size % flash_page_size != 0) {
            XF_LOGE(XF_LITTLEFS_TAG, "LITTLEFS_PAGE_SIZE is not multiple of flash chip page size (%u)",
                    (unsigned int) flash_page_size);
            err = XF_ERR_INVALID_ARG;
            goto exit;
        }

        err = xf_littlefs_init_efs(&efs, partition, conf->read_only);

        if (err != XF_OK) {
            goto exit;
        }
    }

    // Mount and Error Check
    _efs[index] = efs;
    if (!conf->dont_mount) {
        int res;

        res = lfs_mount(efs->fs, &efs->cfg);

        if (conf->format_if_mount_failed && res != LFS_ERR_OK) {
            XF_LOGW(XF_LITTLEFS_TAG, "mount failed, %s (%i). formatting...", xf_littlefs_errno(res), res);
            {
                err = xf_littlefs_format_partition(efs->partition);
            }
            if (err != XF_OK) {
                XF_LOGE(XF_LITTLEFS_TAG, "format failed");
                err = XF_FAIL;
                goto exit;
            }
            res = lfs_mount(efs->fs, &efs->cfg);
        }
        if (res != LFS_ERR_OK) {
            XF_LOGE(XF_LITTLEFS_TAG, "mount failed, %s (%i)", xf_littlefs_errno(res), res);
            err = XF_FAIL;
            goto exit;
        }
        efs->cache_size = 4;
        efs->cache = xf_littlefs_calloc(sizeof(*efs->cache), efs->cache_size);

        if (conf->grow_on_mount) {
            {
                res = lfs_fs_grow(efs->fs, efs->partition->len / efs->cfg.block_size);
            }
            if (res != LFS_ERR_OK) {
                XF_LOGE(XF_LITTLEFS_TAG, "FS grow failed, %s (%i)", xf_littlefs_errno(res), res);
                err = XF_FAIL;
                goto exit;
            }
        }
    }

    err = XF_OK;

exit:
    if (err != XF_OK) {
        if (index >= 0) {
            xf_littlefs_free(&_efs[index]);
        } else {
            xf_littlefs_free(&efs);
        }
    }
    xf_osal_semaphore_release(_efs_lock);
    return err;
}

/**
 * @brief
 * @parameter efs file system context
 */
static inline int sem_take(xf_littlefs_t *efs)
{
    int res;
#if LOG_LOCAL_LEVEL >= 5
    XF_LOGV(XF_LITTLEFS_TAG, "------------------------ Sem Taking [%s]", pcTaskGetName(NULL));
#endif
    // res = xSemaphoreTakeRecursive(efs->lock, XF_OSAL_WAIT_FOREVER);
    xf_err_t xf_ret;
    xf_ret = xf_osal_mutex_acquire(efs->lock, XF_OSAL_WAIT_FOREVER);
    res = (xf_ret == XF_OK) ? (1) : (0);
#if LOG_LOCAL_LEVEL >= 5
    XF_LOGV(XF_LITTLEFS_TAG, "--------------------->>> Sem Taken [%s]", pcTaskGetName(NULL));
#endif
    return res;
}

/**
 * @brief
 * @parameter efs file system context
 */
static inline int sem_give(xf_littlefs_t *efs)
{
#if LOG_LOCAL_LEVEL >= 5
    XF_LOGV(XF_LITTLEFS_TAG, "---------------------<<< Sem Give [%s]", pcTaskGetName(NULL));
#endif
    // return xSemaphoreGiveRecursive(efs->lock);
    return xf_osal_mutex_release(efs->lock);
}

/* We are using a double allocation system here, which an array and a linked list.
   The array contains the pointer to the file descriptor (the index in the array is what's returned to the user).
   The linked list is used for file descriptors.
   This means that position of nodes in the list must stay consistent:
   - Allocation is obvious (append to the list from the head, and realloc the pointers array)
     There is still a O(N) search in the cache for a xf_free position to store
   - Searching is a O(1) process (good)
   - Deallocation is more tricky. That is, for example,
     if you need to remove node 5 in a 12 nodes list, you'll have to:
       1) Mark the 5th position as freed (if it's the last position of the array realloc smaller)
       2) Walk the list until finding the pointer to the node O(N) and scrub the node so the chained list stays consistent
       3) Deallocate the node
*/

/**
 * @brief Get a file descriptor
 * @param[in,out] efs       file system context
 * @param[out]    file      pointer to a file that'll be filled with a file object
 * @param[in]     path_len  the length of the filepath in bytes (including terminating zero byte)
 * @return integer file descriptor. Returns -1 if a FD cannot be obtained.
 * @warning This must be called with lock taken
 */
static int xf_littlefs_allocate_fd(xf_littlefs_t *efs, vfs_littlefs_file_t **file
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
                                   , const size_t path_len
#endif
                                  )
{
    int i = -1;

    _ASSERT(efs->fd_count < UINT16_MAX);
    _ASSERT(efs->cache_size < UINT16_MAX);

    /* Make sure there is enough space in the cache to store new fd */
    if (efs->fd_count + 1 > efs->cache_size) {
        uint16_t new_size = (uint16_t)MIN(UINT16_MAX, CONFIG_LITTLEFS_FD_CACHE_REALLOC_FACTOR * efs->cache_size);
        /* Resize the cache */
        vfs_littlefs_file_t **new_cache = realloc(efs->cache, new_size * sizeof(*efs->cache));
        if (!new_cache) {
            XF_LOGE(XF_LITTLEFS_TAG, "Unable to allocate file cache");
            return -1; /* If it fails here, no harm is done to the filesystem, so it's safe */
        }
        /* Zero out the new portions of the cache */
        memset(&new_cache[efs->cache_size], 0, (new_size - efs->cache_size) * sizeof(*efs->cache));
        efs->cache = new_cache;
        efs->cache_size = new_size;
    }

    /* Allocate file descriptor here now */
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
    *file = xf_littlefs_calloc(1, sizeof(**file) + path_len);
#else
    *file = xf_littlefs_calloc(1, sizeof(**file));
#endif

    if (*file == NULL) {
        /* If it fails here, the file system might have a larger cache, but it's harmless, no need to reverse it */
        XF_LOGE(XF_LITTLEFS_TAG, "Unable to allocate FD");
        return -1;
    }

    /* Starting from here, nothing can fail anymore */

#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
    /* The trick here is to avoid dual allocation so the path pointer
        should point to the next byte after it:
        file => [ lfs_file | # | next | path | free_space ]
                                            |  /\
                                            |__/
    */
    (*file)->path = (char *)(*file) + sizeof(**file);
#endif

    /* Now find a xf_free place in cache */
    for (i = 0; i < efs->cache_size; i++) {
        if (efs->cache[i] == NULL) {
            efs->cache[i] = *file;
            break;
        }
    }
    /* Save file in the list */
    (*file)->next = efs->file;
    efs->file = *file;
    efs->fd_count++;
    return i;
}

/**
 * @brief Release a file descriptor
 * @param[in,out] efs file system context
 * @param[in] fd File Descriptor to release
 * @return 0 on success. -1 if a FD cannot be obtained.
 * @warning This must be called with lock taken
 */
static int xf_littlefs_free_fd(xf_littlefs_t *efs, int fd)
{
    vfs_littlefs_file_t *file, *head;

    if ((uint32_t)fd >= efs->cache_size) {
        XF_LOGE(XF_LITTLEFS_TAG, "FD %d must be <%d.", fd, efs->cache_size);
        return -1;
    }

    /* Get the file descriptor to xf_free it */
    file = efs->cache[fd];
    head = efs->file;
    /* Search for file in SLL to remove it */
    if (file == head) {
        /* Last file, can't fail */
        efs->file = efs->file->next;
    } else {
        while (head && head->next != file) {
            head = head->next;
        }
        if (!head) {
            XF_LOGE(XF_LITTLEFS_TAG, "Inconsistent list");
            return -1;
        }
        /* Transaction starts here and can't fail anymore */
        head->next = file->next;
    }
    efs->cache[fd] = NULL;
    efs->fd_count--;

    XF_LOGV(XF_LITTLEFS_TAG, "Clearing FD");
    xf_free(file);

#if 0
    /* Realloc smaller if its possible
     *     * Find and realloc based on number of trailing NULL ptrs in cache
     *     * Leave some hysteris to prevent thrashing around resize points
     * This is disabled for now because it adds unnecessary complexity
     * and binary size increase that outweights its ebenfits.
     */
    if (efs->cache_size > CONFIG_LITTLEFS_FD_CACHE_MIN_SIZE) {
        uint16_t n_free;
        uint16_t new_size = efs->cache_size / CONFIG_LITTLEFS_FD_CACHE_REALLOC_FACTOR;

        if (new_size >= CONFIG_LITTLEFS_FD_CACHE_MIN_SIZE) {
            /* Count number of trailing NULL ptrs */
            for (n_free = 0; n_free < efs->cache_size; n_free++) {
                if (efs->cache[efs->cache_size - n_free - 1] != NULL) {
                    break;
                }
            }

            if (n_free >= (efs->cache_size - new_size)) {
                new_size += CONFIG_LITTLEFS_FD_CACHE_HYST;
                XF_LOGV(XF_LITTLEFS_TAG, "Reallocating cache %i -> %i", efs->cache_size, new_size);
                vfs_littlefs_file_t **new_cache;
                new_cache = realloc(efs->cache, new_size * sizeof(*efs->cache));
                /* No harm on realloc failure, continue using the oversized cache */
                if (new_cache) {
                    efs->cache = new_cache;
                    efs->cache_size = new_size;
                }
            }
        }
    }
#endif

    return 0;
}

/**
 * @brief Compute the 32bit DJB2 hash of the given string.
 * @param[in]   path the path to hash
 * @returns the hash for this path
 */
static uint32_t compute_hash(const char *path)
{
    uint32_t hash = 5381;
    char c;

    while ((c = *path++)) {
        hash = ((hash << 5) + hash) + c;    /* hash * 33 + c */
    }
    return hash;
}

#ifdef XF_VFS_SUPPORT_DIR_IS_ENABLE
/**
 * @brief finds an open file descriptor by file name.
 * @param[in,out] efs file system context
 * @param[in] path File path to check.
 * @returns integer file descriptor. Returns -1 if not found.
 * @warning This must be called with lock taken
 * @warning if CONFIG_LITTLEFS_USE_ONLY_HASH, there is a slim chance an
 *          erroneous FD may be returned on hash collision.
 */
static int xf_littlefs_get_fd_by_name(xf_littlefs_t *efs, const char *path)
{
    uint32_t hash = compute_hash(path);

    for (uint16_t i = 0, j = 0; i < efs->cache_size && j < efs->fd_count; i++) {
        if (efs->cache[i]) {
            ++j;

            if (
                efs->cache[i]->hash == hash  // Faster than strcmp
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
                && strcmp(path, efs->cache[i]->path) == 0  // May as well check incase of hash collision. Usually short-circuited.
#endif
            ) {
                XF_LOGV(XF_LITTLEFS_TAG, "Found \"%s\" at FD %d.", path, i);
                return i;
            }
        }
    }
    XF_LOGV(XF_LITTLEFS_TAG, "Unable to get a find FD for \"%s\"", path);
    return -1;
}
#endif

/*** Filesystem Hooks ***/

static int vfs_littlefs_open(void *ctx, const char *path, int flags, int mode)
{
    /* Note: mode is currently unused */
    int fd = -1, lfs_flags, res;
    xf_littlefs_t *efs = (xf_littlefs_t *)ctx;
    vfs_littlefs_file_t *file = NULL;
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
    size_t path_len = strlen(path) + 1;  // include NULL terminator
#endif
#if CONFIG_LITTLEFS_OPEN_DIR
    struct lfs_info info;
#endif

    _ASSERT(path);

    XF_LOGV(XF_LITTLEFS_TAG, "Opening %s", path);

    /* Convert flags to lfs flags */
    lfs_flags = xf_littlefs_flags_conv(flags);
    if (efs->read_only && lfs_flags != LFS_O_RDONLY) {
        return LFS_ERR_INVAL;
    }

    /* Get a FD */
    sem_take(efs);

#if CONFIG_LITTLEFS_OPEN_DIR
    /* Check if it is a file with same path */
    if (flags & XF_VFS_O_DIRECTORY) {
        res = lfs_stat(efs->fs, path, &info);
        if (res == LFS_ERR_OK) {
            if (info.type == LFS_TYPE_REG) {
                sem_give(efs);
                XF_LOGV(XF_LITTLEFS_TAG, "Open directory but it is a file");
                errno = ENOTDIR;
                return LFS_ERR_INVAL;
            }
        }
    }
#endif

    fd = xf_littlefs_allocate_fd(efs, &file
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
                                 , path_len
#endif
                                );

    if (fd < 0) {
        errno = lfs_errno_remap(fd);
        sem_give(efs);
        XF_LOGV(XF_LITTLEFS_TAG, "Error obtaining FD");
        return LFS_ERR_INVAL;
    }

#if CONFIG_LITTLEFS_SPIFFS_COMPAT
    /* Create all parent directories (if necessary) */
    XF_LOGV(XF_LITTLEFS_TAG, "LITTLEFS_SPIFFS_COMPAT attempting to create all directories for %s", path);
    mkdirs(efs, path);
#endif  // CONFIG_LITTLEFS_SPIFFS_COMPAT

#ifndef CONFIG_LITTLEFS_MALLOC_STRATEGY_DISABLE
    /* Open File */
    res = lfs_file_open(efs->fs, &file->file, path, lfs_flags);
#else
#error "The use of static buffers is not currently supported by this VFS wrapper"
#endif

#if CONFIG_LITTLEFS_OPEN_DIR
    if (flags & XF_VFS_O_DIRECTORY && res ==  LFS_ERR_ISDIR) {
        res = LFS_ERR_OK;
        file->file.flags = flags;
    }
#endif

    if (res < 0) {
        errno = lfs_errno_remap(res);
        xf_littlefs_free_fd(efs, fd);
        sem_give(efs);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to open file %s. Error %s (%d)",
                path, xf_littlefs_errno(res), res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to open file. Error %s (%d)",
                xf_littlefs_errno(res), res);
#endif
        return LFS_ERR_INVAL;
    }

    /* Sync after opening. If we are overwriting a file, this will xf_free that
     * file's blocks in storage, prevent OOS errors.
     * See TEST_CASE:
     *     "Rewriting file frees space immediately (#7426)"
     */
#if CONFIG_LITTLEFS_OPEN_DIR
    if ((flags & XF_VFS_O_DIRECTORY) == 0) {
#endif
        if (!efs->read_only) {
            res = lfs_file_sync(efs->fs, &file->file);
        }
        if (res < 0) {
            errno = lfs_errno_remap(res);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
            XF_LOGV(XF_LITTLEFS_TAG, "Failed to sync at opening file \"%s\". Error %s (%d)",
                    file->path, xf_littlefs_errno(res), res);
#else
            XF_LOGV(XF_LITTLEFS_TAG, "Failed to sync at opening file %d. Error %d", fd, res);
#endif
        }

#if CONFIG_LITTLEFS_OPEN_DIR
    }
#endif

    file->hash = compute_hash(path);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
    memcpy(file->path, path, path_len);
#endif

#if CONFIG_LITTLEFS_USE_MTIME
    if (lfs_flags != LFS_O_RDONLY) {
        /* If this is being opened as not read-only */
        vfs_littlefs_update_mtime(efs, path);
    }
#endif

    sem_give(efs);
    XF_LOGV(XF_LITTLEFS_TAG, "Done opening %s", path);
    return fd;
}

static xf_vfs_ssize_t vfs_littlefs_write(void *ctx, int fd, const void *data, size_t size)
{
    xf_littlefs_t *efs = (xf_littlefs_t *)ctx;
    xf_vfs_ssize_t res;
    vfs_littlefs_file_t *file = NULL;

    sem_take(efs);
    if ((uint32_t)fd > efs->cache_size) {
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "FD %d must be <%d.", fd, efs->cache_size);
        errno = EBADF;
        return -1;
    }
    file = efs->cache[fd];
    res = lfs_file_write(efs->fs, &file->file, data, size);
#ifdef CONFIG_LITTLEFS_FLUSH_FILE_EVERY_WRITE
    if (res > 0) {
        vfs_littlefs_fsync(ctx, fd);
    }
#endif
    sem_give(efs);

    if (res < 0) {
        errno = lfs_errno_remap(res);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to write FD %d; path \"%s\". Error %s (%d)",
                fd, file->path, xf_littlefs_errno(res), res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to write FD %d. Error %s (%d)",
                fd, xf_littlefs_errno(res), res);
#endif
        return -1;
    }

    return res;
}

static xf_vfs_ssize_t vfs_littlefs_read(void *ctx, int fd, void *dst, size_t size)
{
    xf_littlefs_t *efs = (xf_littlefs_t *)ctx;
    xf_vfs_ssize_t res;
    vfs_littlefs_file_t *file = NULL;

    sem_take(efs);
    if ((uint32_t)fd > efs->cache_size) {
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "FD %d must be <%d.", fd, efs->cache_size);
        errno = EBADF;
        return -1;
    }
    file = efs->cache[fd];
    res = lfs_file_read(efs->fs, &file->file, dst, size);
    sem_give(efs);

    if (res < 0) {
        errno = lfs_errno_remap(res);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to read file \"%s\". Error %s (%d)",
                file->path, xf_littlefs_errno(res), res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to read FD %d. Error %s (%d)",
                fd, xf_littlefs_errno(res), res);
#endif
        return -1;
    }

    return res;
}

static xf_vfs_ssize_t vfs_littlefs_pwrite(void *ctx, int fd, const void *src, size_t size, xf_vfs_off_t offset)
{
    xf_littlefs_t *efs = (xf_littlefs_t *)ctx;
    xf_vfs_ssize_t res, save_res;
    vfs_littlefs_file_t *file = NULL;

    sem_take(efs);
    if ((uint32_t)fd > efs->cache_size) {
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "FD %d must be <%d.", fd, efs->cache_size);
        errno = EBADF;
        return -1;
    }
    file = efs->cache[fd];

    xf_vfs_off_t old_offset = lfs_file_seek(efs->fs, &file->file, 0, SEEK_CUR);
    if (old_offset < (xf_vfs_off_t)0) {
        res = old_offset;
        goto exit;
    }

    /* Set to wanted position.  */
    res = lfs_file_seek(efs->fs, &file->file, offset, SEEK_SET);
    if (res < (xf_vfs_off_t)0) {
        goto exit;

    }

    /* Write out the data.  */
    res = lfs_file_write(efs->fs, &file->file, src, size);

    /* Now we have to restore the position.  If this fails we have to
     return this as an error. But if the writing also failed we
     return writing error.  */
    save_res = lfs_file_seek(efs->fs, &file->file, old_offset, SEEK_SET);
    if (res >= (xf_vfs_ssize_t)0 && save_res < (xf_vfs_off_t)0)
    {     res = save_res;
    }
    sem_give(efs);

exit:
    if (res < 0)
    {     errno = lfs_errno_remap(res);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to write FD %d; path \"%s\". Error %s (%d)",
                fd, file->path, xf_littlefs_errno(res), res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to write FD %d. Error %s (%d)",
                fd, xf_littlefs_errno(res), res);
#endif
        return -1;
    }

    return res;
}

static xf_vfs_ssize_t vfs_littlefs_pread(void *ctx, int fd, void *dst, size_t size, xf_vfs_off_t offset)
{
    xf_littlefs_t *efs = (xf_littlefs_t *)ctx;
    xf_vfs_ssize_t res, save_res;
    vfs_littlefs_file_t *file = NULL;

    sem_take(efs);
    if ((uint32_t)fd > efs->cache_size)
    {
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "FD %d must be <%d.", fd, efs->cache_size);
        errno = EBADF;
        return -1;
    }
    file = efs->cache[fd];

    xf_vfs_off_t old_offset = lfs_file_seek(efs->fs, &file->file, 0, SEEK_CUR);
    if (old_offset < (xf_vfs_off_t)0)
    {
        res = old_offset;
        goto exit;
    }

    /* Set to wanted position.  */
    res = lfs_file_seek(efs->fs, &file->file, offset, SEEK_SET);
    if (res < (xf_vfs_off_t)0)
        goto exit;

    /* Read the data.  */
    res = lfs_file_read(efs->fs, &file->file, dst, size);

    /* Now we have to restore the position.  If this fails we have to
     return this as an error. But if the reading also failed we
     return reading error.  */
    save_res = lfs_file_seek(efs->fs, &file->file, old_offset, SEEK_SET);
    if (res >= (xf_vfs_ssize_t)0 && save_res < (xf_vfs_off_t)0)
    {
        res = save_res;
    }
    sem_give(efs);

exit:
    if (res < 0)
    {
        errno = lfs_errno_remap(res);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to read file \"%s\". Error %s (%d)",
                 file->path, xf_littlefs_errno(res), res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to read FD %d. Error %s (%d)",
                 fd, xf_littlefs_errno(res), res);
#endif
        return -1;
    }

    return res;
}

static int vfs_littlefs_close(void* ctx, int fd) {
    // TODO update mtime on close? SPIFFS doesn't do this
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    int res;
    vfs_littlefs_file_t *file = NULL;

    sem_take(efs);
    if((uint32_t)fd > efs->cache_size) {
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "FD %d must be <%d.", fd, efs->cache_size);
        errno = EBADF;
        return -1;
    }

    file = efs->cache[fd];

#if CONFIG_LITTLEFS_OPEN_DIR
    if ((file->file.flags & XF_VFS_O_DIRECTORY) == 0) {
#endif
    res = lfs_file_close(efs->fs, &file->file);
    if(res < 0){
        errno = lfs_errno_remap(res);
        sem_give(efs);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to close file \"%s\". Error %s (%d)",
                file->path, xf_littlefs_errno(res), res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to close Fd %d. Error %s (%d)",
                fd, xf_littlefs_errno(res), res);
#endif
        return -1;
    }
#if CONFIG_LITTLEFS_OPEN_DIR
    } else {
        res = 0;
    }
#endif

    xf_littlefs_free_fd(efs, fd);
    sem_give(efs);
    return res;
}

static xf_vfs_off_t vfs_littlefs_lseek(void* ctx, int fd, xf_vfs_off_t offset, int mode) {
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    lfs_soff_t res;
    vfs_littlefs_file_t *file = NULL;
    int whence;

    switch (mode) {
        case SEEK_SET: whence = LFS_SEEK_SET; break;
        case SEEK_CUR: whence = LFS_SEEK_CUR; break;
        case SEEK_END: whence = LFS_SEEK_END; break;
        default:
            XF_LOGE(XF_LITTLEFS_TAG, "Invalid mode");
            errno = EINVAL;
            return -1;
    }

    sem_take(efs);
    if((uint32_t)fd > efs->cache_size) {
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "FD %d must be <%d.", fd, efs->cache_size);
        errno = EBADF;
        return -1;
    }
    file = efs->cache[fd];
    res = lfs_file_seek(efs->fs, &file->file, offset, whence);
    sem_give(efs);

    if(res < 0){
        errno = lfs_errno_remap(res);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to seek file \"%s\" to offset %08x. Error %s (%d)",
                file->path, (unsigned int)offset, xf_littlefs_errno(res), (int) res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to seek FD %d to offset %08x. Error (%d)",
                fd, (unsigned int)offset, (int) res);
#endif
        return -1;
    }

    return res;
}

static int vfs_littlefs_fsync(void* ctx, int fd)
{
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    xf_vfs_ssize_t res;
    vfs_littlefs_file_t *file = NULL;

    sem_take(efs);
    if((uint32_t)fd > efs->cache_size) {
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "FD %d must be <%d.", fd, efs->cache_size);
        errno = EBADF;
        return -1;
    }
    file = efs->cache[fd];
    res = lfs_file_sync(efs->fs, &file->file);
    sem_give(efs);

    if(res < 0){
        errno = lfs_errno_remap(res);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to sync file \"%s\". Error %s (%d)",
                file->path, xf_littlefs_errno(res), res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to sync file %d. Error %d", fd, res);
#endif
        return -1;
    }

    return res;
}

#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
static int vfs_littlefs_fstat(void* ctx, int fd, xf_vfs_stat_t * st) {
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    struct lfs_info info;
    int res;
    vfs_littlefs_file_t *file = NULL;

    memset(st, 0, sizeof(xf_vfs_stat_t));
    st->st_blksize = efs->cfg.block_size;

    sem_take(efs);
    if((uint32_t)fd > efs->cache_size) {
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "FD must be <%d.", efs->cache_size);
        errno = EBADF;
        return -1;
    }
    file = efs->cache[fd];
    res = lfs_stat(efs->fs, file->path, &info);
    if (res < 0) {
        errno = lfs_errno_remap(res);
        sem_give(efs);
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to stat file \"%s\". Error %s (%d)",
                file->path, xf_littlefs_errno(res), res);
        return -1;
    }

#if CONFIG_LITTLEFS_USE_MTIME
    st->st_modtime = vfs_littlefs_get_mtime(efs, file->path);
#endif

    sem_give(efs);

    st->st_size = info.size;
    st->st_mode = ((info.type==LFS_TYPE_REG)?XF_VFS_S_IFREG:XF_VFS_S_IFDIR);
    return 0;
}
#endif

#ifdef XF_VFS_SUPPORT_DIR_IS_ENABLE
static int vfs_littlefs_stat(void* ctx, const char * path, xf_vfs_stat_t * st) {
    _ASSERT(path);
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    struct lfs_info info;
    int res;

    memset(st, 0, sizeof(xf_vfs_stat_t));
    st->st_blksize = efs->cfg.block_size;

    sem_take(efs);
    res = lfs_stat(efs->fs, path, &info);
    if (res < 0) {
        errno = lfs_errno_remap(res);
        sem_give(efs);
        /* Not strictly an error, since stat can be used to check
         * if a file exists */
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to stat path \"%s\". Error %s (%d)",
                path, xf_littlefs_errno(res), res);
        return -1;
    }
#if CONFIG_LITTLEFS_USE_MTIME
#undef st_mtime
#define st_mtime st_modtime
    st->st_mtime = vfs_littlefs_get_mtime(efs, path);
#endif
    sem_give(efs);
    st->st_size = info.size;
    st->st_mode = ((info.type==LFS_TYPE_REG)?XF_VFS_S_IFREG:XF_VFS_S_IFDIR);
    return 0;
}

static int vfs_littlefs_unlink(void* ctx, const char *path) {
#define fail_str_1 "Failed to unlink path \"%s\"."
    _ASSERT(path);
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    struct lfs_info info;
    int res;

    sem_take(efs);
    res = lfs_stat(efs->fs, path, &info);
    if (res < 0) {
        errno = lfs_errno_remap(res);
        sem_give(efs);
        XF_LOGV(XF_LITTLEFS_TAG, fail_str_1 " Error %s (%d)",
                path, xf_littlefs_errno(res), res);
        return -1;
    }

    if(xf_littlefs_get_fd_by_name(efs, path) >= 0) {
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, fail_str_1 " Has open FD.", path);
        errno = EBUSY;
        return -1;
    }

    if (info.type == LFS_TYPE_DIR) {
        sem_give(efs);
        XF_LOGV(XF_LITTLEFS_TAG, "Cannot unlink a directory.");
        errno = EISDIR;
        return -1;
    }

    res = lfs_remove(efs->fs, path);
    if (res < 0) {
        errno = lfs_errno_remap(res);
        sem_give(efs);
        XF_LOGV(XF_LITTLEFS_TAG, fail_str_1 " Error %s (%d)",
                path, xf_littlefs_errno(res), res);
        return -1;
    }

#if CONFIG_LITTLEFS_SPIFFS_COMPAT
    /* Attempt to delete all parent directories that are empty */
    rmdirs(efs, path);
#endif  // CONFIG_LITTLEFS_SPIFFS_COMPAT

    sem_give(efs);

    return 0;
#undef fail_str_1
}

static int vfs_littlefs_rename(void* ctx, const char *src, const char *dst) {
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    int res;

    sem_take(efs);

    if(xf_littlefs_get_fd_by_name(efs, src) >= 0){
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "Cannot rename; src \"%s\" is open.", src);
        errno = EBUSY;
        return -1;
    }
    else if(xf_littlefs_get_fd_by_name(efs, dst) >= 0){
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "Cannot rename; dst \"%s\" is open.", dst);
        errno = EBUSY;
        return -1;
    }

#if CONFIG_LITTLEFS_SPIFFS_COMPAT
    /* Create all parent directories to dst (if necessary) */
    XF_LOGV(XF_LITTLEFS_TAG, "LITTLEFS_SPIFFS_COMPAT attempting to create all directories for %s", src);
    mkdirs(efs, dst);
#endif

    res = lfs_rename(efs->fs, src, dst);
    if (res < 0) {
        errno = lfs_errno_remap(res);
        sem_give(efs);
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to rename \"%s\" -> \"%s\". Error %s (%d)",
                src, dst, xf_littlefs_errno(res), res);
        return -1;
    }

#if CONFIG_LITTLEFS_SPIFFS_COMPAT
    /* Attempt to delete all parent directories from src that are empty */
    rmdirs(efs, src);
#endif  // CONFIG_LITTLEFS_SPIFFS_COMPAT

    sem_give(efs);

    return 0;
}

static xf_vfs_dir_t* vfs_littlefs_opendir(void* ctx, const char* name) {
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    int res;
    vfs_littlefs_dir_t *dir = NULL;

    dir = xf_littlefs_calloc(1, sizeof(vfs_littlefs_dir_t));
    if( dir == NULL ) {
        XF_LOGE(XF_LITTLEFS_TAG, "dir struct could not be malloced");
        errno = ENOMEM;
        goto exit;
    }

    dir->path = strdup(name);
    if(dir->path == NULL){
        errno = ENOMEM;
        XF_LOGE(XF_LITTLEFS_TAG, "dir path name could not be malloced");
        goto exit;
    }

    sem_take(efs);
    res = lfs_dir_open(efs->fs, &dir->d, dir->path);
    sem_give(efs);
    if (res < 0) {
        errno = lfs_errno_remap(res);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to opendir \"%s\". Error %s (%d)",
                dir->path, xf_littlefs_errno(res), res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to opendir \"%s\". Error %d", dir->path, res);
#endif
        goto exit;
    }

    return (xf_vfs_dir_t *)dir;

exit:
    xf_littlefs_dir_free(dir);
    return NULL;
}

static int vfs_littlefs_closedir(void* ctx, xf_vfs_dir_t* pdir) {
    _ASSERT(pdir);
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    vfs_littlefs_dir_t * dir = (vfs_littlefs_dir_t *) pdir;
    int res;

    sem_take(efs);
    res = lfs_dir_close(efs->fs, &dir->d);
    sem_give(efs);
    if (res < 0) {
        errno = lfs_errno_remap(res);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to closedir \"%s\". Error %s (%d)",
                dir->path, xf_littlefs_errno(res), res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to closedir \"%s\". Error %d", dir->path, res);
#endif
        return res;
    }

    xf_littlefs_dir_free(dir);
    return 0;
}

static xf_vfs_dirent_t* vfs_littlefs_readdir(void* ctx, xf_vfs_dir_t* pdir) {
    _ASSERT(pdir);
    vfs_littlefs_dir_t * dir = (vfs_littlefs_dir_t *) pdir;
    int res;
    xf_vfs_dirent_t* out_dirent;

    res = vfs_littlefs_readdir_r(ctx, pdir, &dir->e, &out_dirent);
    if (res != 0) return NULL;
    return out_dirent;
}

static int vfs_littlefs_readdir_r(void* ctx, xf_vfs_dir_t* pdir,
        xf_vfs_dirent_t* entry, xf_vfs_dirent_t** out_dirent) {
    _ASSERT(pdir);
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    vfs_littlefs_dir_t * dir = (vfs_littlefs_dir_t *) pdir;
    int res;
    struct lfs_info info = { 0 };

    sem_take(efs);
    do{ /* Read until we get a real object name */
        res = lfs_dir_read(efs->fs, &dir->d, &info);
    }while( res>0 && (strcmp(info.name, ".") == 0 || strcmp(info.name, "..") == 0));
    sem_give(efs);
    if (res < 0) {
        errno = lfs_errno_remap(res);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to readdir \"%s\". Error %s (%d)",
                dir->path, xf_littlefs_errno(res), res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to readdir \"%s\". Error %d", dir->path, res);
#endif
        return -1;
    }

    if(info.type == LFS_TYPE_REG) {
        XF_LOGV(XF_LITTLEFS_TAG, "readdir a file of size %u named \"%s\"",
                (unsigned int) info.size, info.name);
    }
    else {
        XF_LOGV(XF_LITTLEFS_TAG, "readdir a dir named \"%s\"", info.name);
    }

    if(res == 0) {
        /* End of Objs */
        XF_LOGV(XF_LITTLEFS_TAG, "Reached the end of the directory.");
        *out_dirent = NULL;
    }
    else {
        entry->d_ino = 0;
        entry->d_type = info.type == LFS_TYPE_REG ? XF_VFS_DT_REG : XF_VFS_DT_DIR;
        strncpy(entry->d_name, info.name, sizeof(entry->d_name));
        *out_dirent = entry;
    }
    dir->offset++;

    return 0;
}

static long vfs_littlefs_telldir(void* ctx, xf_vfs_dir_t* pdir) {
    _ASSERT(pdir);
    vfs_littlefs_dir_t * dir = (vfs_littlefs_dir_t *) pdir;
    return dir->offset;
}

static void vfs_littlefs_seekdir(void* ctx, xf_vfs_dir_t* pdir, long offset) {
    _ASSERT(pdir);
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    vfs_littlefs_dir_t * dir = (vfs_littlefs_dir_t *) pdir;
    int res;

    if (offset < dir->offset) {
        /* close and re-open dir to rewind to beginning */
        sem_take(efs);
        res = lfs_dir_rewind(efs->fs, &dir->d);
        sem_give(efs);
        if (res < 0) {
            errno = lfs_errno_remap(res);
            XF_LOGV(XF_LITTLEFS_TAG, "Failed to rewind dir \"%s\". Error %s (%d)",
                    dir->path, xf_littlefs_errno(res), res);
            return;
        }
        dir->offset = 0;
    }

    while(dir->offset < offset){
        xf_vfs_dirent_t *out_dirent;
        res = vfs_littlefs_readdir_r(ctx, pdir, &dir->e, &out_dirent);
        if( res != 0 ){
            XF_LOGE(XF_LITTLEFS_TAG, "Error readdir_r");
            return;
        }
    }
}

static int vfs_littlefs_mkdir(void* ctx, const char* name, xf_vfs_mode_t mode) {
    /* Note: mode is currently unused */
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    int res;
    XF_LOGV(XF_LITTLEFS_TAG, "mkdir \"%s\"", name);

    sem_take(efs);
    res = lfs_mkdir(efs->fs, name);
    sem_give(efs);
    if (res < 0) {
        errno = lfs_errno_remap(res);
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to mkdir \"%s\". Error %s (%d)",
                name, xf_littlefs_errno(res), res);
        return -1;
    }
    return 0;
}

static int vfs_littlefs_rmdir(void* ctx, const char* name) {
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    struct lfs_info info;
    int res;

    /* Error Checking */
    sem_take(efs);
    res = lfs_stat(efs->fs, name, &info);
    if (res < 0) {
        errno = lfs_errno_remap(res);
        sem_give(efs);
        XF_LOGV(XF_LITTLEFS_TAG, "\"%s\" doesn't exist.", name);
        return -1;
    }

    if (info.type != LFS_TYPE_DIR) {
        sem_give(efs);
        XF_LOGV(XF_LITTLEFS_TAG, "\"%s\" is not a directory.", name);
        errno = ENOTDIR;
        return -1;
    }

    /* Unlink the dir */
    res = lfs_remove(efs->fs, name);
    sem_give(efs);
    if ( res < 0) {
        errno = lfs_errno_remap(res);
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to unlink path \"%s\". Error %s (%d)",
                name, xf_littlefs_errno(res), res);
        return -1;
    }

    return 0;
}

static xf_vfs_ssize_t vfs_littlefs_truncate( void *ctx, const char *path, xf_vfs_off_t size )
{
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    xf_vfs_ssize_t res = -1;
    vfs_littlefs_file_t *file = NULL;

    int fd = vfs_littlefs_open( ctx, path, LFS_O_RDWR, 438 );

    sem_take(efs);
    if((uint32_t)fd > efs->cache_size)
    {
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "FD %d must be <%d.", fd, efs->cache_size);
        errno = EBADF;
        return -1;
    }
    file = efs->cache[fd];
    res = lfs_file_truncate( efs->fs, &file->file, size );
    sem_give(efs);

    if(res < 0)
    {
        errno = lfs_errno_remap(res);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to truncate file \"%s\". Error %s (%d)",
                file->path, xf_littlefs_errno(res), res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to truncate FD %d. Error %s (%d)",
                fd, xf_littlefs_errno(res), res);
#endif
        res = -1;
    }
    else
    {
        XF_LOGV( XF_LITTLEFS_TAG, "Truncated file %s to %u bytes", path, (unsigned int) size );
    }
    vfs_littlefs_close( ctx, fd );
    return res;
}

#ifdef XF_LITTLEFS_ENABLE_FTRUNCATE
static int vfs_littlefs_ftruncate(void *ctx, int fd, xf_vfs_off_t size)
{
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    xf_vfs_ssize_t res;
    vfs_littlefs_file_t *file = NULL;

    sem_take(efs);
    if((uint32_t)fd > efs->cache_size) {
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "FD %d must be <%d.", fd, efs->cache_size);
        errno = EBADF;
        return -1;
    }
    file = efs->cache[fd];
    res = lfs_file_truncate( efs->fs, &file->file, size );
    sem_give(efs);

    if(res < 0)
    {
        errno = lfs_errno_remap(res);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to truncate file \"%s\". Error %s (%d)",
                file->path, xf_littlefs_errno(res), res);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to truncate FD %d. Error %s (%d)",
                fd, xf_littlefs_errno(res), res);
#endif
        res = -1;
    }
    else
    {
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV( XF_LITTLEFS_TAG, "Truncated file %s to %u bytes", file->path, (unsigned int) size );
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Truncated FD %d to %u bytes", fd, (unsigned int) size );
#endif
    }
    return res;
}
#endif // XF_LITTLEFS_ENABLE_FTRUNCATE
#endif //XF_VFS_SUPPORT_DIR_IS_ENABLE

#if CONFIG_LITTLEFS_USE_MTIME
/**
 * Sets the mtime attr to t.
 */
static int vfs_littlefs_update_mtime_value(xf_littlefs_t *efs, const char *path, xf_vfs_time_t t)
{
    int res;
    res = lfs_setattr(efs->fs, path, LITTLEFS_ATTR_MTIME,
            &t, sizeof(t));
    if( res < 0 ) {
        errno = lfs_errno_remap(res);
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to update mtime (%d)", res);
        return -1;
    }

    return res;
}

/**
 * Sets the mtime attr to an appropriate value
 */
static void vfs_littlefs_update_mtime(xf_littlefs_t *efs, const char *path)
{
    vfs_littlefs_utime(efs, path, NULL);
}

static int vfs_littlefs_utime(void *ctx, const char *path, const xf_vfs_utimbuf_t *times)
{
    xf_littlefs_t * efs = (xf_littlefs_t *)ctx;
    xf_vfs_time_t t;

    _ASSERT(path);

    sem_take(efs);
    if (times) {
        t = times->modtime;
    } else {
#if CONFIG_LITTLEFS_MTIME_USE_SECONDS
        // use current time
        // t = time(NULL);
        t = (xf_vfs_time_t)xf_sys_time_get_ms();
#elif CONFIG_LITTLEFS_MTIME_USE_NONCE
        _ASSERT( sizeof(xf_vfs_time_t) == 4 );
        t = vfs_littlefs_get_mtime(efs, path);
        if( 0 == t ) t = xf_random();
        else t += 1;

        if( 0 == t ) t = 1;
#else
#error "Invalid MTIME configuration"
#endif
    }

    int ret = vfs_littlefs_update_mtime_value(efs, path, t);
    sem_give(efs);
    return ret;
}

static xf_vfs_time_t vfs_littlefs_get_mtime(xf_littlefs_t *efs, const char *path)
{
    xf_vfs_time_t t;
    int size;
    size = lfs_getattr(efs->fs, path, LITTLEFS_ATTR_MTIME,
            &t, sizeof(t));
    if( size < 0 ) {
        errno = lfs_errno_remap(size);
#ifndef CONFIG_LITTLEFS_USE_ONLY_HASH
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to get mtime attribute %s (%d)",
                xf_littlefs_errno(size), size);
#else
        XF_LOGV(XF_LITTLEFS_TAG, "Failed to get mtime attribute %d", size);
#endif
        return -1;
    }
    return t;
}
#endif //CONFIG_LITTLEFS_USE_MTIME

#if CONFIG_LITTLEFS_SPIFFS_COMPAT
static size_t my_strlcpy(char *dst, const char *src, size_t dst_size) 
{
    size_t src_len = xf_strlen(src);
    if (dst_size > 0) {
        size_t copy_len = (src_len >= dst_size) ? (dst_size - 1) : src_len;
        xf_strncpy(dst, src, copy_len); // 拷贝有效部分
        dst[copy_len] = '\0';       // 确保以 NULL 终止
    }
    return src_len; // 返回源字符串的长度
}

/**
 * @brief Recursively make all parent directories for a file.
 * @param[in] dir Path of directories to make up to. The last element
 * of the path is assumed to be the file and IS NOT created.
 *   e.g.
 *       "foo/bar/baz"
 *   will create directories "foo" and "bar"
 */
static void mkdirs(xf_littlefs_t * efs, const char *dir) {
    char tmp[CONFIG_LITTLEFS_OBJ_NAME_LEN];
    char *p = NULL;

    my_strlcpy(tmp, dir, sizeof(tmp));
    for(p = tmp + 1; *p; p++) {
        if(*p == '/') {
            *p = '\0';
            vfs_littlefs_mkdir((void*)efs, tmp, XF_VFS_S_IRWXU);
            *p = '/';
        }
    }
}

/**
 * @brief Recursively attempt to delete all empty directories for a file.
 * @param[in] dir Path of directories to delete. The last element of the path
 * is assumed to be the file and IS NOT deleted.
 *   e.g.
 *       "foo/bar/baz"
 *   will attempt to delete directories (in order):
 *       1. "foo/bar/baz"
 *       2. "foo/bar"
 *       3. "foo"
 */

static void rmdirs(xf_littlefs_t * efs, const char *dir) {
    char tmp[CONFIG_LITTLEFS_OBJ_NAME_LEN];
    char *p = NULL;

    my_strlcpy(tmp, dir, sizeof(tmp));
    for(p = tmp + strlen(tmp) - 1; p != tmp; p--) {
        if(*p == '/') {
            *p = '\0';
            vfs_littlefs_rmdir((void*)efs, tmp);
            *p = '/';
        }
    }
}

#endif  // CONFIG_LITTLEFS_SPIFFS_COMPAT

static int vfs_littlefs_fcntl(void* ctx, int fd, int cmd, int arg)
{
    int result = 0;
    xf_littlefs_t *efs = (xf_littlefs_t *)ctx;
    lfs_file_t *lfs_file = NULL;
    vfs_littlefs_file_t *file = NULL;
    const uint32_t flags_mask = LFS_O_WRONLY | LFS_O_RDONLY | LFS_O_RDWR;

    sem_take(efs);
    if((uint32_t)fd > efs->cache_size) {
        sem_give(efs);
        XF_LOGE(XF_LITTLEFS_TAG, "FD %d must be <%d.", fd, efs->cache_size);
        errno = EBADF;
        return -1;
    }

    file = efs->cache[fd];
    if (file) {
        lfs_file = &efs->cache[fd]->file;
    } else {
        sem_give(efs);
        errno = EBADF;
        return -1;
    }

    if (cmd == XF_VFS_F_GETFL) {
        if ((lfs_file->flags & flags_mask) == LFS_O_WRONLY) {
            result = XF_VFS_O_WRONLY;
        } else if ((lfs_file->flags & flags_mask) == LFS_O_RDONLY) {
            result = XF_VFS_O_RDONLY;
        } else if ((lfs_file->flags & flags_mask) == LFS_O_RDWR) {
            result = XF_VFS_O_RDWR;
        }
    }
#ifdef CONFIG_LITTLEFS_FCNTL_GET_PATH
    else if (cmd == XF_VFS_F_GETPATH) {
        char *buffer = (char *)(uintptr_t)arg;

        _ASSERT(buffer);
        if (snprintf(buffer, MAXPATHLEN, "%s%s", efs->base_path, file->path) > 0) {
            result = 0;
        } else {
            result = -1;
            errno = EINVAL;
        }
    }
#endif
    else {
        result = -1;
        errno = ENOSYS;
    }

    sem_give(efs);

    return result;
}
