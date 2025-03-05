/**
 * @file littlefs_api.c
 * @brief Maps the HAL of xf_fal_partition <-> littlefs
 * @author Brian Pugh
 */

#include "xf_utils.h"
#include "xf_sys.h"
#include "xf_vfs.h"
#include "xf_fal.h"
#include "xf_lfs_api.h"
#include "xf_vfs_littlefs.h"

int littlefs_xf_part_read(const struct lfs_config *c, lfs_block_t block,
                           lfs_off_t off, void *buffer, lfs_size_t size) {
    xf_littlefs_t * efs = c->context;
    size_t part_off = (block * c->block_size) + off;
    xf_err_t err = xf_fal_partition_read(efs->partition, part_off, buffer, size);
    if (err) {
        XF_LOGE(XF_LITTLEFS_TAG, "failed to read addr %08x, size %08x, err %d", (unsigned int) part_off, (unsigned int) size, err);
        return LFS_ERR_IO;
    }
    return 0;
}

int littlefs_xf_part_write(const struct lfs_config *c, lfs_block_t block,
                            lfs_off_t off, const void *buffer, lfs_size_t size) {
    xf_littlefs_t * efs = c->context;
    size_t part_off = (block * c->block_size) + off;
    xf_err_t err = xf_fal_partition_write(efs->partition, part_off, buffer, size);
    if (err) {
        XF_LOGE(XF_LITTLEFS_TAG, "failed to write addr %08x, size %08x, err %d", (unsigned int) part_off, (unsigned int) size, err);
        return LFS_ERR_IO;
    }
    return 0;
}

int littlefs_xf_part_erase(const struct lfs_config *c, lfs_block_t block) {
    xf_littlefs_t * efs = c->context;
    size_t part_off = block * c->block_size;
    xf_err_t err = xf_fal_partition_erase(efs->partition, part_off, c->block_size);
    if (err) {
        XF_LOGE(XF_LITTLEFS_TAG, "failed to erase addr %08x, size %08x, err %d", (unsigned int) part_off, (unsigned int) c->block_size, err);
        return LFS_ERR_IO;
    }
    return 0;

}

int littlefs_xf_part_sync(const struct lfs_config *c) {
    /* Unnecessary */
    return 0;
}

