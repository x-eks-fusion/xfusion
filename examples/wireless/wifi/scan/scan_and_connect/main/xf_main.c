/**
 * @file xf_main.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-09-14
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/**
 * @example examples/wireless/wifi/scan/scan_and_connect/main/xf_main.c
 * xf_wal wifi 扫描和连接 示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_wifi.h"
#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define EXAMPLE_SCAN_CHANNEL_START      1       /*!< 开始通道 */
#define EXAMPLE_SCAN_CHANNEL_END        13      /*!< 结束通道（含） */
#define EXAMPLE_SCAN_RESULT_SIZE        32      /*!< AP 结果数组大小 */
#define EXAMPLE_SCAN_BLOCK_IS_ENABLE    1       /*!< 是否阻塞扫描 */

#if EXAMPLE_SCAN_BLOCK_IS_ENABLE
#   define EXAMPLE_SCAN_BLOCK           true
#else
#   define EXAMPLE_SCAN_BLOCK           false
#endif

#define EXAMPLE_WIFI_NTASK_PRIORITY     15      /*!< 扫描无栈任务优先级 */
#define EXAMPLE_WIFI_NTASK_PERIOD_MS    10      /*!< 扫描无栈任务周期 */

/**
 * @brief 用于创建枚举值与枚举名称的对应关系。
 */
#define TBL_IT(v)                       {v, XSTR(v)}

#define EXAMPLE_WIFI_SSID               "myssid"        /*!< 目标 AP 名称 */
#define EXAMPLE_WIFI_PASS               "mypassword"    /*!< 目标 AP 密码 */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief 扫描任务无栈协程上下文。
 */
typedef struct example_scan_context_s {
    uint8_t scan_complete;
    uint8_t curr_scan_channel;
    xf_wifi_ap_info_t ap_info[EXAMPLE_SCAN_RESULT_SIZE];
    uint32_t ap_num;
} ex_scan_ctx_t;

/**
 * @brief 认证模式与字符串消息。
 */
typedef struct example_authenticate_mode_message_s {
    xf_wifi_auth_mode_t code;
    const char *msg;
} ex_auth_mode_msg_t;

/* ==================== [Static Prototypes] ================================= */

static void _error_check_failed(void);
static void _wifi_sta_event_handler(
    xf_wifi_event_id_t event_id, void *event_data, void *user_args);
static void _ip_event_handler(
    xf_ip_event_id_t event_id, void *event_data, void *user_args);

/**
 * @brief 将 xf_wifi_auth_mode_t 的枚举值转为字符串消息。
 *
 * @param code 认证模式。
 * @return const char* 字符串消息。
 */
static const char *ex_auth_mode_to_name(xf_wifi_auth_mode_t code);

/**
 * @brief 扫描任务。
 *
 * 该任务中逐通道扫描，扫描完毕后，如果找到目标 AP，则尝试连接，如果没有扫描到则重试。
 *
 * @attention 无栈协程内所有临时变量都不安全。
 *            delay 后均会重置。
 *
 * @param task 本任务句柄。
 */
static void _wifi_ntask(xf_task_t task);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "scan";

/**
 * @brief 扫描无栈任务上下文。
 */
static ex_scan_ctx_t ctx = {0};
static ex_scan_ctx_t *sp_ctx = &ctx;
static uint8_t s_is_connected = false;

/**
 * @brief STA 配置。
 */
static xf_wifi_sta_cfg_t s_sta_cfg = {
    .ssid           = EXAMPLE_WIFI_SSID,
    .password       = EXAMPLE_WIFI_PASS,
    .bssid_set      = false,                /*!< 不配置目标 AP 的 MAC 地址 */
    .bssid          = {0},
    .authmode       = XF_WIFI_AUTH_OPEN,    /*!< 扫描到前未知 */
    .channel        = 0,                    /*!< 扫描到前未知 */
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

    /* 3. 启动 sta */
    ERROR_CHECK(xf_wifi_sta_init(NULL));

    /* 4. 开始扫描任务 */
    xf_ttask_create_loop(_wifi_ntask, NULL,
                         EXAMPLE_WIFI_NTASK_PRIORITY,
                         EXAMPLE_WIFI_NTASK_PERIOD_MS);
}

/* ==================== [Static Functions] ================================== */

