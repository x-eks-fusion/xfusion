/**
 * @file xf_netif_ip_addr_types.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-11
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_NETIF_IP_ADDR_TYPES_H__
#define __XF_NETIF_IP_ADDR_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_netif_config_internal.h"

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_init
 * @endcond
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/*
    TODO 机器大小端目前是手动选择的
    或者：
    #include <machine/endian.h>
    defined(BYTE_ORDER) && defined(BIG_ENDIAN) && (BYTE_ORDER == BIG_ENDIAN)
 */
#if (XF_NAL_BIG_ENDIAN_IS_ENABLE == 1)
#define xf_netif_htonl(x)       ((uint32_t)(x))
#else
#define xf_netif_htonl(x) \
    ((((x) & (uint32_t)0x000000ffUL) << 24) | \
     (((x) & (uint32_t)0x0000ff00UL) <<  8) | \
     (((x) & (uint32_t)0x00ff0000UL) >>  8) | \
     (((x) & (uint32_t)0xff000000UL) >> 24))
#endif

#define xf_netif_ip4_makeu32(a,b,c,d) \
    (((uint32_t)((a) & 0xff) << 24) | \
     ((uint32_t)((b) & 0xff) << 16) | \
     ((uint32_t)((c) & 0xff) << 8 ) | \
     ((uint32_t)((d) & 0xff)      ))

#define XF_IPSTR                "%d.%d.%d.%d"
#define xf_ip4_addr_get_byte(ipaddr, idx) (((const uint8_t*)(&(ipaddr)->addr))[idx])
#define xf_ip4_addr1(ipaddr)    xf_ip4_addr_get_byte(ipaddr, 0)
#define xf_ip4_addr2(ipaddr)    xf_ip4_addr_get_byte(ipaddr, 1)
#define xf_ip4_addr3(ipaddr)    xf_ip4_addr_get_byte(ipaddr, 2)
#define xf_ip4_addr4(ipaddr)    xf_ip4_addr_get_byte(ipaddr, 3)

#define xf_ip4_addr1_16(ipaddr) ((uint16_t)xf_ip4_addr1(ipaddr))
#define xf_ip4_addr2_16(ipaddr) ((uint16_t)xf_ip4_addr2(ipaddr))
#define xf_ip4_addr3_16(ipaddr) ((uint16_t)xf_ip4_addr3(ipaddr))
#define xf_ip4_addr4_16(ipaddr) ((uint16_t)xf_ip4_addr4(ipaddr))

#define XF_IP2STR(ipaddr) \
    xf_ip4_addr1_16(ipaddr), \
    xf_ip4_addr2_16(ipaddr), \
    xf_ip4_addr3_16(ipaddr), \
    xf_ip4_addr4_16(ipaddr)

#define XF_IP6_ADDR_BLOCK1(ip6addr) ((uint16_t)((xf_netif_htonl((ip6addr)->addr[0]) >> 16) & 0xffff))
#define XF_IP6_ADDR_BLOCK2(ip6addr) ((uint16_t)((xf_netif_htonl((ip6addr)->addr[0])) & 0xffff))
#define XF_IP6_ADDR_BLOCK3(ip6addr) ((uint16_t)((xf_netif_htonl((ip6addr)->addr[1]) >> 16) & 0xffff))
#define XF_IP6_ADDR_BLOCK4(ip6addr) ((uint16_t)((xf_netif_htonl((ip6addr)->addr[1])) & 0xffff))
#define XF_IP6_ADDR_BLOCK5(ip6addr) ((uint16_t)((xf_netif_htonl((ip6addr)->addr[2]) >> 16) & 0xffff))
#define XF_IP6_ADDR_BLOCK6(ip6addr) ((uint16_t)((xf_netif_htonl((ip6addr)->addr[2])) & 0xffff))
#define XF_IP6_ADDR_BLOCK7(ip6addr) ((uint16_t)((xf_netif_htonl((ip6addr)->addr[3]) >> 16) & 0xffff))
#define XF_IP6_ADDR_BLOCK8(ip6addr) ((uint16_t)((xf_netif_htonl((ip6addr)->addr[3])) & 0xffff))

#define XF_IPV6STR "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x"

#define XF_IPV62STR(ipaddr) \
    XF_IP6_ADDR_BLOCK1(&(ipaddr)), \
    XF_IP6_ADDR_BLOCK2(&(ipaddr)), \
    XF_IP6_ADDR_BLOCK3(&(ipaddr)), \
    XF_IP6_ADDR_BLOCK4(&(ipaddr)), \
    XF_IP6_ADDR_BLOCK5(&(ipaddr)), \
    XF_IP6_ADDR_BLOCK6(&(ipaddr)), \
    XF_IP6_ADDR_BLOCK7(&(ipaddr)), \
    XF_IP6_ADDR_BLOCK8(&(ipaddr))

