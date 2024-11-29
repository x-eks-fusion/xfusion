/**
 * @file xf_net_apps_config_internal.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief xf_netif 模块内部配置总头文件。
 *        确保 xf_netif_config.h 的所有定义都有默认值。
 * @version 1.0
 * @date 2024-09-24
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_NET_APPS_CONFIG_INTERNAL_H__
#define __XF_NET_APPS_CONFIG_INTERNAL_H__

/* ==================== [Includes] ========================================== */

#include "xf_net_apps_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#if !defined(XF_NET_APPS_IPERF_ENABLE) || (XF_NET_APPS_IPERF_ENABLE) || defined(__DOXYGEN__)
#   define XF_NET_APPS_IPERF_IS_ENABLE      (1)
#else
#   define XF_NET_APPS_IPERF_IS_ENABLE      (0)
#endif

#if !defined(XF_NET_APPS_PING_ENABLE) || (XF_NET_APPS_PING_ENABLE) || defined(__DOXYGEN__)
#   define XF_NET_APPS_PING_IS_ENABLE       (1)
#else
#   define XF_NET_APPS_PING_IS_ENABLE       (0)
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_NET_APPS_CONFIG_INTERNAL_H__
