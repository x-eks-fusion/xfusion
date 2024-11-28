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
 * @example examples/wireless/wifi/get_started/station/main/xf_main.c
 * xf_wal wifi station 示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_wifi.h"
#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define EXAMPLE_WIFI_SSID           "myssid"        /*!< 目标 AP 名称 */
#define EXAMPLE_WIFI_PASS           "mypassword"    /*!< 目标 AP 密码 */
#define EXAMPLE_WIFI_CHANNEL        6               /*!< 目标 AP 信道 */
#define EXAMPLE_WIFI_AUTHMODE       XF_WIFI_AUTH_WPA_WPA2_PSK   /*!< 目标 AP 认证模式 */

#define EXAMPLE_WIFI_NTASK_PRIORITY     15          /*!< 扫描无栈任务优先级 */
#define EXAMPLE_WIFI_NTASK_PERIOD_MS    10          /*!< 扫描无栈任务周期 */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void _error_check_failed(void);
static void _wifi_sta_event_handler(
    xf_wifi_event_id_t event_id, void *event_data, void *user_args);
static void _ip_event_handler(
    xf_ip_event_id_t event_id, void *event_data, void *user_args);

/**
 * @brief wifi 无栈任务。
 *
 * 不同平台的 wifi 回调处理可能不一致，有些回调运行在中断中，有些运行在任务中。
 * 有些平台在连接 AP 时，如果连接失败会调用回调，并发出 XF_WIFI_EVENT_STA_DISCONNECTED 事件，
 * 如果此时在中断中调用 xf_wifi_sta_connect() 会导致递归调用而栈溢出。
 *
 * 因此创建一个无栈任务处理 wifi 相关事件。
 *
 * @attention 无栈协程内所有临时变量都不安全。
 *            delay 后均会重置。
 *
 * @param task 本任务句柄。
 */
static void _wifi_ntask(xf_task_t task);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "sta";
static uint8_t s_sta_started = false;       /*!< STA 是否已启动 */
static uint8_t s_is_connected = false;      /*!< 是否已连接 */
static xf_wifi_sta_cfg_t s_sta_cfg = {
    .ssid           = EXAMPLE_WIFI_SSID,
    .password       = EXAMPLE_WIFI_PASS,
    .bssid_set      = false,                /*!< 不配置目标 AP 的 MAC 地址 */
    .bssid          = {0},
    .authmode       = EXAMPLE_WIFI_AUTHMODE,
    .channel        = EXAMPLE_WIFI_CHANNEL,
    .p_cfg_ext      = NULL,                 /*!< 不使用扩展配置 */
    .p_static_ip    = NULL,                 /*!< 不使用静态 IP */
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
    /* 1. 使能 wifi 协议栈 */
    ERROR_CHECK(xf_wifi_enable());

    /* 2. 设置 sta 事件回调 */
    ERROR_CHECK(xf_wifi_sta_set_cb(_wifi_sta_event_handler, NULL));
    ERROR_CHECK(xf_wifi_sta_set_ip_cb(_ip_event_handler, NULL));

    /* 3. 配置并启动 sta */
    ERROR_CHECK(xf_wifi_sta_init(&s_sta_cfg));

    /* 4. 开始 wifi 任务 */
    xf_ntask_create_loop(_wifi_ntask, NULL,
                         EXAMPLE_WIFI_NTASK_PRIORITY,
                         EXAMPLE_WIFI_NTASK_PERIOD_MS);
}

/* ==================== [Static Functions] ================================== */

static void _wifi_ntask(xf_task_t task)
{
    XF_NTASK_BEGIN(task);
    XF_LOGI(TAG, "_wifi_ntask start!");

    /* 等待 STA 启动 */
    while (false == s_sta_started) {
        xf_ntask_delay(20);
    }

    for (;;) {
        if (false == xf_wifi_sta_is_connected()) {
            /* 如果填 NULL，则使用 xf_wifi_sta_init() 设置的配置连接 */
            xf_wifi_sta_connect(NULL);
            XF_LOGI(TAG, "STA attempts to connect.");
            xf_ntask_delay(3000);
        }
        xf_ntask_delay(1000);
    }
    XF_NTASK_END();
}

static void _wifi_sta_event_handler(
    xf_wifi_event_id_t event_id, void *event_data, void *user_args)
{
    UNUSED(user_args);

    switch (event_id) {
    case XF_WIFI_EVENT_STA_START: {
        XF_LOGI(TAG, "STA has started.");
        s_sta_started = true;
    } break;
    case XF_WIFI_EVENT_STA_STOP: {
        XF_LOGI(TAG, "STA has stopped.");
        s_sta_started = false;
    } break;
    case XF_WIFI_EVENT_STA_CONNECTED: {
        s_is_connected = true;
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
        if (true == s_is_connected) {
            s_is_connected = false;
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
