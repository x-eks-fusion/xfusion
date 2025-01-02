/**
 * @file ex_easy_wifi.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-09-30
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "ex_easy_wifi.h"

#include "xf_osal.h"

/* ==================== [Defines] =========================================== */

#define EXAMPLE_WIFI_AUTHMODE           XF_WIFI_AUTH_WPA_WPA2_PSK
#define EXAMPLE_WIFI_CHANNEL            6 /*!< wifi 信道 */
#define EXAMPLE_WIFI_SSID_HIDDEN_FLAG   0 /*!< 不隐藏 */

/*
    TODO Kconfig 如果 string 类型输入 "\xaa\xbb\xcc\xdd\xee\xff"
    要么转义成 "\\xaa\\xbb\\xcc\\xdd\\xee\\xff",
    要么变成   "xaaxbbxccxddxeexff"
 */
#if !defined(EX_EASY_WIFI_AP_MAC)
#   define EX_EASY_WIFI_AP_MAC          "\x11\x22\x33\x44\x55\x66"
#endif
#if !defined(EX_EASY_WIFI_STA_MAC)
#   define EX_EASY_WIFI_STA_MAC         "\xaa\xbb\xcc\xdd\xee\xff"
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void _wifi_ap_event_handler(
    xf_wifi_event_id_t event_id, void *event_data, void *user_args);
static void _wifi_ip_event_handler(
    xf_ip_event_id_t event_id, void *event_data, void *user_args);

static void _wifi_sta_event_handler(
    xf_wifi_event_id_t event_id, void *event_data, void *user_args);
static void _ip_event_handler(
    xf_ip_event_id_t event_id, void *event_data, void *user_args);

static void _sta_task(void *argument);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "ex_easy_wifi";

/* AP */

static xf_wifi_ap_cfg_ext_t s_ap_cfg_ext = {
    .b_set_mac      = true,
    .mac            = EX_EASY_WIFI_AP_MAC,
};
static xf_wifi_ap_cfg_t s_ap_cfg = {
    .ssid           = CONFIG_EX_EASY_WIFI_SSID,
    .password       = CONFIG_EX_EASY_WIFI_PASSWORD,
    .authmode       = EXAMPLE_WIFI_AUTHMODE,
    .channel        = EXAMPLE_WIFI_CHANNEL,
    .ssid_hidden    = EXAMPLE_WIFI_SSID_HIDDEN_FLAG,
    .p_cfg_ext      = &s_ap_cfg_ext,    /*!< 配置 mac */
    .p_static_ip    = NULL,             /*!< 不使用静态 IP */
};
static uint8_t s_ap_ip_assigned = false;

/* STA */

static uint8_t s_sta_connected = false;     /*!< 是否已连接 */
static uint8_t s_sta_got_ip_flag = false;   /*!< 是否获取到 IP */

static xf_wifi_sta_cfg_ext_t s_sta_cfg_ext = {
    .b_set_mac      = true,
    .mac            = EX_EASY_WIFI_STA_MAC,
};
static xf_wifi_sta_cfg_t s_sta_cfg = {
    .ssid           = CONFIG_EX_EASY_WIFI_SSID,
    .password       = CONFIG_EX_EASY_WIFI_PASSWORD,
    .bssid_set      = false,                /*!< 不配置目标 AP 的 MAC 地址 */
    .bssid          = {0},
    .authmode       = EXAMPLE_WIFI_AUTHMODE,
    .channel        = EXAMPLE_WIFI_CHANNEL,
    .p_cfg_ext      = &s_sta_cfg_ext,       /*!< 配置 mac */
    .p_static_ip    = NULL,                 /*!< 不使用静态 IP */
};

static xf_osal_thread_t s_sta_thread_hdl = NULL;
#define EX_THREAD_NAME          "ex_sta"
#define EX_THREAD_PRIORITY      XF_OSAL_PRIORITY_NORMOL
#define EX_THREAD_STACK_SIZE    (1024 * 4)
static const xf_osal_thread_attr_t s_sta_attr = {
    .name       = EX_THREAD_NAME,
    .priority   = EX_THREAD_PRIORITY,
    .stack_size = EX_THREAD_STACK_SIZE,
};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_err_t ex_easy_wifi_ap(void)
{
    XF_ERROR_CHECK(xf_wifi_enable());
    XF_ERROR_CHECK(xf_wifi_ap_set_cb(_wifi_ap_event_handler, NULL));
    XF_ERROR_CHECK(xf_wifi_ap_set_ip_cb(_wifi_ip_event_handler, NULL));

    s_ap_cfg.authmode = EXAMPLE_WIFI_AUTHMODE;
    if (strnlen((const char *)s_ap_cfg.password, XF_WIFI_PASSWORD_LEN_MAX - 1) == 0) {
        s_ap_cfg.authmode = XF_WIFI_AUTH_OPEN;
    }

    XF_ERROR_CHECK(xf_wifi_ap_init(&s_ap_cfg));

    while (false == s_ap_ip_assigned) {
        xf_osal_delay_ms(100);
    }

    return XF_OK;
}

