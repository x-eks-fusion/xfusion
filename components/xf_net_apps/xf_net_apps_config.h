/**
 * @file xf_net_apps_config.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief 使用 xfusion 菜单配置 xf_net_apps 内部配置。
 * @version 0.1
 * @date 2024-10-11
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_NET_APPS_CONFIG_H__
#define __XF_NET_APPS_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_NET_APPS_IPERF_ENABLE        CONFIG_XF_NET_APPS_IPERF_ENABLE
#define XF_NET_APPS_PING_ENABLE         CONFIG_XF_NET_APPS_PING_ENABLE

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_NET_APPS_CONFIG_H__
