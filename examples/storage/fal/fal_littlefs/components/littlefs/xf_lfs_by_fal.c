/**
 * @file xf_lfs_by_fal.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2025-01-03
 *
 * Copyright (c) 2025, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "lfs.h"

#include "xf_lfs_by_fal.h"

/* ==================== [Defines] =========================================== */

#define TAG "xf_lfs_by_fal"

/* ==================== [Typedefs] ========================================== */

typedef struct xf_lfs_by_fal_ctx_s {
    const xf_fal_partition_t   *p_part;
    lfs_t                       lfs_ctx;
    struct lfs_config           lfs_cfg;
} xf_lfs_by_fal_ctx_t;

/* ==================== [Static Prototypes] ================================= */

static int port_lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
static int port_lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer,
                         lfs_size_t size);
static int port_lfs_erase(const struct lfs_config *c, lfs_block_t block);
static int port_lfs_sync(const struct lfs_config *c);

/* ==================== [Static Variables] ================================== */

static xf_lfs_by_fal_ctx_t s_xf_lfs_by_fal_ctx = {0};
static xf_lfs_by_fal_ctx_t *sp_xf_lfs_by_fal_ctx = &s_xf_lfs_by_fal_ctx;
#define ctx() sp_xf_lfs_by_fal_ctx

/* ==================== [Macros] ============================================ */

#if defined(CONFIG_PORT_LFS_DEBUG_ENABLE) && (CONFIG_PORT_LFS_DEBUG_ENABLE)
#define LFS_DBG(format, ...)    xf_log_printf(format "\r\n", ##__VA_ARGS__)
#else
#define LFS_DBG(...)
#endif

/* ==================== [Global Functions] ================================== */

xf_err_t xf_lfs_init(const char *p_partition_name, bool format_if_err)
{
    xf_err_t                    xf_ret = XF_OK;
    const char                 *p_part_name;
    const xf_fal_partition_t   *p_part;
    int                         lfs_err = 0;

    /* 初始化 FAL */
    xf_ret = xf_fal_init();
    if ((xf_ret != XF_OK) && (xf_ret != XF_ERR_INITED)) {
        XF_LOGI(TAG, "FAL init failed: %s", xf_err_to_name(xf_ret));
        goto l_xf_ret;
    }

#if defined(CONFIG_PORT_LFS_SHOW_XF_PART_TABLE_ENABLE) && (CONFIG_PORT_LFS_SHOW_XF_PART_TABLE_ENABLE)
    xf_fal_show_part_table();
#endif

    /* 获取分区 */
    if ((p_partition_name != NULL) && (xf_strlen(p_partition_name) != 0)) {
        p_part_name = p_partition_name;
    } else {
        p_part_name = CONFIG_PORT_LFS_XF_FAL_PARTITION_NAME;
    }
    p_part = xf_fal_partition_find(p_part_name);
    if (NULL == p_part) {
        XF_LOGE(TAG, "Partition not found!");
        xf_ret = XF_ERR_NOT_FOUND;
        goto l_xf_ret;
    }

    ctx()->p_part = p_part;

    ctx()->lfs_cfg.read             = port_lfs_read;
    ctx()->lfs_cfg.prog             = port_lfs_prog;
    ctx()->lfs_cfg.erase            = port_lfs_erase;
    ctx()->lfs_cfg.sync             = port_lfs_sync;

#if defined(CONFIG_PORT_LFS_CUSTOM_BLOCK_ENABLE) && (CONFIG_PORT_LFS_CUSTOM_BLOCK_ENABLE)
    ctx()->lfs_cfg.block_size       = CONFIG_PORT_LFS_CUSTOM_BLOCK_SIZE;
    ctx()->lfs_cfg.block_count      = CONFIG_PORT_LFS_CUSTOM_BLOCK_COUNT;
    ctx()->lfs_cfg.read_size        = CONFIG_PORT_LFS_READ_SIZE;
    ctx()->lfs_cfg.prog_size        = CONFIG_PORT_LFS_WRITE_SIZE;
#else
    const xf_fal_flash_dev_t *p_flash_dev = xf_fal_flash_device_find_by_part(p_part);
    if (NULL == p_flash_dev) {
        XF_LOGE(TAG, "flash device not found.");
        xf_ret = XF_ERR_NOT_FOUND;
        goto l_xf_ret;
    }
    if ((p_flash_dev->sector_size == 0)
            || (p_flash_dev->page_size == 0)
       ) {
        XF_LOGE(TAG, "Flash device parameter error.");
        XF_LOGE(TAG, "sector_size:%d", (int)p_flash_dev->sector_size);
        XF_LOGE(TAG, "page_size:%d", (int)p_flash_dev->page_size);
        xf_ret = XF_FAIL;
        goto l_xf_ret;
    }
    ctx()->lfs_cfg.block_size       = p_flash_dev->sector_size;
    ctx()->lfs_cfg.block_count      = p_part->len / p_flash_dev->sector_size;
    if (ctx()->lfs_cfg.block_count == 0) {
        XF_LOGE(TAG, "The partition length is too short.");
        xf_ret = XF_FAIL;
        goto l_xf_ret;
    }
    ctx()->lfs_cfg.read_size        = p_flash_dev->page_size;
    ctx()->lfs_cfg.prog_size        = p_flash_dev->page_size;
#endif

    ctx()->lfs_cfg.lookahead_size   = CONFIG_PORT_LFS_LOOKAHEAD_SIZE;
    ctx()->lfs_cfg.cache_size       = CONFIG_PORT_LFS_CACHE_SIZE;
    ctx()->lfs_cfg.block_cycles     = CONFIG_PORT_LFS_BLOCK_CYCLES;

    lfs_err = lfs_mount(&ctx()->lfs_ctx, &ctx()->lfs_cfg);

    if ((lfs_err) && (format_if_err)) {
        lfs_err = lfs_format(&ctx()->lfs_ctx, &ctx()->lfs_cfg);
        if (lfs_err != LFS_ERR_OK) {
            XF_LOGE(TAG, "Formatting failed.");
        }
        lfs_err = lfs_mount(&ctx()->lfs_ctx, &ctx()->lfs_cfg);
        if (lfs_err != LFS_ERR_OK) {
            XF_LOGE(TAG, "Failed to mount again.");
        }
    }

    if (lfs_err == LFS_ERR_OK) {
        xf_ret = XF_OK;
    } else {
        xf_ret = XF_FAIL;
    }

l_xf_ret:;
    return xf_ret;
}

