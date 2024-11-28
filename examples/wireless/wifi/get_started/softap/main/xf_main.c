/**
 * @file xf_main.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/**
 * @example examples/wireless/wifi/get_started/softap/main/xf_main.c
 * xf_wal wifi softap 示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_wifi.h"

/* ==================== [Defines] =========================================== */

#define EXAMPLE_WIFI_SSID               "myssid"
#define EXAMPLE_WIFI_PASS               "mypassword"
#define EXAMPLE_WIFI_AUTHMODE           XF_WIFI_AUTH_WPA_WPA2_PSK
#define EXAMPLE_WIFI_CHANNEL            6 /*!< wifi 信道 */
#define EXAMPLE_WIFI_SSID_HIDDEN_FLAG   0 /*!< 不隐藏 */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void _error_check_failed(void);
static void _wifi_ap_event_handler(
    xf_wifi_event_id_t event_id, void *event_data, void *user_args);
static void _wifi_ip_event_handler(
    xf_ip_event_id_t event_id, void *event_data, void *user_args);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "ap";
static xf_wifi_ap_cfg_t s_ap_cfg = {
    .ssid           = EXAMPLE_WIFI_SSID,
    .password       = EXAMPLE_WIFI_PASS,
    .authmode       = EXAMPLE_WIFI_AUTHMODE,
    .channel        = EXAMPLE_WIFI_CHANNEL,
    .ssid_hidden    = EXAMPLE_WIFI_SSID_HIDDEN_FLAG,
    .p_cfg_ext      = NULL,             /*!< 不使用扩展配置 */
    .p_static_ip    = NULL,             /*!< 不使用静态 IP */
};

/* ==================== [Macros] ============================================ */

#define ERROR_CHECK(expression) \
    do { \
        if (unlikely((expression) != XF_OK)) { \
            XF_LOGE(TAG, "An error occurred: %s", XSTR(expression != XF_OK)); \
            _error_check_failed(); \
        } \
    } while (0)

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    /* 1. 使能 wifi 协议栈. */
    ERROR_CHECK(xf_wifi_enable());

    /* 2. 设置事件回调. */
    ERROR_CHECK(xf_wifi_ap_set_cb(_wifi_ap_event_handler, NULL));
    ERROR_CHECK(xf_wifi_ap_set_ip_cb(_wifi_ip_event_handler, NULL));

    /* 3. 配置并开始 ap. */
    ERROR_CHECK(xf_wifi_ap_init(&s_ap_cfg));
}

/* ==================== [Static Functions] ================================== */

static void _wifi_ap_event_handler(
    xf_wifi_event_id_t event_id, void *event_data, void *user_args)
{
    UNUSED(user_args);
    switch (event_id) {
    case XF_WIFI_EVENT_AP_START: {
        XF_LOGI(TAG, "AP has started.");
    } break;
    case XF_WIFI_EVENT_AP_STOP: {
        XF_LOGI(TAG, "AP has stopped.");
    } break;
    case XF_WIFI_EVENT_AP_STA_CONNECTED: {
        xf_wifi_event_ap_sta_conn_t *e = (xf_wifi_event_ap_sta_conn_t *)event_data;
        XF_LOGI(TAG, "station "XF_MACSTR" join", XF_MAC2STR(e->mac));
    } break;
    case XF_WIFI_EVENT_AP_STA_DISCONNECTED: {
        xf_wifi_event_ap_sta_disconn_t *e = (xf_wifi_event_ap_sta_disconn_t *)event_data;
        XF_LOGI(TAG, "station "XF_MACSTR" leave", XF_MAC2STR(e->mac));
    } break;
    default:
        break;
    }
}

static void _wifi_ip_event_handler(
    xf_ip_event_id_t event_id, void *event_data, void *user_args)
{
    UNUSED(user_args);
    switch (event_id) {
    case XF_IP_EVENT_IP_ASSIGNED: {
        xf_ip_event_ip_assigned_t *e = (xf_ip_event_ip_assigned_t *)event_data;
        XF_LOGI(TAG, "Assign ip " XF_IPSTR " to site " XF_MACSTR,
                XF_IP2STR(&e->ip), XF_MAC2STR(e->mac));
    } break;
    default:
        break;
    }
}

static void _error_check_failed(void)
{
    XF_LOGI(TAG, "Run-time error.");
    for (;;) {
    }
}