static void _wifi_ntask(xf_task_t task)
{
    XF_NTASK_BEGIN(task);
    XF_LOGI(TAG, "_wifi_ntask start!");

    /* 初始化上下文内的变量 */
    sp_ctx->curr_scan_channel = EXAMPLE_SCAN_CHANNEL_START;
    sp_ctx->ap_num = 0;

    for (;;) {
        /* 启动扫描 */
        xf_wifi_scan_cfg_t scan_cfg = {
            .channel        = sp_ctx->curr_scan_channel,
            .show_hidden    = true,
        };
        xf_wifi_scan_start(&scan_cfg, EXAMPLE_SCAN_BLOCK);

#if (!EXAMPLE_SCAN_BLOCK_IS_ENABLE)
        /* 非阻塞时等待扫描结束 */
        if (false == sp_ctx->scan_complete) {
            xf_ntask_delay(50);
        }
        sp_ctx->scan_complete = false;
#endif

        /* 保存当前通道的扫描结果，这里只保存 EXAMPLE_SCAN_RESULT_SIZE 条 */
        uint32_t result_num = 0;
        xf_wifi_scan_get_result(
            &sp_ctx->ap_info[sp_ctx->ap_num],
            ARRAY_SIZE(sp_ctx->ap_info) - sp_ctx->ap_num,
            &result_num);
        sp_ctx->ap_num += result_num;

        /**
         * @attention 为了保持最大兼容性，
         * 此处不调用 xf_wifi_scan_clear_result() 清除扫描结果列表。
         *
         * 对于 esp32 清除扫描结果列表后不影响连接；
         * 而在 ws63 上，清除清除扫描结果列表后无法直接连接，扫描后不清除才能连接。
         *
         * 因此只在连接成功后，或者没有找到目标 AP 时清除列表。
         */

        /* 扫描下一个通道 */
        ++sp_ctx->curr_scan_channel;
        if (sp_ctx->curr_scan_channel > EXAMPLE_SCAN_CHANNEL_END) {
            sp_ctx->curr_scan_channel = EXAMPLE_SCAN_CHANNEL_START;

            /* 扫描完毕 */

            int32_t matched_index   = -1;
            int32_t cmp_res         = -1;

            /* 输出所有扫描结果 */
            XF_LOGI(TAG, "done!");
            XF_LOGI(TAG, "------------------------------");
            for (uint32_t i = 0;
                    (i < ARRAY_SIZE(sp_ctx->ap_info)) && (i < sp_ctx->ap_num);
                    i++) {
                XF_LOGI(TAG,
                        "\n"
                        "    index:     %d\n"
                        "    ssid:      %s\n"
                        "    bssid:     " XF_MACSTR "\n"
                        "    rssi:      %d\n"
                        "    authmode:  %s\n"
                        "    channel:   %d",
                        i,
                        sp_ctx->ap_info[i].ssid,
                        XF_MAC2STR(sp_ctx->ap_info[i].bssid),
                        sp_ctx->ap_info[i].rssi,
                        ex_auth_mode_to_name(sp_ctx->ap_info[i].authmode),
                        (int)sp_ctx->ap_info[i].channel);

                /* 比较扫描到的 SSID 与目标 SSID 是否相同 */
                cmp_res = strcmp(EXAMPLE_WIFI_SSID, (char *)sp_ctx->ap_info[i].ssid);
                if ((0 == cmp_res) && (-1 == matched_index)) {
                    /* 只匹配第一个扫描到的 ap */
                    matched_index = i;
                    /* 已知 SSID 和密码，需要配置认证模式和通道 */
                    s_sta_cfg.authmode  = sp_ctx->ap_info[matched_index].authmode;
                    s_sta_cfg.channel   = sp_ctx->ap_info[matched_index].channel;
                }
            } /* for 遍历 ap_info */
            XF_LOGI(TAG, "------------------------------");

            /* 找到对应的 ap 则连接 */
            if (matched_index != -1) {
                XF_LOGI(TAG, "AP(%s) has been found, try to connect.", EXAMPLE_WIFI_SSID);
                xf_wifi_sta_connect(&s_sta_cfg);
                while (1) {
                    xf_ntask_delay(50);
                    if (xf_wifi_sta_is_connected()) {
                        break;
                    }
                }
                goto l_succ;
            }

            /* 目标 AP 未找到，清除可能未读完的数据 */
            xf_wifi_scan_clear_result();

            sp_ctx->ap_num = 0;
            XF_LOGI(TAG, "AP(%s) Not found", EXAMPLE_WIFI_SSID);
            xf_ntask_delay(1000);
        } /* sp_ctx->curr_scan_channel > EXAMPLE_SCAN_CHANNEL_END */
    } /* for 扫描所有通道 */

l_succ:;
    /* 连接成功，清除可能未读完的数据 */
    xf_wifi_scan_clear_result();
    XF_LOGI(TAG, "successful connection!");
    XF_NTASK_END();
}

static void _wifi_sta_event_handler(
    xf_wifi_event_id_t event_id, void *event_data, void *user_args)
{
    UNUSED(user_args);

    switch (event_id) {
    case XF_WIFI_EVENT_STA_START: {
        XF_LOGI(TAG, "STA has started.");
        /* 扫描到前未知 ap 通道和认证模式，此处不连接 */
    } break;
    case XF_WIFI_EVENT_STA_STOP: {
        XF_LOGI(TAG, "STA has stopped.");
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
    case XF_WIFI_EVENT_SCAN_DONE: {
        sp_ctx->scan_complete   = true;
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

static const char *ex_auth_mode_to_name(xf_wifi_auth_mode_t code)
{
    static const ex_auth_mode_msg_t ex_auth_mode_msg_table[] = {
        TBL_IT(XF_WIFI_AUTH_OPEN),
        TBL_IT(XF_WIFI_AUTH_WEP),
        TBL_IT(XF_WIFI_AUTH_WPA_PSK),
        TBL_IT(XF_WIFI_AUTH_WPA2_PSK),
        TBL_IT(XF_WIFI_AUTH_WPA_WPA2_PSK),
        TBL_IT(XF_WIFI_AUTH_ENTERPRISE),
        TBL_IT(XF_WIFI_AUTH_WPA3_PSK),
        TBL_IT(XF_WIFI_AUTH_WPA2_WPA3_PSK),
        TBL_IT(XF_WIFI_AUTH_WAPI_PSK),
        TBL_IT(XF_WIFI_AUTH_OWE),
        TBL_IT(XF_WIFI_AUTH_WPA3_ENT_192),
        TBL_IT(XF_WIFI_AUTH_MAX),
    };

    unsigned int i = 0;
    for (i = 0; i < (unsigned int)ARRAY_SIZE(ex_auth_mode_msg_table); ++i) {
        if (ex_auth_mode_msg_table[i].code == code) {
            return ex_auth_mode_msg_table[i].msg;
        }
    }
    return ex_auth_mode_msg_table[ARRAY_SIZE(ex_auth_mode_msg_table) - 1].msg;
}

static void _error_check_failed(void)
{
    XF_LOGI(TAG, "Run-time error.");
    for (;;) {
    }
}