lfs_t *xf_lfs_get_lfs_handle(void)
{
    return &ctx()->lfs_ctx;
}

/* ==================== [Static Functions] ================================== */

static int port_lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    LFS_DBG("LittleFS Read b = 0x%08lx o = 0x%08lx s = 0x%08lx", block, off, size);
    xf_err_t xf_ret;
    size_t part_off = (block * c->block_size) + off;
    xf_ret = xf_fal_partition_read(ctx()->p_part, part_off, buffer, size);
    if (xf_ret != XF_OK) {
        XF_LOGE(TAG, "littlefs_read ret:%s", xf_err_to_name(xf_ret));
        XF_LOGE(TAG, "block:%d", (int)block);
        XF_LOGE(TAG, "part_off:%d", (int)part_off);
        XF_LOGE(TAG, "buffer:%p", buffer);
        XF_LOGE(TAG, "size:%d", (int)size);
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

static int port_lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer,
                         lfs_size_t size)
{
    LFS_DBG("LittleFS Prog b = 0x%08lx o = 0x%08lx s = 0x%08lx", block, off, size);
    xf_err_t xf_ret;
    size_t part_off = (block * c->block_size) + off;
    xf_ret = xf_fal_partition_write(ctx()->p_part, part_off, buffer, size);
    if (xf_ret != XF_OK) {
        XF_LOGE(TAG, "port_lfs_prog ret:%s", xf_err_to_name(xf_ret));
        XF_LOGE(TAG, "block:%d", (int)block);
        XF_LOGE(TAG, "part_off:%d", (int)part_off);
        XF_LOGE(TAG, "buffer:%p", buffer);
        XF_LOGE(TAG, "size:%d", (int)size);
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

static int port_lfs_erase(const struct lfs_config *c, lfs_block_t block)
{
    LFS_DBG("LittleFS Erase b = 0x%08lx", block);
    xf_err_t xf_ret;
    size_t part_off = block * c->block_size;
    xf_ret = xf_fal_partition_erase(ctx()->p_part, part_off, c->block_size);
    if (xf_ret != XF_OK) {
        XF_LOGE(TAG, "port_lfs_prog ret:%s", xf_err_to_name(xf_ret));
        XF_LOGE(TAG, "block:%d", (int)block);
        XF_LOGE(TAG, "part_off:%d", (int)part_off);
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

static int port_lfs_sync(const struct lfs_config *c)
{
    LFS_DBG("LittleFS Sync");
    return LFS_ERR_OK;
}
