/**
 * @file lfs_util.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief 此文件仅用于包含 "littlefs/lfs_util.h".
 *        由于 `littlefs/lfs_util.h` 使用自定义头文件时需要设置 `-D`，
 *        考虑到不是所有平台都对接了 cflags 配置，故使用此方式覆盖头文件。
 * @version 1.0
 * @date 2024-12-16
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#if !defined(LFS_CONFIG)
/* 不需要 `"`，`littlefs/lfs_util.h` 内部字符串化 */
#   define LFS_CONFIG lfs_config.h
#endif

#include "littlefs/lfs_util.h"
