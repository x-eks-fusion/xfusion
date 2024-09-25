/**
 * @file port_xf_wifi.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __PORT_XF_WIFI_H__
#define __PORT_XF_WIFI_H__

/* ==================== [Includes] ========================================== */

#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "port_common.h"
#include "port_xf_netif.h"

#include "xf_utils.h"
#include "xf_wifi.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

typedef struct port_xf_wifi_context_s {
    /* ap */
    xf_wifi_cb_t    ap_cb;
    void            *ap_user_args;
    /* sta */
    xf_wifi_cb_t    sta_cb;
    void            *sta_user_args;

    uint32_t b_inited           : 1;
    uint32_t b_ap_inited        : 1;
    uint32_t b_sta_inited       : 1;
    uint32_t b_ap_started       : 1;
    uint32_t b_sta_started      : 1;
    uint32_t b_sta_connected    : 1;
    uint32_t b_scanning         : 1;
    uint32_t scanning_ch        : 8;
    uint32_t reserve            : 19;

    port_xf_netif_hdl_ctx_t netif_ap;
    port_xf_netif_hdl_ctx_t netif_sta;

    xf_ip_cb_t  ap_ip_cb;
    void        *ap_ip_user_args;
    xf_ip_cb_t  sta_ip_cb;
    void        *sta_ip_user_args;

    bool b_ap_netif_is_inited;
    bool b_sta_netif_is_inited;

    esp_event_handler_instance_t instance_ap_event;
    esp_event_handler_instance_t instance_sta_event;

    esp_event_handler_instance_t instance_ap_ip_event;
    esp_event_handler_instance_t instance_sta_ip_event;

    wifi_config_t sta_cfg;
} port_xf_wifi_context_t;

/* ==================== [Global Prototypes] ================================= */

port_xf_wifi_context_t *port_xf_wifi_get_context(void);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __PORT_XF_WIFI_H__ */
