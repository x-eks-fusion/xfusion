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

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_wifi.h"
#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define EXAMPLE_SCAN_CHANNEL_START      1
#define EXAMPLE_SCAN_CHANNEL_END        13
#define EXAMPLE_SCAN_RESULT_SIZE        20
#define EXAMPLE_SCAN_BLOCK_IS_ENABLE    1

#if EXAMPLE_SCAN_BLOCK_IS_ENABLE
#   define EXAMPLE_SCAN_BLOCK           true
#else
#   define EXAMPLE_SCAN_BLOCK           false
#endif

#define EXAMPLE_WIFI_NTASK_PRIORITY     15
#define EXAMPLE_WIFI_NTASK_PERIOD_MS    10

#define TBL_IT(v)                       {v, XSTR(v)}

/* ==================== [Typedefs] ========================================== */

typedef struct example_scan_context_s {
    uint8_t scan_complete;
    uint8_t curr_scan_channel;
    xf_wifi_ap_info_t ap_info[EXAMPLE_SCAN_RESULT_SIZE];
    uint32_t ap_num;
} ex_scan_ctx_t;

typedef struct example_authenticate_mode_message_s {
    xf_wifi_auth_mode_t code;
    const char *msg;
} ex_auth_mode_msg_t;

/* ==================== [Static Prototypes] ================================= */

static void _error_check_failed(void);
static void _wifi_sta_event_handler(
    xf_wifi_event_id_t event_id, void *event_data, void *user_args);

static const char *ex_auth_mode_to_name(xf_wifi_auth_mode_t code);

static void _wifi_ntask(xf_task_t task);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "scan";

static ex_scan_ctx_t ctx = {0};
static ex_scan_ctx_t *sp_ctx = &ctx;

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

    /* 3. 启动 sta */
    ERROR_CHECK(xf_wifi_sta_init(NULL));

    /* 4. 开始扫描任务 */
    xf_ntask_create_loop(_wifi_ntask, NULL,
                         EXAMPLE_WIFI_NTASK_PRIORITY,
                         EXAMPLE_WIFI_NTASK_PERIOD_MS);
}

/* ==================== [Static Functions] ================================== */

static void _wifi_ntask(xf_task_t task)
{
    XF_NTASK_BEGIN(task);
    XF_LOGI(TAG, "_wifi_ntask start!");
    sp_ctx->curr_scan_channel = EXAMPLE_SCAN_CHANNEL_START;

    for (;;) {
        xf_wifi_scan_cfg_t scan_cfg = {
            .channel        = sp_ctx->curr_scan_channel,
            .show_hidden    = true,
        };
        xf_wifi_scan_start(&scan_cfg, EXAMPLE_SCAN_BLOCK);

#if (!EXAMPLE_SCAN_BLOCK_IS_ENABLE)
        if (false == sp_ctx->scan_complete) {
            xf_ntask_delay(50);
        }
        sp_ctx->scan_complete = false;
#endif

        XF_LOGI(TAG, "------------------------------");
        xf_wifi_scan_get_result(
            sp_ctx->ap_info, ARRAY_SIZE(sp_ctx->ap_info), &sp_ctx->ap_num);
        XF_LOGI(TAG, "Channel %d scanning is complete. There are %d APs.",
                (int)sp_ctx->curr_scan_channel, (int)sp_ctx->ap_num);
        for (uint32_t i = 0;
                (i < ARRAY_SIZE(sp_ctx->ap_info)) && (i < sp_ctx->ap_num);
                i++) {
            XF_LOGI(TAG,
                    "\n"
                    "    index:     %d\n"
                    "    ssid:      %s\n"
                    "    bssid:     " XF_MACSTR "\n"
                    "    rssi:      %d\n"
                    "    authmode:  %s",
                    i,
                    sp_ctx->ap_info[i].ssid,
                    XF_MAC2STR(sp_ctx->ap_info[i].bssid),
                    sp_ctx->ap_info[i].rssi,
                    ex_auth_mode_to_name(sp_ctx->ap_info[i].authmode));
        }
        XF_LOGI(TAG, "------------------------------");

        /* 清除未读完的数据 */
        xf_wifi_scan_clear_result();
        memset(sp_ctx->ap_info, 0, sizeof(sp_ctx->ap_info));

        /* 扫描下一个通道 */
        ++sp_ctx->curr_scan_channel;
        if (sp_ctx->curr_scan_channel > EXAMPLE_SCAN_CHANNEL_END) {
            sp_ctx->curr_scan_channel = EXAMPLE_SCAN_CHANNEL_START;
            XF_LOGI(TAG, "done!");
            xf_ntask_delay(10000);
        }
    }
    XF_NTASK_END();
}

static void _wifi_sta_event_handler(
    xf_wifi_event_id_t event_id, void *event_data, void *user_args)
{
    UNUSED(user_args);
    UNUSED(event_data);
    switch (event_id) {
    case XF_WIFI_EVENT_SCAN_DONE: {
        sp_ctx->scan_complete   = true;
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
