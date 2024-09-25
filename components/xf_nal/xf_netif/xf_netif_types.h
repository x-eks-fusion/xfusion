/**
 * @file xf_netif_types.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-11
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_NETIF_TYPES_H__
#define __XF_NETIF_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#include "xf_mac.h"
#include "xf_netif_ip_addr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_NETIF_TODO       0

/* ==================== [Typedefs] ========================================== */

#if XF_NETIF_TODO
/**
 * @brief 网络设备类型码，用于查询 netif 句柄归属。
 * 
 * @todo 暂未使用。
 */
typedef enum _xf_netif_type_code_t {
    XF_NETIF_TYPE_BASE = 0,

    XF_NETIF_TYPE_STA = XF_NETIF_TYPE_BASE,
    XF_NETIF_TYPE_AP,
    XF_NETIF_TYPE_ETH,

    XF_NETIF_TYPE_CUSTOM0,
    XF_NETIF_TYPE_CUSTOM1,
    XF_NETIF_TYPE_CUSTOM2,
    XF_NETIF_TYPE_CUSTOM3,

    XF_NETIF_TYPE_MAX,
} xf_netif_type_code_t;
#endif /* XF_NETIF_TODO */

/**
 * @brief xfusion netif 句柄。
 */
typedef void *xf_netif_t;

/**
 * @brief DNS 服务器类型。
 */
typedef enum _xf_netif_dns_type_t {
    XF_NETIF_DNS_MAIN = 0,      /*!< DNS 主服务器 */
    XF_NETIF_DNS_BACKUP,        /*!< DNS 备份服务器 */
    XF_NETIF_DNS_MAX,           /*!< DNS 服务器无效值 */
} xf_netif_dns_type_t;

/**
 * @brief DNS 服务器信息。
 */
typedef struct _xf_netif_dns_info_t {
    xf_ip_addr_t ip; /*!< DNS 服务器的 IPV4 地址 */
} xf_netif_dns_info_t;

/**
 * @brief DHCP 客户端的地址信息（MAC 和 IP 地址对）。
 */
typedef struct _xf_netif_pair_mac_ip_t {
    uint8_t mac[XF_MAC_LEN_MAX];    /*!< 客户端 MAC 地址 */
    xf_ip4_addr_t ip;               /*!< 客户端 IP 地址 */
} xf_netif_pair_mac_ip_t;

/**
 * @brief IP 事件声明。
 */
typedef enum _xf_ip_event_code_t {
    XF_IP_EVENT_BASE = 0,           /*!< IP 事件 ID 起始。 */

    XF_IP_EVENT_GOT_IP = XF_IP_EVENT_BASE,      /*!< 站点从连接的 AP 获取 IP, @see xf_ip_event_got_ip_t */
    /* TODO 暂未支持 */ XF_IP_EVENT_LOST_IP,         /*!< 站点丢失 IP 且 IP 重置为 0 */
    XF_IP_EVENT_IP_ASSIGNED,        /*!< soft-AP 为连接的站点分配 IP, @see xf_ip_event_ap_staipassigned_t */
    /* TODO 暂未支持 */ XF_IP_EVENT_GOT_IP6,         /*!< 获得 IPv6 IP */
    XF_IP_EVENT_MAX,
} xf_ip_event_code_t;

/**
 * @brief ip 事件 id。见 @ref xf_ip_event_code_t.
 */
typedef int32_t xf_ip_event_id_t;

/**
 * @brief XF_IP_EVENT_STA_GOT_IP 事件的事件结构。
 */
typedef struct _xf_ip_event_got_ip_t {
    xf_netif_t netif_hdl;           /*!< netif 对象句柄 */
    xf_netif_ip_info_t ip_info;     /*!< IP 地址、网络掩码、网关 IP 地址 */
} xf_ip_event_got_ip_t;

/**
 * @brief XF_IP_EVENT_AP_STAIPASSIGNED 事件的事件结构。
 */
typedef struct _xf_ip_event_ip_assigned_t {
    xf_netif_t netif_hdl;           /*!< netif 对象句柄 */
    xf_ip4_addr_t ip;               /*!< 分配给 STA 的 IP 地址 */
    uint8_t mac[XF_MAC_LEN_MAX];    /*!< 客户端的 MAC 地址*/
} xf_ip_event_ip_assigned_t;

/**
 * @brief ip 回调函数原型。
 *
 * @param event_id 事件 id. 根据事件类型, 见 @ref xf_ip_event_code_t.
 * @param event_data 事件数据. 需要根据事件 id 强转为对应的类型。
 * @param user_args 用户数据. `xf_ip_set_cb(..., user_args);`时传入.
 */
typedef void (*xf_ip_cb_t)(
    xf_ip_event_id_t event_id, void *event_data, void *user_args);

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_NETIF_TYPES_H__ */
