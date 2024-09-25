/**
 * @file xf_netif_config_internal.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief xf_netif 模块内部配置总头文件。
 *        确保 xf_netif_config.h 的所有定义都有默认值。
 * @version 1.0
 * @date 2024-09-24
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_NETIF_CONFIG_INTERNAL_H__
#define __XF_NETIF_CONFIG_INTERNAL_H__

/* ==================== [Includes] ========================================== */

#include "xf_nal_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#ifndef XF_NAL_BIG_ENDIAN_IS_ENABLE
#   if  !defined(CONFIG_XF_NAL_BIG_ENDIAN_ENABLE) || (CONFIG_XF_NAL_BIG_ENDIAN_ENABLE == 0)
#       define XF_NAL_BIG_ENDIAN_IS_ENABLE      0
#   else
#       define XF_NAL_BIG_ENDIAN_IS_ENABLE      1
#   endif
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_NETIF_CONFIG_INTERNAL_H__
