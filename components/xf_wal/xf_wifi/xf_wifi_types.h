/**
 * @file xf_wifi_types.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_WIFI_TYPES_H__
#define __XF_WIFI_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#include "xf_mac.h"
#include "xf_netif_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/**
 * @brief wifi ssid 最大长度。
 * 32 字节有效字符 + 一个 '\0'。
 */
#define XF_WIFI_SSID_LEN_MAX        (33)

/**
 * @brief wifi 密码最大长度。
 * 64 字节有效字符 + 一个 '\0'。
 */
#define XF_WIFI_PASSWORD_LEN_MAX    (65)

/* ==================== [Typedefs] ========================================== */

/**
 * @brief wifi 接口类型枚举。
 */
typedef enum _xf_wifi_interface_t {
    XF_WIFI_IF_STA = 0,
    XF_WIFI_IF_AP,
    XF_WIFI_IF_MAX,
} xf_wifi_interface_t;

/**
 * @brief 授权模式的强度。
 * OPEN < WEP < WPA_PSK < OWE < WPA2_PSK = WPA_WPA2_PSK < WAPI_PSK < WPA3_PSK = WPA2_WPA3_PSK
 */
typedef enum _xf_wifi_authenticate_mode_t {
    XF_WIFI_AUTH_OPEN = 0,              /*!< authenticate mode : open */
    XF_WIFI_AUTH_WEP,                   /*!< authenticate mode : WEP */
    XF_WIFI_AUTH_WPA_PSK,               /*!< authenticate mode : WPA_PSK */
    XF_WIFI_AUTH_WPA2_PSK,              /*!< authenticate mode : WPA2_PSK */
    XF_WIFI_AUTH_WPA_WPA2_PSK,          /*!< authenticate mode : WPA_WPA2_PSK */
    XF_WIFI_AUTH_ENTERPRISE,            /*!< authenticate mode : WiFi EAP security */
    XF_WIFI_AUTH_WPA2_ENTERPRISE = XF_WIFI_AUTH_ENTERPRISE,  /*!< authenticate mode : WiFi EAP security */
    XF_WIFI_AUTH_WPA3_PSK,              /*!< authenticate mode : WPA3_PSK */
    XF_WIFI_AUTH_WPA2_WPA3_PSK,         /*!< authenticate mode : WPA2_WPA3_PSK */
    XF_WIFI_AUTH_WAPI_PSK,              /*!< authenticate mode : WAPI_PSK */
    XF_WIFI_AUTH_OWE,                   /*!< authenticate mode : OWE */
    XF_WIFI_AUTH_WPA3_ENT_192,          /*!< authenticate mode : WPA3_ENT_SUITE_B_192_BIT */
    XF_WIFI_AUTH_MAX
} xf_wifi_auth_mode_t;

/**
 * @brief WiFi 国家码。
 */
typedef struct _xf_wifi_country_t {
    char cc[3];   /*!< 国家/地区代码字符串 */
} xf_wifi_country_t;

/**
 * @brief wifi 静态 IP。
 */
typedef struct _xf_wifi_static_ip_configuration_t {
    uint8_t type;                       /*!< IP 地址类型，内部用于选择 ip4 或 ip6, @ref XF_IPADDR_TYPE_V4 */
    xf_netif_ip_info_t ip4;             /*!< IPv4 静态 IP、网关、子网掩码 */
    xf_netif_ip6_info_t ip6;            /*!< IPv6 静态 IP、子网前缀长度、网关 */
} xf_wifi_static_ip_cfg_t;

/**
 * @brief Soft-AP 扩展配置结构体。
 * 谨慎使用，不同平台对这些配置的处理不一定相同。
 */
typedef struct _xf_wifi_softap_configuration_extended_t {
    uint32_t b_set_country          : 1;    /*!< 是否设置国家码国家码。 */
    uint32_t b_set_beacon_interval  : 1;    /*!< 是否设置信标间隔。 */
    uint32_t b_set_mac              : 1;    /*!< 是否设置 AP 的 MAC 地址。 */
    uint32_t reserve                : 29;   /*!< 保留。 */

    xf_wifi_country_t country;              /*!< 国家码  */
    uint16_t beacon_interval;               /*!< 信标间隔。单位: ms. 默认值为 100. */
    uint8_t mac[XF_MAC_LEN_MAX];            /*!< AP 的 MAC 地址, b_set_mac == 1 时有效。 */
} xf_wifi_ap_cfg_ext_t;

/**
 * @brief Soft-AP 基础配置结构体.
 */