xf_ip4_addr_t ex_easy_wifi_ap_get_last_sta_ip(void)
{
    xf_ip4_addr_t ip4_addr = {0};
    xf_wifi_sta_info_t sta_array[8] = {0};
    uint32_t sta_num = 0;
    xf_err_t xf_ret = XF_OK;
    xf_ret = xf_wifi_ap_get_sta_list(sta_array, ARRAY_SIZE(sta_array), &sta_num);
    if (XF_OK != xf_ret) {
        XF_LOGE(TAG, "ap_get_sta_list-%s", xf_err_to_name(xf_ret));
        goto l_err;
    }
    if (sta_num > ARRAY_SIZE(sta_array) || (0 == sta_num)) {
        XF_LOGE(TAG, "sta_num:%d-%s", sta_num, xf_err_to_name(XF_ERR_INVALID_ARG));
        goto l_err;
    }
    xf_netif_t netif_hdl = NULL;
    xf_ret = xf_wifi_ap_get_netif(&netif_hdl);
    if (XF_OK != xf_ret) {
        XF_LOGE(TAG, "ap_get_netif-%s", xf_err_to_name(xf_ret));
        goto l_err;
    }
    xf_netif_pair_mac_ip_t pair_mac_ip[1] = {0};
    xf_memcpy(pair_mac_ip[0].mac, sta_array[sta_num - 1].mac, ARRAY_SIZE(sta_array[0].mac));
    xf_ret = xf_netif_dhcps_get_clients_by_mac(
                 netif_hdl, pair_mac_ip, ARRAY_SIZE(pair_mac_ip));
    if (XF_OK != xf_ret) {
        XF_LOGE(TAG, "xf_netif_dhcps_get_clients_by_mac-%s", xf_err_to_name(xf_ret));
        goto l_err;
    }
    ip4_addr.addr = pair_mac_ip[0].ip.addr;
    return ip4_addr;

l_err:;
    return (xf_ip4_addr_t) {
        0
    };
}

xf_ip4_addr_t ex_easy_wifi_ap_get_onw_ip(void)
{
    xf_ip4_addr_t ip4_addr = {0};
    xf_err_t xf_ret = XF_OK;
    xf_netif_t netif_hdl = NULL;
    xf_ret = xf_wifi_ap_get_netif(&netif_hdl);
    if (XF_OK != xf_ret) {
        XF_LOGE(TAG, "ap_get_netif-%s", xf_err_to_name(xf_ret));
        goto l_err;
    }
    xf_netif_ip_info_t ip_info = {0};
    xf_ret = xf_netif_get_ip_info(netif_hdl, &ip_info);
    if (XF_OK != xf_ret) {
        XF_LOGE(TAG, "get_ip_info-%s", xf_err_to_name(xf_ret));
        goto l_err;
    }

    ip4_addr.addr = ip_info.ip.addr;
    return ip4_addr;

l_err:;
    return (xf_ip4_addr_t) {
        0
    };
}

xf_err_t ex_easy_wifi_sta(void)
{
    XF_ERROR_CHECK(xf_wifi_enable());
    XF_ERROR_CHECK(xf_wifi_sta_set_cb(_wifi_sta_event_handler, NULL));
    XF_ERROR_CHECK(xf_wifi_sta_set_ip_cb(_ip_event_handler, NULL));

    s_sta_cfg.authmode = EXAMPLE_WIFI_AUTHMODE;
    if (strnlen((const char *)s_sta_cfg.password, XF_WIFI_PASSWORD_LEN_MAX) == 0) {
        s_sta_cfg.authmode = XF_WIFI_AUTH_OPEN;
    }

    XF_ERROR_CHECK(xf_wifi_sta_init(&s_sta_cfg));

    s_sta_thread_hdl = xf_osal_thread_create(_sta_task, NULL, &s_sta_attr);
    if (s_sta_thread_hdl == NULL) {
        XF_LOGE(TAG, "xf_osal_thread_create error");
        return XF_FAIL;
    }

    for (;;) {
        if (true == s_sta_got_ip_flag) {
            break;
        }
        xf_osal_delay_ms(100);
    }

    return XF_OK;
}

bool ex_easy_wifi_sta_is_connected(void)
{
    return (bool)s_sta_connected;
}

bool ex_easy_wifi_sta_got_ip(void)
{
    return (bool)s_sta_got_ip_flag;
}

xf_ip4_addr_t ex_easy_wifi_sta_get_gw_ip(void)
{
    xf_ip4_addr_t ip4_addr = {0};
    xf_err_t xf_ret = XF_OK;
    xf_netif_t netif_hdl = NULL;
    xf_ret = xf_wifi_sta_get_netif(&netif_hdl);
    if (XF_OK != xf_ret) {
        XF_LOGE(TAG, "ap_get_netif-%s", xf_err_to_name(xf_ret));
        goto l_err;
    }
    xf_netif_ip_info_t ip_info = {0};
    xf_ret = xf_netif_get_ip_info(netif_hdl, &ip_info);
    if (XF_OK != xf_ret) {
        XF_LOGE(TAG, "get_ip_info-%s", xf_err_to_name(xf_ret));
        goto l_err;
    }

    ip4_addr.addr = ip_info.gw.addr;
    return ip4_addr;

l_err:;
    return (xf_ip4_addr_t) {
        0
    };
}

