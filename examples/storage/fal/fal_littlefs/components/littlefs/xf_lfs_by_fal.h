/**
 * @file xf_lfs_by_fal.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2025-01-03
 *
 * Copyright (c) 2025, CorAL. All rights reserved.
 *
 */

#ifndef __XF_LFS_BY_FAL_H__
#define __XF_LFS_BY_FAL_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_fal.h"

#include "lfs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 对接到 xf_fal 并初始化 littlefs.
 *
 * @param p_partition_name      注册到 xf_fal 中的分区名。
 *                              可以为 NULL, 此时使用 CONFIG_PORT_LFS_XF_FAL_PARTITION_NAME 配置。
 * @param format_if_err         如果挂载错误，是否格式化对应的分区。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_ERR_NOT_FOUND      未找到对应的分区或者 flash 设备
 *      - XF_ERR_TIMEOUT        指定时间内未接收到接收端请求
 *      - XF_FAIL               失败，可能是 flash 设备或者分区参数错误
 */
xf_err_t xf_lfs_init(const char *p_partition_name, bool format_if_err);

/**
 * @brief 获取 littlefs 句柄。
 *
 * @return lfs_t* littlefs 句柄。
 */
lfs_t *xf_lfs_get_lfs_handle(void);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_LFS_BY_FAL_H__ */