typedef struct _xf_wifi_softap_configuration_t {
    uint8_t ssid[XF_WIFI_SSID_LEN_MAX];         /*!< soft-AP 的 SSID. 必须以 '\0' 结尾。 */
    uint8_t password[XF_WIFI_PASSWORD_LEN_MAX]; /*!< soft-AP 的密码。必须以 '\0' 结尾。 */
    xf_wifi_auth_mode_t authmode;               /*!< soft-AP 的认证模式。 */
    uint8_t channel;                            /*!< soft-AP 的通道, 1~13. */
    bool ssid_hidden;                           /*!< 是否隐藏 SSID，默认 0, 即开启 SSID 广播。 */
    xf_wifi_ap_cfg_ext_t *p_cfg_ext;            /*!< 扩展配置，传入 NULL 时表示使用平台默认配置。 */
    xf_wifi_static_ip_cfg_t *p_static_ip;       /*!< 静态 IP 配置。填入 NULL 时表示使用 DHCP. */
} xf_wifi_ap_cfg_t;

/**
 * @brief STA 扩展配置结构体。
 */
typedef struct _xf_wifi_station_configuration_extended_t {
    uint32_t b_set_mac              : 1;        /*!< 是否设置 STA 的 MAC 地址。  */
    uint32_t reserve                : 31;       /*!< 保留。 */
    uint8_t mac[XF_MAC_LEN_MAX];                /*!< STA 的 MAC 地址, b_set_mac == 1 时有效。 */
} xf_wifi_sta_cfg_ext_t;

/**
 * @brief STA 基础配置结构体。
 */
typedef struct _xf_wifi_station_configuration_t {
    uint8_t ssid[XF_WIFI_SSID_LEN_MAX];         /*!< 目标 AP 的 SSID. 必须以 '\0' 结尾。 */
    uint8_t password[XF_WIFI_PASSWORD_LEN_MAX]; /*!< 目标 AP 的密码。必须以 '\0' 结尾。 */
    uint8_t bssid[XF_MAC_LEN_MAX];              /*!< 目标 AP 的 MAC 地址 */
    bool bssid_set;                             /*!< 是否设置目标 AP 的 MAC 地址。
                                                 *   仅当需要以 MAC 地址连接时才需要设置为 1.
                                                 */
    xf_wifi_auth_mode_t authmode;               /*!< 目标 AP 的认证模式。 */
    uint8_t channel;                            /*!< 目标 AP 的通道。
                                                 *   如果 AP 的信道未知，则设置为 0。
                                                 */
    xf_wifi_sta_cfg_ext_t *p_cfg_ext;           /*!< 扩展配置，传入 NULL 时表示使用平台默认配置。
                                                 *   只有在调用 xf_wifi_sta_init() 时有效。
                                                 */
    xf_wifi_static_ip_cfg_t *p_static_ip;       /*!< 静态 IP 配置。填入 NULL 时表示使用 DHCP. 
                                                 *   只有在调用 xf_wifi_sta_init() 时有效。
                                                 */
} xf_wifi_sta_cfg_t;

/**
 * @brief 连接到当前 AP 的 STA 的信息。
 */
typedef struct _xf_wifi_station_information_t {
    uint8_t mac[XF_MAC_LEN_MAX];    /*!< MAC 地址 */
    int8_t rssi;                    /*!< 当前连接的 STA 平均 RSSI */
} xf_wifi_sta_info_t;

/** @brief Parameters for an SSID scan. */

/**
 * @brief wifi 扫描参数。
 *
 * 目前只支持指定通道扫描。
 *
 * @todo 基于 ssid 或者 bssid 扫描。
 * @todo 扫描模式：快速扫描、全通道扫描。
 */
typedef struct _xf_wifi_scan_cfg_t {
    /* TODO 暂未支持 */ uint8_t *ssid;      /*!< AP 的 SSID. */
    /* TODO 暂未支持 */ uint8_t *bssid;     /*!< 目标 AP 的 MAC 地址。 */
    uint8_t channel;    /*!< 扫描通道. 通常指定 1~13. 0 时为全通道扫描，
                         *   但是由于内存原因并非所有平台都支持。
                         */
    bool show_hidden;   /*!< 启用扫描隐藏 SSID 的 AP. */
} xf_wifi_scan_cfg_t;

/**
 * @brief AP 信息。
 * 扫描结果中使用。
 */
typedef struct _xf_wifi_ap_info_t {
    uint8_t bssid[XF_MAC_LEN_MAX];      /*!< AP 的 MAC 地址。 */
    uint8_t ssid[XF_WIFI_SSID_LEN_MAX]; /*!< AP 的 SSID. */
    int8_t  rssi;                       /*!< AP 的信号强度。 */
    xf_wifi_auth_mode_t authmode;       /*!< AP 的授权模式。 */
    uint8_t channel;                    /*!< AP 的通道，指定通道扫描时该值可能无用（因为已经知道通道）。 */
} xf_wifi_ap_info_t;

/**
 * @brief WiFi 事件声明。
 */
