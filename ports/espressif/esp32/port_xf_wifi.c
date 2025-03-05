/**
 * @file port_xf_wifi.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "nvs_flash.h"

#include "port_xf_wifi.h"
#include "port_xf_netif.h"

#include "xf_utils.h"

#include "xf_wifi.h"

/* ==================== [Defines] =========================================== */

#define EXAMPLE_MAX_STA_CONN        8       /*!< ap 默认最大站点连接数 */
#define EXAMPLE_BEACON_INTERVAL     100     /*!< 信标间隔默认 100, @ref wifi_ap_config_t.beacon_interval */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief 平台认证模式与 xf 认证模式对应关系。
 */
typedef struct port_convert_wifi_auth_mode_s {
    xf_wifi_auth_mode_t xf;
    wifi_auth_mode_t pf;
} port_convert_wifi_auth_mode_t;

/* ==================== [Static Prototypes] ================================= */

static void port_wifi_ap_event_handler(
    void *arg, esp_event_base_t event_base,
    int32_t event_id, void *event_data);

static void port_wifi_sta_event_handler(
    void *arg, esp_event_base_t event_base,
    int32_t event_id, void *event_data);

static void port_convert_xf2pf_ap_cfg(
    const xf_wifi_ap_cfg_t *p_xf_ap_cfg, xf_wifi_ap_cfg_ext_t *p_xf_ap_cfg_ext,
    wifi_config_t *p_esp_cfg);
static void port_convert_xf2pf_sta_cfg(
    const xf_wifi_sta_cfg_t *p_xf_sta_cfg, xf_wifi_sta_cfg_ext_t *p_xf_sta_cfg_ext,
    wifi_config_t *p_esp_cfg);

static void port_esp_netif_init(void);
static void port_esp_netif_deinit(void);

static void port_ap_netif_init(void);
static void port_ap_netif_deinit(void);

static void port_sta_netif_init(void);
static void port_sta_netif_deinit(void);

static xf_err_t port_convert_wifi_auth_mode(
    xf_wifi_auth_mode_t *xf, wifi_auth_mode_t *pf, bool is_xf2pf);
static wifi_auth_mode_t     port_convert_xf2pf_wifi_auth_mode(xf_wifi_auth_mode_t authmode);
static xf_wifi_auth_mode_t port_convert_pf2xf_wifi_auth_mode(wifi_auth_mode_t authmode);
static xf_wifi_event_code_t port_convert_pf2xf_wifi_event(wifi_event_t event);
static xf_ip_event_id_t     port_convert_pf2xf_ip_event(ip_event_t event);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "port_xf_wifi";

static port_xf_wifi_context_t s_port_xf_wifi_context = {0};
static port_xf_wifi_context_t *sp_port_xf_wifi_context = &s_port_xf_wifi_context;
#define ctx_w() sp_port_xf_wifi_context

static port_xf_netif_context_t *sp_port_xf_netif_context = NULL;
#define ctx_n() sp_port_xf_netif_context

/* ==================== [Macros] ============================================ */

#ifndef min
#define min(x, y)       (((x) < (y)) ? (x) : (y))
#endif

/* ==================== [Global Functions] ================================== */

port_xf_wifi_context_t *port_xf_wifi_get_context(void)
{
    return sp_port_xf_wifi_context;
}

xf_err_t xf_wifi_enable(void)
{
    ctx_n() = port_xf_netif_get_context();
    if (ctx_w()->b_inited) {
        return XF_ERR_INITED;
    }

    pf_err_t ret = nvs_flash_init();
    if ((ret == ESP_ERR_NVS_NO_FREE_PAGES) || (ret == ESP_ERR_NVS_NEW_VERSION_FOUND)) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ctx_w()->b_inited = true;

    return XF_OK;
}

xf_err_t xf_wifi_disable(void)
{
    if (!ctx_w()->b_inited) {
        return XF_ERR_UNINIT;
    }
    pf_err_t esp_err = esp_wifi_deinit();
    if (ESP_OK == esp_err) {
        ctx_w()->b_inited = false;
    }
    return XF_OK;
}

xf_err_t xf_wifi_set_mac(
    xf_wifi_interface_t ifx, const uint8_t mac[XF_MAC_LEN_MAX])
{
    XF_CHECK(ifx >= XF_WIFI_IF_MAX, XF_ERR_INVALID_ARG,
             TAG, "ifx invalid");
    XF_CHECK(mac == NULL, XF_ERR_INVALID_ARG,
             TAG, "mac invalid");
    esp_wifi_set_mac((ifx == XF_WIFI_IF_STA) ? (WIFI_IF_STA) : (WIFI_IF_AP), mac);
    return XF_OK;
}

xf_err_t xf_wifi_get_mac(
    xf_wifi_interface_t ifx, uint8_t mac[XF_MAC_LEN_MAX])
{
    XF_CHECK(ifx >= XF_WIFI_IF_MAX, XF_ERR_INVALID_ARG,
             TAG, "ifx invalid");
    XF_CHECK(mac == NULL, XF_ERR_INVALID_ARG,
             TAG, "mac invalid");
    esp_wifi_get_mac((ifx == XF_WIFI_IF_STA) ? (WIFI_IF_STA) : (WIFI_IF_AP), mac);
    return XF_OK;
}

/* ap */

