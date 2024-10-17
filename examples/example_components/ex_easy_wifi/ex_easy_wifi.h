/**
 * @file ex_easy_wifi.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-09-30
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __EX_EASY_WIFI_H__
#define __EX_EASY_WIFI_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#include "xf_wifi.h"
#include "xf_netif.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

xf_err_t ex_easy_wifi_ap(void);
xf_ip4_addr_t ex_easy_wifi_ap_get_last_sta_ip(void);
xf_ip4_addr_t ex_easy_wifi_ap_get_onw_ip(void);

xf_err_t ex_easy_wifi_sta(void);
bool ex_easy_wifi_sta_is_connected(void);
bool ex_easy_wifi_sta_got_ip(void);
xf_ip4_addr_t ex_easy_wifi_sta_get_gw_ip(void);
xf_ip4_addr_t ex_easy_wifi_sta_get_onw_ip(void);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EX_EASY_WIFI_H__ */