#define XF_IPADDR_TYPE_V4                0U
#define XF_IPADDR_TYPE_V6                6U
#define XF_IPADDR_TYPE_ANY               46U

#define XF_IP4TOUINT32(a,b,c,d) \
    (((uint32_t)((a) & 0xffU) << 24) | \
     ((uint32_t)((b) & 0xffU) << 16) | \
     ((uint32_t)((c) & 0xffU) << 8 ) | \
     ((uint32_t)((d) & 0xffU)      ))

#define XF_IP4TOADDR(a, b, c, d)        xf_netif_htonl(XF_IP4TOUINT32(a, b, c, d))

/**
 * @brief @ref xf_ip_addr_t.
 */
#define XF_IP4ADDR_INIT(a, b, c, d) \
    (xf_ip_addr_t) { \
        .type = XF_IPADDR_TYPE_V4, \
        .u_addr = { \
            .ip4 = { \
                .addr = XF_IP4TOADDR(a, b, c, d), \
            } \
        } \
    }

/**
 * @brief @ref xf_ip_addr_t.
 */
#define XF_IP6ADDR_INIT(a, b, c, d) \
    (xf_ip_addr_t) { \
        .type = XF_IPADDR_TYPE_V6, \
        .u_addr = { \
            .ip6 = { \
                .addr = { a, b, c, d }, \
                .zone = 0, \
            } \
        } \
    }

#define XF_IP4_ADDR(ipaddr, a, b, c, d)  (ipaddr)->addr = xf_netif_htonl(xf_netif_ip4_makeu32(a, b, c, d))

/* ==================== [Typedefs] ========================================== */

typedef struct xf_ip4_addr {
    uint32_t addr;          /*!< IPv4 地址 */
} xf_ip4_addr_t;

typedef struct xf_ip6_addr {
    uint32_t addr[4];       /*!< IPv6 地址 */
    uint8_t zone;           /*!< zone ID */
} xf_ip6_addr_t;

typedef struct _xf_ip_addr {
    union {
        xf_ip6_addr_t ip6;  /*!< IPv6 地址类型 */
        xf_ip4_addr_t ip4;  /*!< IPv4 地址类型 */
    } u_addr;               /*!< IP 地址共用体 */
    uint8_t type;           /*!< IP 地址类型, @ref XF_IPADDR_TYPE_V4 */
} xf_ip_addr_t;

typedef struct _xf_netif_ip_info_t {
    xf_ip4_addr_t ip;       /*!< 接口 IPV4 地址 */
    xf_ip4_addr_t gw;       /*!< 接口 IPV4 网关地址 */
    xf_ip4_addr_t netmask;  /*!< 接口 IPV4 网络掩码 */
} xf_netif_ip_info_t;

typedef struct _xf_netif_ip6_info_t {
    xf_ip6_addr_t ip;       /*!< 接口 IPV6 地址 */
    uint8_t prefix_len;     /*!< 子网前缀长度 */
    xf_ip6_addr_t gw;       /*!< 接口 IPV6 网关 */
} xf_netif_ip6_info_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 复制 IP 地址。
 *
 * @param[out] dest 目标 IP。
 * @param src 源 IP。
 */
static inline void xf_netif_ip_addr_types_copy(xf_ip_addr_t *dest, const xf_ip_addr_t *src)
{
    dest->type = src->type;
    if (src->type == XF_IPADDR_TYPE_V6) {
        dest->u_addr.ip6.addr[0] = src->u_addr.ip6.addr[0];
        dest->u_addr.ip6.addr[1] = src->u_addr.ip6.addr[1];
        dest->u_addr.ip6.addr[2] = src->u_addr.ip6.addr[2];
        dest->u_addr.ip6.addr[3] = src->u_addr.ip6.addr[3];
        dest->u_addr.ip6.zone = src->u_addr.ip6.zone;
    } else {
        dest->u_addr.ip4.addr = src->u_addr.ip4.addr;
        dest->u_addr.ip6.addr[1] = 0;
        dest->u_addr.ip6.addr[2] = 0;
        dest->u_addr.ip6.addr[3] = 0;
        dest->u_addr.ip6.zone    = 0;
    }
}

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_init
 * @}
 */

#endif /* __XF_NETIF_IP_ADDR_TYPES_H__ */
