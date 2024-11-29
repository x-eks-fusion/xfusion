/**
 * @file xf_mac.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-11
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_MAC_H__
#define __XF_MAC_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/**
 * @brief mac 地址最大长度。
 */
#define XF_MAC_LEN_MAX      (6)

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifndef XF_MAC2STR
#   define XF_MAC2STR(a)    (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#endif
#ifndef XF_MACSTR
#   define XF_MACSTR        "%02x:%02x:%02x:%02x:%02x:%02x"
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_MAC_H__ */
