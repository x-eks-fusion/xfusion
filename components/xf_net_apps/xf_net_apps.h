/**
 * @file xf_nal_config.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-11
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#if defined(__DOXYGEN__) && defined(__XFUSION__) && defined(__DOXYGEN_ADD_COMPONENTS__)
/* 用于将组件内部 doxygen 组添加到 xfusion 文档的 `components_internal` 组中。 */
/**
 * @cond (XFAPI_USER || XFAPI_PORT || XFAPI_INTERNAL)
 * @ingroup group_components_internal
 * @defgroup group_xf_net_apps xf_net_apps
 * @brief 网络应用。
 * @endcond
 */
#endif

#ifndef __XF_NET_APPS_H__
#define __XF_NET_APPS_H__

/* ==================== [Includes] ========================================== */

#include "xf_net_apps_config_internal.h"

#if XF_NET_APPS_IPERF_IS_ENABLE
#include "xf_iperf.h"
#endif

#if XF_NET_APPS_PING_IS_ENABLE
#include "xf_ping.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_NET_APPS_H__