xf_err_t xf_wifi_ap_init(const xf_wifi_ap_cfg_t *p_cfg)
{
    if (ctx_w()->b_ap_inited) {
        return XF_ERR_INITED;
    }

    XF_CHECK(NULL == p_cfg, XF_ERR_INVALID_ARG,
             TAG, "NULL==p_cfg");
    XF_CHECK(((false == p_cfg->ssid_hidden) && ('\0' == p_cfg->ssid[0])), XF_ERR_INVALID_ARG,
             TAG, "ssid length is 0.");

    /* 初始化内部 netif 组件 */
    port_esp_netif_init();
    /* ap 申请内部 netif 句柄并存储 */
    port_ap_netif_init();

    /* 转换 xf 配置，并启动 wifi */
    wifi_config_t wifi_config = {0};
    port_convert_xf2pf_ap_cfg(p_cfg, p_cfg->p_cfg_ext, &wifi_config);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    if (p_cfg->p_cfg_ext) {
        /* beacon_interval 的配置在 wifi_config 中 */
        if (p_cfg->p_cfg_ext->b_set_country) {
            /* TODO 国家码如果只改变字符其余不改变是否可行？ */
            wifi_country_t wifi_country = {0};
            esp_wifi_get_country(&wifi_country);
            strncpy(wifi_country.cc, p_cfg->p_cfg_ext->country.cc,
                    ARRAY_SIZE(p_cfg->p_cfg_ext->country.cc));
            esp_wifi_set_country(&wifi_country);
        }
        if (p_cfg->p_cfg_ext->b_set_mac) {
            esp_wifi_set_mac(WIFI_IF_AP, p_cfg->p_cfg_ext->mac);
        }
    }
    ESP_ERROR_CHECK(esp_wifi_start());

    /* 如果配置静态 ip 则停止 dhcps */
    if (p_cfg->p_static_ip) {
        if (XF_IPADDR_TYPE_V4 != p_cfg->p_static_ip->type) {
            XF_LOGW(TAG, "IPv6 is not currently supported.");
        } else {
            esp_netif_t *esp_netif = ctx_w()->netif_ap.esp_netif;
            esp_netif_dhcps_stop(esp_netif);

            esp_netif_ip_info_t esp_ip_info = {0};
            esp_ip_info.ip.addr        = p_cfg->p_static_ip->ip4.ip.addr;
            esp_ip_info.netmask.addr   = p_cfg->p_static_ip->ip4.netmask.addr;
            esp_ip_info.gw.addr        = p_cfg->p_static_ip->ip4.gw.addr;
            esp_netif_set_ip_info(esp_netif, &esp_ip_info);
        }
    }

    XF_LOGD(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
            wifi_config.ap.ssid, wifi_config.ap.password, wifi_config.ap.channel);

    ctx_w()->b_ap_inited = true;

    // /* TODO 是否需要 ap 成功启动才退出函数？ */
    // while (false == ctx_w()->b_ap_started) {
    //     vTaskDelay(20 / portTICK_PERIOD_MS);
    // }

    return XF_OK;
}

xf_err_t xf_wifi_ap_deinit(void)
{
    if (!ctx_w()->b_ap_inited) {
        return XF_ERR_UNINIT;
    }

    esp_wifi_stop();
    port_ap_netif_deinit();

    /* TODO netif 判断其他网络设备是否还需使用 */
    UNUSED(port_esp_netif_deinit);

    ctx_w()->b_ap_inited = false;
    return XF_OK;
}

xf_err_t xf_wifi_ap_get_netif(xf_netif_t *p_netif_hdl)
{
    XF_CHECK(false == ctx_w()->b_ap_inited, XF_ERR_UNINIT,
             TAG, "AP is not initialized");
    XF_CHECK(NULL == p_netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==p_netif_hdl");
    *p_netif_hdl = &ctx_w()->netif_ap;
    return XF_OK;
}

xf_err_t xf_wifi_ap_set_cb(xf_wifi_cb_t cb_func, void *user_args)
{
    ctx_w()->ap_cb         = cb_func;
    ctx_w()->ap_user_args  = user_args;
    return XF_OK;
}

xf_err_t xf_wifi_ap_set_ip_cb(xf_ip_cb_t cb_func, void *user_args)
{
    ctx_w()->ap_ip_cb           = cb_func;
    ctx_w()->ap_ip_user_args    = user_args;
    return XF_OK;
}

xf_err_t xf_wifi_ap_get_sta_list(
    xf_wifi_sta_info_t sta_array[], uint32_t sta_array_size,
    uint32_t *p_sta_num)
{
    XF_CHECK((NULL == p_sta_num), XF_ERR_INVALID_ARG,
             TAG, "NULL==p_sta_num");

    pf_err_t pf_err;
    wifi_sta_list_t sta_list = {0};

    pf_err = esp_wifi_ap_get_sta_list(&sta_list);
    if (ESP_OK != pf_err) {
        return XF_FAIL;
    }

    *p_sta_num = (uint32_t)sta_list.num;

    /* 不读取则返回 */
    if ((NULL == sta_array) || (0 == sta_array_size)) {
        return XF_OK;
    }

    uint32_t sta_size_min = min(sta_array_size, sta_list.num);
    for (uint32_t i = 0; i < sta_size_min; ++i) {
        memcpy(sta_array[i].mac, sta_list.sta->mac, XF_MAC_LEN_MAX);
        sta_array[i].rssi = sta_list.sta->rssi;
    }

    return XF_OK;
}

xf_err_t xf_wifi_ap_deauth_sta(const uint8_t mac[])
{
    XF_CHECK((NULL == mac), XF_ERR_INVALID_ARG,
             TAG, "NULL==mac");

    uint16_t aid;
    xf_err_t xf_ret;
    pf_err_t pf_err;

    pf_err = esp_wifi_ap_get_sta_aid(mac, &aid);
    xf_ret = port_convert_pf2xf_err(pf_err);
    if (XF_OK != xf_ret) {
        return xf_ret;
    }

    pf_err = esp_wifi_deauth_sta(aid);
    xf_ret = port_convert_pf2xf_err(pf_err);
    return xf_ret;
}

/* ap */

/* sta */

xf_err_t xf_wifi_sta_init(const xf_wifi_sta_cfg_t *p_cfg)
{
    if (ctx_w()->b_sta_inited) {
        return XF_ERR_INITED;
    }

    /* 初始化内部 netif 组件 */
    port_esp_netif_init();
    /* sta 申请内部 netif 句柄并存储 */
    port_sta_netif_init();

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    /* p_cfg 不为空时使用当前配置连接，为空时通常是扫描后连接 */
    ctx_w()->sta_cfg.sta.scan_method    = WIFI_ALL_CHANNEL_SCAN;    /*!< 默认全通道扫描 */
    if (p_cfg) {
        port_convert_xf2pf_sta_cfg(p_cfg, p_cfg->p_cfg_ext, &ctx_w()->sta_cfg);
        if (p_cfg->p_static_ip) {
            if (XF_IPADDR_TYPE_V4 != p_cfg->p_static_ip->type) {
                XF_LOGW(TAG, "IPv6 is not currently supported.");
            } else {
                /* ipv4 停止 dhcp 客户端并设置 ip */
                esp_netif_t *esp_netif = ctx_w()->netif_sta.esp_netif;
                esp_netif_dhcpc_stop(esp_netif);
                esp_netif_ip_info_t esp_ip_info = {0};
                esp_ip_info.ip.addr        = p_cfg->p_static_ip->ip4.ip.addr;
                esp_ip_info.netmask.addr   = p_cfg->p_static_ip->ip4.netmask.addr;
                esp_ip_info.gw.addr        = p_cfg->p_static_ip->ip4.gw.addr;
                esp_netif_set_ip_info(esp_netif, &esp_ip_info);
            }
        }
        if (p_cfg->p_cfg_ext) {
            if (p_cfg->p_cfg_ext->b_set_mac) {
                esp_wifi_set_mac(WIFI_IF_STA, p_cfg->p_cfg_ext->mac);
            }
        }
    }
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &ctx_w()->sta_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());

    ctx_w()->b_sta_inited = true;

    /* sta 成功启动才退出函数 */
    while (false == ctx_w()->b_sta_started) {
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }

    return XF_OK;
}

