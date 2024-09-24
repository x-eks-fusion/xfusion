/**
 * @file port_xf_netif.c
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

#include "esp_mac.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"

#include "esp_netif.h"
#include "esp_netif_net_stack.h"

#include "xf_utils.h"

#include "port_xf_netif.h"

#include "xf_netif.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

static const char *TAG = "port_xf_netif";
static port_xf_netif_context_t s_port_xf_netif_context = {0};
static port_xf_netif_context_t *sp_port_xf_netif_context = &s_port_xf_netif_context;
#define ctx_n() sp_port_xf_netif_context

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

port_xf_netif_context_t *port_xf_netif_get_context(void)
{
    return sp_port_xf_netif_context;
}

xf_err_t xf_netif_set_hostname(
    xf_netif_t netif_hdl, const char *hostname, uint32_t len)
{
    XF_CHECK(NULL == netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==netif_hdl");
    XF_CHECK(NULL == hostname, XF_ERR_INVALID_ARG,
             TAG, "NULL==hostname");
    UNUSED(len);
    port_xf_netif_hdl_ctx_t *p_hdl = (port_xf_netif_hdl_ctx_t *)netif_hdl;

    xf_err_t xf_ret;
    pf_err_t pf_err;
    pf_err = esp_netif_set_hostname(p_hdl->esp_netif, hostname);
    xf_ret = port_convert_pf2xf_err(pf_err);
    return xf_ret;
}

xf_err_t xf_netif_get_hostname(
    xf_netif_t netif_hdl, char hostname[], uint32_t len)
{
    XF_CHECK(NULL == netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==netif_hdl");
    XF_CHECK(NULL == hostname, XF_ERR_INVALID_ARG,
             TAG, "NULL==hostname");
    port_xf_netif_hdl_ctx_t *p_hdl = (port_xf_netif_hdl_ctx_t *)netif_hdl;
    const char *p_hostname = NULL;

    xf_err_t xf_ret;
    pf_err_t pf_err;
    pf_err = esp_netif_get_hostname(p_hdl->esp_netif, &p_hostname);
    xf_ret = port_convert_pf2xf_err(pf_err);

    if ((XF_OK == xf_ret) && (NULL != p_hostname)) {
        uint32_t hostname_len = (uint32_t)strlen(p_hostname);
        uint32_t len_min = (hostname_len > len) ? (len) : (hostname_len);
        memcpy(hostname, p_hostname, (size_t)len_min);
    }
    return xf_ret;
}

bool xf_netif_is_netif_up(xf_netif_t netif_hdl)
{
    XF_CHECK(NULL == netif_hdl, false,
             TAG, "NULL==netif_hdl");
    port_xf_netif_hdl_ctx_t *p_hdl = (port_xf_netif_hdl_ctx_t *)netif_hdl;
    return esp_netif_is_netif_up(p_hdl->esp_netif);
}

xf_err_t xf_netif_get_ip_info(xf_netif_t netif_hdl, xf_netif_ip_info_t *ip_info)
{
    XF_CHECK(NULL == netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==netif_hdl");
    XF_CHECK(NULL == ip_info, XF_ERR_INVALID_ARG,
             TAG, "NULL==ip_info");
    port_xf_netif_hdl_ctx_t *p_hdl = (port_xf_netif_hdl_ctx_t *)netif_hdl;

    esp_netif_ip_info_t esp_ip_info = {0};

    xf_err_t xf_ret;
    pf_err_t pf_err;
    pf_err = esp_netif_get_ip_info(p_hdl->esp_netif, &esp_ip_info);
    xf_ret = port_convert_pf2xf_err(pf_err);

    ip_info->ip.addr        = esp_ip_info.ip.addr;
    ip_info->netmask.addr   = esp_ip_info.netmask.addr;
    ip_info->gw.addr        = esp_ip_info.gw.addr;

    return xf_ret;
}

xf_err_t xf_netif_set_ip_info(xf_netif_t netif_hdl, const xf_netif_ip_info_t *ip_info)
{
    XF_CHECK(NULL == netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==netif_hdl");
    XF_CHECK(NULL == ip_info, XF_ERR_INVALID_ARG,
             TAG, "NULL==ip_info");
    port_xf_netif_hdl_ctx_t *p_hdl = (port_xf_netif_hdl_ctx_t *)netif_hdl;

    esp_netif_ip_info_t esp_ip_info = {0};
    esp_ip_info.ip.addr        = ip_info->ip.addr;
    esp_ip_info.netmask.addr   = ip_info->netmask.addr;
    esp_ip_info.gw.addr        = ip_info->gw.addr;

    xf_err_t xf_ret;
    pf_err_t pf_err;
    pf_err = esp_netif_set_ip_info(p_hdl->esp_netif, &esp_ip_info);
    xf_ret = port_convert_pf2xf_err(pf_err);

    return xf_ret;
}

xf_err_t xf_netif_dhcpc_start(xf_netif_t netif_hdl)
{
    XF_CHECK(NULL == netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==netif_hdl");
    port_xf_netif_hdl_ctx_t *p_hdl = (port_xf_netif_hdl_ctx_t *)netif_hdl;

    xf_err_t xf_ret;
    pf_err_t pf_err;
    pf_err = esp_netif_dhcpc_start(p_hdl->esp_netif);
    xf_ret = port_convert_pf2xf_err(pf_err);

    return xf_ret;
}

xf_err_t xf_netif_dhcpc_stop(xf_netif_t netif_hdl)
{
    XF_CHECK(NULL == netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==netif_hdl");
    port_xf_netif_hdl_ctx_t *p_hdl = (port_xf_netif_hdl_ctx_t *)netif_hdl;

    xf_err_t xf_ret;
    pf_err_t pf_err;
    pf_err = esp_netif_dhcpc_stop(p_hdl->esp_netif);
    xf_ret = port_convert_pf2xf_err(pf_err);

    return xf_ret;
}

xf_err_t xf_netif_dhcps_start(xf_netif_t netif_hdl)
{
    XF_CHECK(NULL == netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==netif_hdl");
    port_xf_netif_hdl_ctx_t *p_hdl = (port_xf_netif_hdl_ctx_t *)netif_hdl;

    xf_err_t xf_ret;
    pf_err_t pf_err;
    pf_err = esp_netif_dhcps_start(p_hdl->esp_netif);
    xf_ret = port_convert_pf2xf_err(pf_err);

    return xf_ret;
}

xf_err_t xf_netif_dhcps_stop(xf_netif_t netif_hdl)
{
    XF_CHECK(NULL == netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==netif_hdl");
    port_xf_netif_hdl_ctx_t *p_hdl = (port_xf_netif_hdl_ctx_t *)netif_hdl;

    xf_err_t xf_ret;
    pf_err_t pf_err;
    pf_err = esp_netif_dhcps_stop(p_hdl->esp_netif);
    xf_ret = port_convert_pf2xf_err(pf_err);

    return xf_ret;
}

xf_err_t xf_netif_dhcps_get_clients_by_mac(
    xf_netif_t netif_hdl,
    xf_netif_pair_mac_ip_t mac_ip_pair_array[], uint32_t pair_array_size)
{
    XF_CHECK(NULL == netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==netif_hdl");
    port_xf_netif_hdl_ctx_t *p_hdl = (port_xf_netif_hdl_ctx_t *)netif_hdl;

    xf_err_t xf_ret;
    pf_err_t pf_err;
    /*
        xf_netif_pair_mac_ip_t 与 esp_netif_pair_mac_ip_t 成员一致，此处不做转换。
     */
    pf_err = esp_netif_dhcps_get_clients_by_mac(
                 p_hdl->esp_netif,
                 (int)pair_array_size, (esp_netif_pair_mac_ip_t *)mac_ip_pair_array);
    xf_ret = port_convert_pf2xf_err(pf_err);

    return xf_ret;
}

