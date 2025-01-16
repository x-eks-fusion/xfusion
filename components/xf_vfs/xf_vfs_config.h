/**
 * @file xf_vfs_config.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief 使用 xfusion 菜单配置 xf_vfs 内部配置。
 * @version 1.0
 * @date 2025-01-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __XF_VFS_CONFIG_H__
#define __XF_VFS_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_VFS_SUPPORT_IO_ENABLE CONFIG_XF_VFS_SUPPORT_IO_ENABLE
#define XF_VFS_SUPPORT_DIR_ENABLE CONFIG_XF_VFS_SUPPORT_DIR_ENABLE
#define XF_VFS_SUPPORT_SELECT_ENABLE CONFIG_XF_VFS_SUPPORT_SELECT_ENABLE
#define XF_VFS_MAX_COUNT CONFIG_XF_VFS_MAX_COUNT
#define XF_VFS_CUSTOM_FD_SETSIZE_ENABLE CONFIG_XF_VFS_CUSTOM_FD_SETSIZE_ENABLE
#define XF_VFS_CUSTOM_FD_SETSIZE CONFIG_XF_VFS_CUSTOM_FD_SETSIZE
#define XF_VFS_PATH_MAX CONFIG_XF_VFS_PATH_MAX
#define XF_VFS_DIRENT_NAME_SIZE CONFIG_XF_VFS_DIRENT_NAME_SIZE

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_VFS_CONFIG_H__