xf_err_t xf_wifi_sta_deinit(void)
{
    if (!ctx_w()->b_sta_inited) {
        return XF_ERR_UNINIT;
    }

    esp_wifi_stop();

    port_sta_netif_deinit();

    /* TODO netif 判断其他网络设备是否还需使用 */
    UNUSED(port_esp_netif_deinit);

    ctx_w()->sta_cfg = (wifi_config_t) {
        0
    };
    ctx_w()->b_sta_inited = false;

    return XF_OK;
}

xf_err_t xf_wifi_sta_get_netif(xf_netif_t *p_netif_hdl)
{
    XF_CHECK(false == ctx_w()->b_sta_inited, XF_ERR_UNINIT,
             TAG, "STA is not initialized");
    XF_CHECK(NULL == p_netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==p_netif_hdl");
    *p_netif_hdl = &ctx_w()->netif_sta;
    return XF_OK;
}

xf_err_t xf_wifi_sta_set_cb(xf_wifi_cb_t cb_func, void *user_args)
{
    ctx_w()->sta_cb             = cb_func;
    ctx_w()->sta_user_args      = user_args;
    return XF_OK;
}

xf_err_t xf_wifi_sta_set_ip_cb(xf_ip_cb_t cb_func, void *user_args)
{
    ctx_w()->sta_ip_cb          = cb_func;
    ctx_w()->sta_ip_user_args   = user_args;
    return XF_OK;
}

xf_err_t xf_wifi_sta_connect(xf_wifi_sta_cfg_t *p_cfg)
{
    xf_err_t xf_ret;
    pf_err_t pf_err;
    if (p_cfg) {
        port_convert_xf2pf_sta_cfg(p_cfg, NULL, &ctx_w()->sta_cfg);
        /* 此处不用 p_cfg->p_cfg_ext 和 p_cfg->p_static_ip 配置 */
        pf_err = esp_wifi_set_config(WIFI_IF_STA, &ctx_w()->sta_cfg);
        xf_ret = port_convert_pf2xf_err(pf_err);
        if (XF_OK != xf_ret) {
            return xf_ret;
        }
    }
    pf_err = esp_wifi_connect();
    xf_ret = port_convert_pf2xf_err(pf_err);
    return xf_ret;
}

bool xf_wifi_sta_is_connected(void)
{
    /*
        TODO sta_connected 判断方式似乎不是特别可靠？
        或改成
        pf_err_t pf_err;
        wifi_ap_record_t ap_record = {0};
        pf_err = esp_wifi_sta_get_ap_info(&ap_record);
        return (ESP_OK == pf_err) ? (true) : (false);
     */
    return ctx_w()->b_sta_connected;
}

xf_err_t xf_wifi_sta_disconnect(void)
{
    xf_err_t xf_ret;
    pf_err_t pf_err;
    pf_err = esp_wifi_disconnect();
    xf_ret = port_convert_pf2xf_err(pf_err);
    /* 不需要手动清除 ctx_w()->b_sta_connected */
    return xf_ret;
}

xf_err_t xf_wifi_sta_get_ap_info(xf_wifi_ap_info_t *p_info)
{
    XF_CHECK(NULL == p_info, XF_ERR_INVALID_ARG,
             TAG, "NULL==p_info");
    xf_err_t xf_ret;
    pf_err_t pf_err;
    wifi_ap_record_t ap_record = {0};
    wifi_config_t wifi_cfg = {0};

    pf_err = esp_wifi_sta_get_ap_info(&ap_record);
    xf_ret = port_convert_pf2xf_err(pf_err);
    if (XF_OK == xf_ret) {
        memcpy(p_info->bssid,   ap_record.bssid,    XF_MAC_LEN_MAX);
        memcpy(p_info->ssid,    ap_record.ssid,     ARRAY_SIZE(ap_record.ssid));
        p_info->rssi        = ap_record.rssi;
        esp_wifi_get_config(WIFI_IF_STA, &wifi_cfg);
        p_info->channel     = wifi_cfg.sta.channel;
    }
    return xf_ret;
}

xf_err_t xf_wifi_scan_start(const xf_wifi_scan_cfg_t *p_cfg, bool block)
{
    XF_CHECK(NULL == p_cfg, XF_ERR_INVALID_ARG,
             TAG, "NULL==p_cfg");
    /* TODO 增加软件层面上，对全通道扫描的支持 */
    XF_CHECK(0 == p_cfg->channel, XF_ERR_INVALID_ARG,
             TAG, "Full channel scanning is not supported.");

    if (ctx_w()->b_scanning) {
        return XF_ERR_BUSY;
    }
    ctx_w()->b_scanning = true;

    xf_err_t xf_ret;
    pf_err_t pf_err;
    wifi_scan_config_t scan_config = {0};
    scan_config.ssid                    = p_cfg->ssid;
    scan_config.bssid                   = p_cfg->bssid;
    scan_config.channel                 = p_cfg->channel;
    scan_config.show_hidden             = p_cfg->show_hidden;
    ctx_w()->scanning_ch                = p_cfg->channel;   /*!< 用于在读取时输出 */
    scan_config.scan_type               = WIFI_SCAN_TYPE_ACTIVE;
    scan_config.scan_time.active.min    = 0;    /*!< 保持默认配置 */
    scan_config.scan_time.active.max    = 120;  /*!< 保持默认配置 */
    scan_config.scan_time.passive       = 360;  /*!< 保持默认配置 */
    scan_config.home_chan_dwell_time    = 30;   /*!< 保持默认配置 */
    pf_err = esp_wifi_scan_start(&scan_config, block);
    xf_ret = port_convert_pf2xf_err(pf_err);
    if (block) {
        ctx_w()->b_scanning = false;
    }
    return xf_ret;
}

xf_err_t xf_wifi_scan_stop(void)
{
    xf_err_t xf_ret;
    pf_err_t pf_err;
    pf_err = esp_wifi_scan_stop();
    xf_ret = port_convert_pf2xf_err(pf_err);
    return xf_ret;
}

xf_err_t xf_wifi_scan_get_result(
    xf_wifi_ap_info_t result[], uint32_t result_size,
    uint32_t *p_result_num)
{
    XF_CHECK((NULL == p_result_num), XF_ERR_INVALID_ARG,
             TAG, "NULL==p_result_num");

    xf_err_t xf_ret;
    pf_err_t pf_err;

    uint16_t result_num = 0;
    pf_err = esp_wifi_scan_get_ap_num(&result_num);
    xf_ret = port_convert_pf2xf_err(pf_err);
    if (XF_OK != xf_ret) {
        return xf_ret;
    }

    *p_result_num = (uint32_t)result_num;

    /* 不读取或没有结果则返回 */
    if ((NULL == result) || (0 == result_size)
            || (0 == result_num)) {
        return XF_OK;
    }

    wifi_ap_record_t ap_record = {0};
    pf_err = esp_wifi_scan_get_ap_record(&ap_record);
    uint32_t i = 0;
    for (; (i < result_size) && (ESP_OK == pf_err); ++i) {
        memcpy(result[i].bssid, ap_record.bssid,    XF_MAC_LEN_MAX);
        memcpy(result[i].ssid,  ap_record.ssid,     ARRAY_SIZE(ap_record.ssid));
        result[i].rssi      = ap_record.rssi;
        result[i].authmode  = port_convert_pf2xf_wifi_auth_mode(ap_record.authmode);
        result[i].channel   = ctx_w()->scanning_ch;
        pf_err = esp_wifi_scan_get_ap_record(&ap_record);
    }
    /* FIXME esp_wifi_scan_get_ap_num() 获得的数量会更多 */
    /**
     * @bug esp_wifi_scan_get_ap_num() 获得的数量与实际通过
     * esp_wifi_scan_get_ap_record() 获取的数量不一致。
     * 后者更少，且正确。
     */
    *p_result_num = i;

    /*
        TODO 待定清除 AP 列表行为
        如果清除了，则内存安全性更高；
        如果不清除，则用户可分多次读取。
     */
    // esp_wifi_clear_ap_list();

    return XF_OK;
}

xf_err_t xf_wifi_scan_clear_result(void)
{
    xf_err_t xf_ret;
    pf_err_t pf_err;
    pf_err = esp_wifi_clear_ap_list();
    xf_ret = port_convert_pf2xf_err(pf_err);
    return xf_ret;
}

/* sta */

/* ==================== [Static Functions] ================================== */

static void port_wifi_ap_event_handler(
    void *arg, esp_event_base_t event_base,
    int32_t event_id, void *event_data)
{
    UNUSED(arg);

    if (!ctx_w()->ap_cb) {
        return;
    }

    xf_wifi_event_id_t xf_eid =
        port_convert_pf2xf_wifi_event((wifi_event_t)event_id);

    switch (xf_eid) {
    case XF_WIFI_EVENT_AP_START: {
        ctx_w()->b_ap_started = true;
        ctx_w()->ap_cb(xf_eid, NULL, ctx_w()->ap_user_args);
    } break;
    case XF_WIFI_EVENT_AP_STOP: {
        ctx_w()->b_ap_started = false;
        ctx_w()->ap_cb(xf_eid, NULL, ctx_w()->ap_user_args);
    } break;
    case XF_WIFI_EVENT_AP_STA_CONNECTED: {
        wifi_event_ap_staconnected_t *e = (wifi_event_ap_staconnected_t *)event_data;
        xf_wifi_event_ap_sta_conn_t sta_conn = {0};
        memcpy(sta_conn.mac, e->mac, ARRAY_SIZE(e->mac));
        ctx_w()->ap_cb(xf_eid, &sta_conn, ctx_w()->ap_user_args);
    } break;
    case XF_WIFI_EVENT_AP_STA_DISCONNECTED: {
        wifi_event_ap_stadisconnected_t *e = (wifi_event_ap_stadisconnected_t *)event_data;
        xf_wifi_event_ap_sta_disconn_t sta_disconn = {0};
        memcpy(sta_disconn.mac, e->mac, ARRAY_SIZE(e->mac));
        ctx_w()->ap_cb(xf_eid, &sta_disconn, ctx_w()->ap_user_args);
    } break;
    case XF_WIFI_EVENT_SCAN_DONE: {
        /* TODO ap 模式下可以扫描吗？ */
        ctx_w()->b_scanning = false;
        if (!ctx_w()->sta_cb) {
            return;
        }
        wifi_event_sta_scan_done_t *e = (wifi_event_sta_scan_done_t *)event_data;
        /* 这两个参数暂未未使用 */
        UNUSED(e->number);
        UNUSED(e->scan_id);
        if ((!e->status)) {
            ctx_w()->sta_cb(xf_eid, NULL, ctx_w()->sta_user_args);
        }
    } break;
    default:
        break;
    }

    return;
}

static void port_wifi_sta_event_handler(
    void *arg, esp_event_base_t event_base,
    int32_t event_id, void *event_data)
{
    UNUSED(arg);
    UNUSED(event_base);

    xf_wifi_event_id_t xf_eid =
        port_convert_pf2xf_wifi_event((wifi_event_t)event_id);

    switch (xf_eid) {
    case XF_WIFI_EVENT_STA_START: {
        ctx_w()->b_sta_started = true;
        ctx_w()->sta_cb(xf_eid, NULL, ctx_w()->sta_user_args);
    } break;
    case XF_WIFI_EVENT_STA_STOP: {
        ctx_w()->b_sta_started = false;
        ctx_w()->sta_cb(xf_eid, NULL, ctx_w()->sta_user_args);
    } break;
    case XF_WIFI_EVENT_STA_CONNECTED: {
        ctx_w()->b_sta_connected = true;
        if (!ctx_w()->sta_cb) {
            return;
        }
        xf_wifi_event_sta_conn_t    sta_conn    = {0};
        wifi_event_sta_connected_t *e = (wifi_event_sta_connected_t *)event_data;
        memcpy(sta_conn.ssid,       e->ssid,    e->ssid_len);
        memcpy(sta_conn.bssid,      e->bssid,   ARRAY_SIZE(e->bssid));
        sta_conn.channel    = e->channel;
        sta_conn.authmode   = port_convert_pf2xf_wifi_auth_mode(e->authmode);
        ctx_w()->sta_cb(xf_eid, &sta_conn, ctx_w()->sta_user_args);
    } break;
    case XF_WIFI_EVENT_STA_DISCONNECTED: {
        ctx_w()->b_sta_connected = false;
        if (!ctx_w()->sta_cb) {
            return;
        }
        /*
            应用程序处理此事件最常用的方法为：
            调用函数 esp_wifi_connect() 重新连接 Wi-Fi。
            但是，如果此事件是由函数 esp_wifi_disconnect() 引发的，
            则应用程序不应调用 esp_wifi_connect() 来重新连接。
         */
        xf_wifi_event_sta_disconn_t sta_disconn = {0};
        wifi_event_sta_disconnected_t *e = (wifi_event_sta_disconnected_t *)event_data;
        memcpy(sta_disconn.ssid,    e->ssid,    e->ssid_len);
        memcpy(sta_disconn.bssid,   e->bssid,   ARRAY_SIZE(e->bssid));
        sta_disconn.rssi    = e->rssi;
        UNUSED(e->reason);  /* TODO 断连原因是否必要，且大部分平台支持？ */
        ctx_w()->sta_cb(xf_eid, &sta_disconn, ctx_w()->sta_user_args);
    } break;
    case XF_WIFI_EVENT_SCAN_DONE: {
        ctx_w()->b_scanning = false;
        if (!ctx_w()->sta_cb) {
            return;
        }
        wifi_event_sta_scan_done_t *e = (wifi_event_sta_scan_done_t *)event_data;
        /* 这两个参数暂未未使用 */
        UNUSED(e->number);
        UNUSED(e->scan_id);
        if ((!e->status)) {
            ctx_w()->sta_cb(xf_eid, NULL, ctx_w()->sta_user_args);
        }
    } break;
    default:
        break;
    }
    return;
}

static void port_wifi_ap_ip_event_handler(
    void *arg, esp_event_base_t event_base,
    int32_t event_id, void *event_data)
{
    UNUSED(arg);

    if (!ctx_w()->ap_ip_cb) {
        return;
    }

    xf_ip_event_id_t xf_eid =
        port_convert_pf2xf_ip_event((ip_event_t)event_id);

    switch (xf_eid) {
    case XF_IP_EVENT_IP_ASSIGNED: {
        /*
            该回调只对应 wifi ap ip 事件，不需要判断：
            if (e->esp_netif == ctx_w()->netif_ap.esp_netif) {
            }
         */
        ip_event_ap_staipassigned_t *e = (ip_event_ap_staipassigned_t *)event_data;
        xf_ip_event_ip_assigned_t ip_assigned = {0};
        ip_assigned.netif_hdl   = (xf_netif_t)&ctx_w()->netif_ap;
        ip_assigned.ip.addr     = e->ip.addr;
        memcpy(ip_assigned.mac, e->mac, ARRAY_SIZE(ip_assigned.mac));
        ctx_w()->ap_ip_cb(xf_eid, &ip_assigned, ctx_w()->ap_ip_user_args);
    } break;
    default:
        break;
    }

    return;
}

static void port_wifi_sta_ip_event_handler(
    void *arg, esp_event_base_t event_base,
    int32_t event_id, void *event_data)
{
    UNUSED(arg);

    if (!ctx_w()->sta_ip_cb) {
        return;
    }

    xf_ip_event_id_t xf_eid =
        port_convert_pf2xf_ip_event((ip_event_t)event_id);

    switch (xf_eid) {
    case IP_EVENT_STA_GOT_IP: {
        ip_event_got_ip_t *e = (ip_event_got_ip_t *)event_data;
        xf_ip_event_got_ip_t got_ip = {0};
        /*
            该回调只对应 wifi sta ip 事件，不需要判断：
            if (e->esp_netif == ctx_w()->netif_sta.esp_netif) {
            }
         */
        got_ip.netif_hdl                = (xf_netif_t)&ctx_w()->netif_sta;
        got_ip.ip_info.ip.addr          = e->ip_info.ip.addr;
        got_ip.ip_info.netmask.addr     = e->ip_info.netmask.addr;
        got_ip.ip_info.gw.addr          = e->ip_info.gw.addr;
        ctx_w()->sta_ip_cb(xf_eid, &got_ip, ctx_w()->sta_ip_user_args);
    } break;
    /*
        TODO 以下连个事件还未支持
        - XF_IP_EVENT_LOST_IP
        - XF_IP_EVENT_GOT_IP6
     */
    default:
        break;
    }

    return;
}

static void port_convert_xf2pf_ap_cfg(
    const xf_wifi_ap_cfg_t *p_xf_ap_cfg, xf_wifi_ap_cfg_ext_t *p_xf_ap_cfg_ext,
    wifi_config_t *p_esp_cfg)
{
    memcpy(p_esp_cfg->ap.ssid,      p_xf_ap_cfg->ssid,      ARRAY_SIZE(p_xf_ap_cfg->ssid) - 1);
    p_esp_cfg->ap.ssid_len              = strlen((const char *)p_xf_ap_cfg->ssid);
    p_esp_cfg->ap.channel               = p_xf_ap_cfg->channel;
    memcpy(p_esp_cfg->ap.password,  p_xf_ap_cfg->password,  ARRAY_SIZE(p_xf_ap_cfg->password) - 1);
    p_esp_cfg->ap.authmode              = port_convert_xf2pf_wifi_auth_mode(p_xf_ap_cfg->authmode);
    p_esp_cfg->ap.ssid_hidden           = p_xf_ap_cfg->ssid_hidden;
    p_esp_cfg->ap.max_connection    = EXAMPLE_MAX_STA_CONN;
    if ((p_xf_ap_cfg_ext) && (p_xf_ap_cfg_ext->b_set_beacon_interval)) {
        p_esp_cfg->ap.beacon_interval   = p_xf_ap_cfg_ext->beacon_interval;
    } else {
        p_esp_cfg->ap.beacon_interval   = EXAMPLE_BEACON_INTERVAL;
    }
    p_esp_cfg->ap.pmf_cfg.required      = false;
}

static void port_convert_xf2pf_sta_cfg(
    const xf_wifi_sta_cfg_t *p_xf_sta_cfg, xf_wifi_sta_cfg_ext_t *p_xf_sta_cfg_ext,
    wifi_config_t *p_esp_cfg)
{
    UNUSED(p_xf_sta_cfg_ext);

    memcpy(p_esp_cfg->sta.ssid,     p_xf_sta_cfg->ssid,     ARRAY_SIZE(p_xf_sta_cfg->ssid) - 1);
    memcpy(p_esp_cfg->sta.password, p_xf_sta_cfg->password, ARRAY_SIZE(p_xf_sta_cfg->password) - 1);

    p_esp_cfg->sta.channel              = p_xf_sta_cfg->channel;
    p_esp_cfg->sta.listen_interval      = 0;
    /* 默认全通道扫描 */
    p_esp_cfg->sta.scan_method          = WIFI_ALL_CHANNEL_SCAN;
    /* 默认不指定 AP mac */
    p_esp_cfg->sta.bssid_set            = p_xf_sta_cfg->bssid_set;
    if (p_xf_sta_cfg->bssid_set) {
        memcpy(p_esp_cfg->sta.bssid, p_xf_sta_cfg->bssid, XF_MAC_LEN_MAX);
    }
    /* 默认按照 rssi 排序 */
    p_esp_cfg->sta.sort_method          = WIFI_CONNECT_AP_BY_SIGNAL;
    /* 默认不设最低信号阈值 */
    UNUSED(p_esp_cfg->sta.threshold.rssi);
    p_esp_cfg->sta.threshold.authmode   = port_convert_xf2pf_wifi_auth_mode(p_xf_sta_cfg->authmode);
    UNUSED(p_esp_cfg->sta.pmf_cfg.capable);
    p_esp_cfg->sta.pmf_cfg.required     = false;
    UNUSED(p_esp_cfg->sta.rm_enabled);
    UNUSED(p_esp_cfg->sta.btm_enabled);
    UNUSED(p_esp_cfg->sta.mbo_enabled);
    UNUSED(p_esp_cfg->sta.ft_enabled);
    UNUSED(p_esp_cfg->sta.owe_enabled);
    UNUSED(p_esp_cfg->sta.transition_disable);
    UNUSED(p_esp_cfg->sta.reserved);
    /* 默认 WPA3_SAE_PWE_BOTH */
    p_esp_cfg->sta.sae_pwe_h2e          = WPA3_SAE_PWE_BOTH;
    UNUSED(p_esp_cfg->sta.failure_retry_cnt);
}

static void port_esp_netif_init(void)
{
    if (ctx_n()->b_netif_inited) {
        return;
    }
    UNUSED(esp_netif_init());
    UNUSED(esp_event_loop_create_default());
}

static void port_esp_netif_deinit(void)
{
    if (!ctx_n()->b_netif_inited) {
        return;
    }
    ctx_n()->b_netif_inited = false;
    UNUSED(esp_event_loop_delete_default());
    UNUSED(esp_netif_deinit());
}

static void port_ap_netif_init(void)
{
    if (ctx_w()->b_ap_netif_is_inited) {
        return;
    }
    ctx_w()->b_ap_netif_is_inited = true;
    ctx_w()->netif_ap.esp_netif = esp_netif_create_default_wifi_ap();

#define __AP_EVENT_REGISTER(id, instance) \
    esp_event_handler_instance_register( \
        WIFI_EVENT, (id), &port_wifi_ap_event_handler, NULL, (instance))
    /* AP 事件注册 */
    __AP_EVENT_REGISTER(ESP_EVENT_ANY_ID,               &ctx_w()->instance_ap);
#undef __AP_EVENT_REGISTER

#define __AP_IP_EVENT_REGISTER(id, instance) \
    esp_event_handler_instance_register( \
        IP_EVENT, (id), &port_wifi_ap_ip_event_handler, NULL, (instance))
    /* AP IP 事件注册 */
    __AP_IP_EVENT_REGISTER(IP_EVENT_AP_STAIPASSIGNED,   &ctx_w()->instance_ap_ip_staipassigned);
#undef __AP_IP_EVENT_REGISTER
}

static void port_ap_netif_deinit(void)
{
    if (!ctx_w()->b_ap_netif_is_inited) {
        return;
    }
    ctx_w()->b_ap_netif_is_inited = false;

#define __AP_EVENT_UNREGISTER(id, instance) \
    esp_event_handler_instance_unregister(WIFI_EVENT, (id), (instance))
    /* AP 事件反注册 */
    __AP_EVENT_UNREGISTER(ESP_EVENT_ANY_ID,                 ctx_w()->instance_ap);
#undef __AP_EVENT_REGISTER

#define __AP_IP_EVENT_UNREGISTER(id, instance) \
    esp_event_handler_instance_unregister(IP_EVENT, (id), (instance))
    /* AP IP 事件反注册 */
    __AP_IP_EVENT_UNREGISTER(IP_EVENT_AP_STAIPASSIGNED,     ctx_w()->instance_ap_ip_staipassigned);
#undef __AP_IP_EVENT_REGISTER

    esp_netif_destroy_default_wifi(ctx_w()->netif_ap.esp_netif);
    ctx_w()->netif_ap.esp_netif = NULL;
}

static void port_sta_netif_init(void)
{
    if (ctx_w()->b_sta_netif_is_inited) {
        return;
    }
    ctx_w()->b_sta_netif_is_inited = true;
    ctx_w()->netif_sta.esp_netif = esp_netif_create_default_wifi_sta();

#define __STA_EVENT_REGISTER(id, instance) \
    esp_event_handler_instance_register( \
        WIFI_EVENT, (id), &port_wifi_sta_event_handler, NULL, (instance))
    /* STA 事件注册 */
    __STA_EVENT_REGISTER(ESP_EVENT_ANY_ID,              &ctx_w()->instance_sta);
#undef __STA_EVENT_REGISTER

#define __STA_IP_EVENT_REGISTER(id, instance) \
    esp_event_handler_instance_register( \
        IP_EVENT, (id), &port_wifi_sta_ip_event_handler, NULL, (instance))
    /* STA IP 事件注册 */
    __STA_IP_EVENT_REGISTER(IP_EVENT_STA_GOT_IP,        &ctx_w()->instance_sta_ip_got_ip);
    __STA_IP_EVENT_REGISTER(IP_EVENT_STA_LOST_IP,       &ctx_w()->instance_sta_ip_lost_ip);
    __STA_IP_EVENT_REGISTER(IP_EVENT_GOT_IP6,           &ctx_w()->instance_sta_ip_got_ip6);
#undef __STA_IP_EVENT_REGISTER
}

static void port_sta_netif_deinit(void)
{
    if (!ctx_w()->b_sta_netif_is_inited) {
        return;
    }
    ctx_w()->b_sta_netif_is_inited = false;

#define __STA_EVENT_UNREGISTER(id, instance) \
    esp_event_handler_instance_unregister(WIFI_EVENT, (id), (instance))
    /* STA 事件反注册 */
    __STA_EVENT_UNREGISTER(ESP_EVENT_ANY_ID,                ctx_w()->instance_sta);
#undef __STA_EVENT_UNREGISTER

#define __STA_IP_EVENT_UNREGISTER(id, instance) \
    esp_event_handler_instance_unregister(IP_EVENT, (id), (instance))
    /* STA IP 事件反注册 */
    __STA_IP_EVENT_UNREGISTER(IP_EVENT_STA_GOT_IP,          ctx_w()->instance_sta_ip_got_ip);
    __STA_IP_EVENT_UNREGISTER(IP_EVENT_STA_LOST_IP,         ctx_w()->instance_sta_ip_lost_ip);
    /* TODO IP_EVENT_GOT_IP6 事件被 wifi 和 eth 共享 */
    __STA_IP_EVENT_UNREGISTER(IP_EVENT_GOT_IP6,             ctx_w()->instance_sta_ip_got_ip6);
#undef __STA_IP_EVENT_UNREGISTER

    esp_netif_destroy_default_wifi(ctx_w()->netif_sta.esp_netif);
    ctx_w()->netif_sta.esp_netif = NULL;
}

static xf_err_t port_convert_wifi_auth_mode(
    xf_wifi_auth_mode_t *xf, wifi_auth_mode_t *pf, bool is_xf2pf)
{
    if ((NULL == xf) || (NULL == pf)) {
        return XF_FAIL;
    }
    static const port_convert_wifi_auth_mode_t port_convert_wifi_auth_mode[] = {
        {XF_WIFI_AUTH_OPEN,             WIFI_AUTH_OPEN,},
        {XF_WIFI_AUTH_WEP,              WIFI_AUTH_WEP,},
        {XF_WIFI_AUTH_WPA_PSK,          WIFI_AUTH_WPA_PSK,},
        {XF_WIFI_AUTH_WPA2_PSK,         WIFI_AUTH_WPA2_PSK,},
        {XF_WIFI_AUTH_WPA_WPA2_PSK,     WIFI_AUTH_WPA_WPA2_PSK,},
        {XF_WIFI_AUTH_ENTERPRISE,       WIFI_AUTH_ENTERPRISE,},
        {XF_WIFI_AUTH_WPA3_PSK,         WIFI_AUTH_WPA3_PSK,},
        {XF_WIFI_AUTH_WPA2_WPA3_PSK,    WIFI_AUTH_WPA2_WPA3_PSK,},
        {XF_WIFI_AUTH_WAPI_PSK,         WIFI_AUTH_WAPI_PSK,},
        {XF_WIFI_AUTH_OWE,              WIFI_AUTH_OWE,},
        {XF_WIFI_AUTH_WPA3_ENT_192,     WIFI_AUTH_WPA3_ENT_192,},
        {XF_WIFI_AUTH_MAX,              WIFI_AUTH_MAX,},
    };
#define _MODE_MAP(n) port_convert_wifi_auth_mode[n]
    if (is_xf2pf) {
        for (uint32_t i = 0; i < ARRAY_SIZE(port_convert_wifi_auth_mode); i++) {
            if (*xf == _MODE_MAP(i).xf) {
                *pf = _MODE_MAP(i).pf;
                return XF_OK;
            }
        }
        return XF_FAIL;
    }
    for (uint32_t i = 0; i < ARRAY_SIZE(port_convert_wifi_auth_mode); i++) {
        if (*pf == _MODE_MAP(i).pf) {
            *xf = _MODE_MAP(i).xf;
            return XF_OK;
        }
    }
#undef _MODE_MAP
    return XF_FAIL;
}

static wifi_auth_mode_t port_convert_xf2pf_wifi_auth_mode(xf_wifi_auth_mode_t authmode)
{
    xf_err_t xf_ret = XF_OK;
    wifi_auth_mode_t pf_auth_mode = 0;
    xf_ret = port_convert_wifi_auth_mode(&authmode, &pf_auth_mode, true);
    if (XF_OK == xf_ret) {
        return pf_auth_mode;
    }
    return WIFI_AUTH_MAX;
}

static xf_wifi_auth_mode_t port_convert_pf2xf_wifi_auth_mode(wifi_auth_mode_t authmode)
{
    xf_err_t xf_ret = XF_OK;
    xf_wifi_auth_mode_t xf_auth_mode = 0;
    xf_ret = port_convert_wifi_auth_mode(&xf_auth_mode, &authmode, false);
    if (XF_OK == xf_ret) {
        return xf_auth_mode;
    }
    return XF_WIFI_AUTH_MAX;
}

static xf_wifi_event_code_t port_convert_pf2xf_wifi_event(wifi_event_t event)
{
    switch (event) {
    case WIFI_EVENT_SCAN_DONE:          return XF_WIFI_EVENT_SCAN_DONE;             break;
    case WIFI_EVENT_STA_START:          return XF_WIFI_EVENT_STA_START;             break;
    case WIFI_EVENT_STA_STOP:           return XF_WIFI_EVENT_STA_STOP;              break;
    case WIFI_EVENT_STA_CONNECTED:      return XF_WIFI_EVENT_STA_CONNECTED;         break;
    case WIFI_EVENT_STA_DISCONNECTED:   return XF_WIFI_EVENT_STA_DISCONNECTED;      break;
    case WIFI_EVENT_AP_START:           return XF_WIFI_EVENT_AP_START;              break;
    case WIFI_EVENT_AP_STOP:            return XF_WIFI_EVENT_AP_STOP;               break;
    case WIFI_EVENT_AP_STACONNECTED:    return XF_WIFI_EVENT_AP_STA_CONNECTED;      break;
    case WIFI_EVENT_AP_STADISCONNECTED: return XF_WIFI_EVENT_AP_STA_DISCONNECTED;   break;
    default:
    case WIFI_EVENT_MAX:                return XF_WIFI_EVENT_MAX;                   break;
    }
}

static xf_ip_event_id_t port_convert_pf2xf_ip_event(ip_event_t event)
{
    switch (event) {
    case IP_EVENT_STA_GOT_IP:           return XF_IP_EVENT_GOT_IP;              break;
    case IP_EVENT_STA_LOST_IP:          return XF_IP_EVENT_LOST_IP;             break;
    case IP_EVENT_AP_STAIPASSIGNED:     return XF_IP_EVENT_IP_ASSIGNED;         break;
    case IP_EVENT_GOT_IP6:              return XF_IP_EVENT_GOT_IP6;             break;
    default:                            return XF_IP_EVENT_MAX;                 break;
    }
}
