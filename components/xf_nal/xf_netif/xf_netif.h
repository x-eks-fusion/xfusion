/**
 * @file xf_netif.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-11
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_NETIF_H__
#define __XF_NETIF_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_netif_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 设置当前 netif 句柄对应的主机名。
 *
 * @attention 为了确保平台兼容性（有些平台只保存指针，有些平台是拷贝副本），
 *            传入的 hostname 必须随时可读取！
 *
 * @param netif_hdl netif 句柄。
 * @param hostname 主机名。必须随时可读取！
 * @param len 主机名的长度。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_netif_set_hostname(
    xf_netif_t netif_hdl, const char *hostname, uint32_t len);

/**
 * @brief 获取当前 netif 句柄对应的主机名。
 *
 * @param netif_hdl netif 句柄。
 * @param[out] hostname 传出主机名字符串。
 * @param len 主机名字符串数组最大长度。如果 hostname 过小可能导致截断。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_netif_get_hostname(
    xf_netif_t netif_hdl, char hostname[], uint32_t len);

/**
 * @brief 检测当前 netif 句柄对应设备是否上线。
 *
 * @param netif_hdl netif 句柄。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
bool xf_netif_is_netif_up(xf_netif_t netif_hdl);

/**
 * @brief 获取指定 netif 句柄的 IPv4 信息(ip地址、网关、子网掩码)。
 *
 * @param netif_hdl netif 句柄。
 * @param[out] ip_info 传出 IPv4 信息。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_netif_get_ip_info(xf_netif_t netif_hdl, xf_netif_ip_info_t *ip_info);

/**
 * @brief 设置指定 netif 句柄的 IPv4 信息(ip地址、网关、子网掩码)。
 *
 * @param netif_hdl netif 句柄。
 * @param ip_info 要设置的 IPv4 信息
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_netif_set_ip_info(xf_netif_t netif_hdl, const xf_netif_ip_info_t *ip_info);

/* TODO 配置 DHCP 客户端选项 */

/**
 * @brief 设置指定 netif 句柄启动 DHCP 客户端。
 *
 * @param netif_hdl netif 句柄。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_netif_dhcpc_start(xf_netif_t netif_hdl);

/**
 * @brief 设置指定 netif 句柄停止 DHCP 客户端。
 *
 * @param netif_hdl netif 句柄。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_netif_dhcpc_stop(xf_netif_t netif_hdl);

/* TODO 配置 DHCP 服务端选项 */

/**
 * @brief 设置指定 netif 句柄启动 DHCP 服务端。
 *
 * @param netif_hdl netif 句柄。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_netif_dhcps_start(xf_netif_t netif_hdl);

/**
 * @brief 设置指定 netif 句柄停止 DHCP 服务端。
 *
 * @param netif_hdl netif 句柄。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_netif_dhcps_stop(xf_netif_t netif_hdl);

/**
 * @brief 根据 MAC 地址获取对应的 IP（对于 DHCP 服务器）。
 *
 * @param netif_hdl netif 句柄。
 * @param mac_ip_pair_array MAC 和 IP 地址对的数组(MAC 是输入，IP 输出)。
 * @param pair_array_size 数组大小，单位：个。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_netif_dhcps_get_clients_by_mac(
    xf_netif_t netif_hdl,
    xf_netif_pair_mac_ip_t mac_ip_pair_array[], uint32_t pair_array_size);

/**
 * @brief 设置指定 netif 句柄的 DNS 服务端。
 *
 * @param netif_hdl netif 句柄。
 * @param type DNS 服务器类型。
 * @param dns DNS 服务器信息。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_netif_set_dns_info(
    xf_netif_t netif_hdl, xf_netif_dns_type_t type, xf_netif_dns_info_t *dns);

/**
 * @brief 从指定 netif 句柄获取的 DNS 服务端信息。
 *
 * @param netif_hdl netif 句柄。
 * @param type DNS 服务器类型。
 * @param[out] dns DNS 服务器信息。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_netif_get_dns_info(
    xf_netif_t netif_hdl, xf_netif_dns_type_t type, xf_netif_dns_info_t *dns);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_NETIF_H__ */