xf_err_t xf_netif_set_dns_info(
    xf_netif_t netif_hdl, xf_netif_dns_type_t type, xf_netif_dns_info_t *dns)
{
    XF_CHECK(NULL == netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==netif_hdl");
    port_xf_netif_hdl_ctx_t *p_hdl = (port_xf_netif_hdl_ctx_t *)netif_hdl;

    xf_err_t xf_ret;
    pf_err_t pf_err;
    /*
        xf_netif_dns_type_t 与 esp_netif_dns_type_t 成员一致，此处不做转换。
        xf_netif_dns_info_t 与 esp_netif_dns_info_t 成员一致，此处不做转换。
     */
    pf_err = esp_netif_set_dns_info(
                 p_hdl->esp_netif,
                 (esp_netif_dns_type_t)type, (esp_netif_dns_info_t *)dns);
    xf_ret = port_convert_pf2xf_err(pf_err);

    return xf_ret;
}

xf_err_t xf_netif_get_dns_info(
    xf_netif_t netif_hdl, xf_netif_dns_type_t type, xf_netif_dns_info_t *dns)
{
    XF_CHECK(NULL == netif_hdl, XF_ERR_INVALID_ARG,
             TAG, "NULL==netif_hdl");
    port_xf_netif_hdl_ctx_t *p_hdl = (port_xf_netif_hdl_ctx_t *)netif_hdl;

    xf_err_t xf_ret;
    pf_err_t pf_err;
    /*
        xf_netif_dns_type_t 与 esp_netif_dns_type_t 成员一致，此处不做转换。
        xf_netif_dns_info_t 与 esp_netif_dns_info_t 成员一致，此处不做转换。
     */
    pf_err = esp_netif_get_dns_info(
                 p_hdl->esp_netif,
                 (esp_netif_dns_type_t)type, (esp_netif_dns_info_t *)dns);
    xf_ret = port_convert_pf2xf_err(pf_err);

    return xf_ret;
}

/* ==================== [Static Functions] ================================== */
