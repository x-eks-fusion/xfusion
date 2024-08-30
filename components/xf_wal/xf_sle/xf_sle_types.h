/**
 * @file xf_sle_types.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_SLE_TYPES_H__
#define __XF_SLE_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_ble_gap_types.h"

#define XF_SLE_IS_ENABLE 1

#if XF_SLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#if (!defined(CONFIG_XF_SLE_DEBUG_ENABLE)) || (CONFIG_XF_SLE_DEBUG_ENABLE == 0)
#define XF_SLE_DEBUG_ENABLE    0
#else
#define XF_SLE_DEBUG_ENABLE    1
#endif

#define XF_SLE_ADDR_LEN        6

/* ==================== [Typedefs] ========================================== */

typedef enum {
    XF_SLE_ADDRESS_TYPE_PUBLIC = 0,      /*!< @if Eng public address
                                           @else   公有地址 @endif */
    XF_SLE_ADDRESS_TYPE_RANDOM = 6,      /*!< @if Eng random address
                                           @else   随机地址 @endif */
} xf_sle_addr_type_t;


typedef struct {
    xf_sle_addr_type_t type;            /*!< SLE设备地址类型 */
    uint8_t addr[XF_SLE_ADDR_LEN];     /*!< SLE设备地址 */
} xf_sle_addr_t;

#define XF_SLE_ADDR_PRINT_FMT  "%02X:%02X:%02X:%02X:%02X:%02X"
#define XF_SLE_ADDR_EXPAND_TO_ARG(addr)  (addr)[0],(addr)[1],(addr)[2],(addr)[3],(addr)[4],(addr)[5]

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_TYPES_H__ */