xf_ip4_addr_t ex_easy_wifi_sta_get_onw_ip(void)
{
    xf_ip4_addr_t ip4_addr = {0};
    xf_err_t xf_ret = XF_OK;
    xf_netif_t netif_hdl = NULL;
    xf_ret = xf_wifi_sta_get_netif(&netif_hdl);
    if (XF_OK != xf_ret) {
        XF_LOGE(TAG, "ap_get_netif-%s", xf_err_to_name(xf_ret));
        goto l_err;
    }
    xf_netif_ip_info_t ip_info = {0};
    xf_ret = xf_netif_get_ip_info(netif_hdl, &ip_info);
    if (XF_OK != xf_ret) {
        XF_LOGE(TAG, "get_ip_info-%s", xf_err_to_name(xf_ret));
        goto l_err;
    }

    ip4_addr.addr = ip_info.ip.addr;
    return ip4_addr;
l_err:;
    return (xf_ip4_addr_t) {
        0
    };
}

/* ==================== [Static Functions] ================================== */

static void _sta_task(void *argument)
{
    for (;;) {
        if (false == xf_wifi_sta_is_connected()) {
            /* 如果填 NULL，则使用 xf_wifi_sta_init() 设置的配置连接 */
            xf_wifi_sta_connect(NULL);
            xf_osal_delay_ms(3000);
        }
        xf_osal_delay_ms(100);
    }
    xf_osal_thread_delete(NULL);
}

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
        s_ap_ip_assigned = true;
    } break;
    default:
        break;
    }
}

static void _wifi_sta_event_handler(
    xf_wifi_event_id_t event_id, void *event_data, void *user_args)
{
    UNUSED(user_args);

    switch (event_id) {
    case XF_WIFI_EVENT_STA_START: {
        XF_LOGI(TAG, "STA has started.");
    } break;
    case XF_WIFI_EVENT_STA_STOP: {
        s_sta_got_ip_flag = false;
        XF_LOGI(TAG, "STA has stopped.");
    } break;
    case XF_WIFI_EVENT_STA_CONNECTED: {
        s_sta_connected = true;
        XF_LOGI(TAG, "The STA is connected to the AP.");
        xf_wifi_event_sta_conn_t *e = (xf_wifi_event_sta_conn_t *)event_data;
        XF_LOGI(TAG,
                "\nssid:      %s\n"
                "mac:       "XF_MACSTR"\n"
                "channel:   %d\n"
                "authmode:  %d",
                e->ssid, XF_MAC2STR(e->bssid), (int)e->channel, (int)e->authmode);
    } break;
    case XF_WIFI_EVENT_STA_DISCONNECTED: {
        /* 如果之前的状态是已连接 */
        if (true == s_sta_connected) {
            s_sta_got_ip_flag = false;
            s_sta_connected = false;
            XF_LOGI(TAG, "The STA has been disconnected from the AP.");
            xf_wifi_event_sta_disconn_t *e = (xf_wifi_event_sta_disconn_t *)event_data;
            XF_LOGI(TAG,
                    "\nssid:      %s\n"
                    "mac:       "XF_MACSTR"\n"
                    "rssi:      %d",
                    e->ssid, XF_MAC2STR(e->bssid), (int)e->rssi);
        }
    } break;
    default:
        break;
    }
}

static void _ip_event_handler(
    xf_ip_event_id_t event_id, void *event_data, void *user_args)
{
    switch (event_id) {
    case XF_IP_EVENT_GOT_IP: {
        xf_ip_event_got_ip_t *e = (xf_ip_event_got_ip_t *)event_data;
        XF_LOGI(TAG, "\n"
                "got ip:       " XF_IPSTR "\n"
                "got gw:       " XF_IPSTR "\n"
                "got netmask:  " XF_IPSTR,
                XF_IP2STR(&e->ip_info.ip),
                XF_IP2STR(&e->ip_info.gw),
                XF_IP2STR(&e->ip_info.netmask));
        s_sta_got_ip_flag = true;
        if (0 == e->ip_info.gw.addr) {
            /* TODO 对于 ws63 的默认 at ap(1.10.101)，存在不会分配 gw 的现象 */
            xf_netif_ip_info_t ip_info = {0};
            ip_info.ip.addr         = e->ip_info.ip.addr;
            ip_info.netmask.addr    = e->ip_info.netmask.addr;
            ip_info.gw.addr = XF_IP4TOADDR(xf_ip4_addr1_16(&e->ip_info.ip),
                                           xf_ip4_addr2_16(&e->ip_info.ip),
                                           xf_ip4_addr3_16(&e->ip_info.ip),
                                           1);
            xf_netif_set_ip_info(e->netif_hdl, &ip_info);
        }
    } break;
    default:
        break;
    }
}