typedef enum _xf_wifi_event_code_t {
    XF_WIFI_EVENT_BASE = 0,             /*!< WiFi 事件 ID 起始。 */

    XF_WIFI_EVENT_SCAN_DONE = XF_WIFI_EVENT_BASE,   /*!< 扫描 AP 完毕。
                                                     *   该事件无数据。
                                                     *   扫描结果通过 xf_wifi_get_scan_result() 获取。
                                                     */

    XF_WIFI_EVENT_STA_START,            /*!< STA 启动。该事件无数据。 */
    XF_WIFI_EVENT_STA_STOP,             /*!< STA 停止。该事件无数据。 */
    XF_WIFI_EVENT_STA_CONNECTED,        /*!< STA 已连接到 AP。
                                         *   事件数据类型见 @ref xf_wifi_event_sta_conn_t.
                                         */
    XF_WIFI_EVENT_STA_DISCONNECTED,     /*!< STA 与 AP 断开连接。
                                         *   事件数据类型见 @ref xf_wifi_event_sta_disconn_t.
                                         */

    XF_WIFI_EVENT_AP_START,             /*!< soft-AP 启动。该事件无数据。 */
    XF_WIFI_EVENT_AP_STOP,              /*!< soft-AP 停止。该事件无数据。 */
    XF_WIFI_EVENT_AP_STA_CONNECTED,     /*!< 有 STA 连接到 soft-AP.
                                         *   事件数据类型见 @ref xf_wifi_event_ap_sta_conn_t.
                                         */
    XF_WIFI_EVENT_AP_STA_DISCONNECTED,  /*!< 有 STA 与 soft-AP 断开连接。
                                         *   事件数据类型见 @ref xf_wifi_event_ap_sta_disconn_t.
                                         */

    XF_WIFI_EVENT_MAX,                  /*!< WiFi 事件 ID 最大值，无效的 WiFi 事件 ID. */
} xf_wifi_event_code_t;

/**
 * @brief wifi 事件 id。见 @ref xf_wifi_event_code_t.
 */
typedef int32_t xf_wifi_event_id_t;

/**
 * @brief STA 事件消息类型: 已连接到 AP。
 * 事件 id 见 @ref XF_WIFI_EVENT_STA_CONNECTED.
 */
typedef struct _xf_wifi_event_sta_connected_t {
    uint8_t ssid[XF_WIFI_SSID_LEN_MAX]; /*!< 已连接到 AP 的 SSID */
    uint8_t bssid[XF_MAC_LEN_MAX];      /*!< 已连接到 AP 的 MAC */
    uint8_t channel;                    /*!< 已连接到 AP 的信道 */
    xf_wifi_auth_mode_t authmode;       /*!< 已连接到 AP 的使用的认证方式 */
} xf_wifi_event_sta_conn_t;

/**
 * @brief STA 事件消息类型: 已从 AP 断开连接。
 * 事件 id 见 @ref XF_WIFI_EVENT_STA_DISCONNECTED.
 */
typedef struct _xf_wifi_event_sta_disconnected_t {
    uint8_t ssid[XF_WIFI_SSID_LEN_MAX]; /*!< 已断开 AP 的 SSID */
    uint8_t bssid[XF_MAC_LEN_MAX];      /*!< 已断开 AP 的 MAC */
    /* TODO 暂未支持 */ uint8_t reason;                     /*!< 断连原因 */
    int8_t rssi;                        /*!< 断连时的 rssi */
} xf_wifi_event_sta_disconn_t;

/**
 * @brief AP 事件消息类型: STA 连接。
 * 事件 id 见 @ref XF_WIFI_EVENT_AP_STA_CONNECTED.
 */
typedef struct _xf_wifi_event_ap_sta_connected_t {
    uint8_t mac[XF_MAC_LEN_MAX];        /*!< 连接到 soft-AP 的 STA MAC 地址 */
} xf_wifi_event_ap_sta_conn_t;

/**
 * @brief AP 事件消息类型: STA 断开连接。
 * 事件 id 见 @ref XF_WIFI_EVENT_AP_STA_DISCONNECTED.
 */
typedef struct _xf_wifi_event_ap_sta_disconnected_t {
    uint8_t mac[XF_MAC_LEN_MAX];        /*!< 与 soft-AP 断开连接的的 STA MAC 地址 */
} xf_wifi_event_ap_sta_disconn_t;

/**
 * @brief wifi 回调函数原型。
 *
 * @param event_id 事件 id. 根据事件类型, 见 @ref xf_wifi_event_code_t.
 * @param event_data 事件数据. 需要根据事件 id 强转为对应的类型.
 * @param user_args 用户数据. `xf_wifi_ap/sta_set_cb(..., user_args);`时传入.
 */
typedef void (*xf_wifi_cb_t)(
    xf_wifi_event_id_t event_id, void *event_data, void *user_args);

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_WIFI_TYPES_H__ */
