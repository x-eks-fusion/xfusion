/**
 * @file port_xf_netif.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-11
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __PORT_XF_NETIF_H__
#define __PORT_XF_NETIF_H__

/* ==================== [Includes] ========================================== */

#include "esp_event.h"
#include "esp_wifi.h"

#include "esp_netif.h"

#include "port_common.h"

#include "xf_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

typedef struct port_xf_netif_hdl_ctx_s {
    esp_netif_t *esp_netif;
} port_xf_netif_hdl_ctx_t;

typedef struct port_xf_netif_context_s {
    bool b_netif_inited;
} port_xf_netif_context_t;

/* ==================== [Global Prototypes] ================================= */

port_xf_netif_context_t *port_xf_netif_get_context(void);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __PORT_XF_NETIF_H__ */
