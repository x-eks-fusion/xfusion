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

/**
 * @cond (XFAPI_USER || XFAPI_PORT || XFAPI_INTERNAL)
 * @ingroup group_xf_wal
 * @defgroup group_xf_wal_sle xf_sle
 * @brief xf_sle 是对星闪接口的封装。
 * @endcond
 */

#ifndef __XF_SLE_TYPES_H__
#define __XF_SLE_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#define XF_SLE_IS_ENABLE 1

#if XF_SLE_IS_ENABLE  || defined(__DOXYGEN__)

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_wal_sle
 * @endcond
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#if (!defined(CONFIG_XF_SLE_DEBUG_ENABLE)) || (CONFIG_XF_SLE_DEBUG_ENABLE == 0)
#define XF_SLE_DEBUG_ENABLE    0
#else
#define XF_SLE_DEBUG_ENABLE    1
#endif

/* ==================== [Typedefs] ========================================== */

/**
 * @brief SLE 地址长度
 */
#define XF_SLE_ADDR_LEN        6

/**
 * @brief SLE 地址打印格式
 */
#define XF_SLE_ADDR_PRINT_FMT  "%02X:%02X:%02X:%02X:%02X:%02X"

/**
 * @brief SLE 地址展开为参数
 *
 * @note 一般用于配合 'XF_SLE_ADDR_PRINT_FMT' 进行地址打印输出
 */
#define XF_SLE_ADDR_EXPAND_TO_ARG(addr)  (addr)[0],(addr)[1],(addr)[2],(addr)[3],(addr)[4],(addr)[5]

/**
 * @brief  SLE 地址类型
 */
typedef enum {
    XF_SLE_ADDRESS_TYPE_PUBLIC = 0,     /*!< 公有地址 */
    XF_SLE_ADDRESS_TYPE_RANDOM = 6,     /*!< 随机地址 */
} xf_sle_addr_type_t;

/**
 * @brief  SLE 地址信息
 */
typedef struct {
    xf_sle_addr_type_t type;            /*!< SLE 地址类型 */
    uint8_t addr[XF_SLE_ADDR_LEN];      /*!< SLE 地址值 */
} xf_sle_addr_t;

/**
 * @brief SLE UUID 类型 (长度类型)
 */
typedef enum {
    XF_SLE_UUID_TYPE_16 = 2,        /*!< 长度 16-bit */
    XF_SLE_UUID_TYPE_128 = 16,      /*!< 长度 128-bit */
} xf_sle_uuid_type_t;

/**
 * @brief SLE UUID 信息 (多种长度类型通用)
 */
typedef struct __packed _xf_sle_uuid_info_t {
    union {
        xf_sle_uuid_type_t type;                    /*!< UUID 长度类型，见 @ref xf_sle_uuid_type_t */
        uint8_t _invalid;                           /*!< 用于固定 UUID 长度类型变量的大小为 8-bit */
    };
    union {
        uint16_t    uuid16;                         /*!< 16-bit UUID */
        uint8_t     uuid128[XF_SLE_UUID_TYPE_128];  /*!< 128-bit UUID */
    };
} xf_sle_uuid_info_t;

/**
 * @brief SLE 16-bit UUID 信息
 */
typedef struct __packed _xf_sle_uuid16_info_t {
    union {
        xf_sle_uuid_type_t type;                    /*!< UUID 长度类型，见 @ref xf_sle_uuid_type_t */
        uint8_t _invalid;                           /*!< 用于固定 UUID 长度类型变量的大小为 8-bit */
    };
    uint16_t value;                                 /*!< 16-bit UUID */
} xf_sle_uuid16_info_t;

/**
 * @brief SLE 128-bit UUID 信息
 */
typedef struct __packed _xf_sle_uuid128_info_t {
    union {
        xf_sle_uuid_type_t type;            /*!< UUID 长度类型，见 @ref xf_sle_uuid_type_t */
        uint8_t _invalid;                   /*!< 用于固定 UUID 长度类型变量的大小为 8-bit */
    };
    uint8_t value[XF_SLE_UUID_TYPE_128];    /*!< 32-bit UUID */
} xf_sle_uuid128_info_t;

#define _XF_SLE_UUID16_INIT(uuid16)         \
    {                                       \
        .type = XF_SLE_UUID_TYPE_16,        \
        .value = (uuid16),                  \
    }

#define _XF_SLE_UUID128_INIT(uuid128 ...)   \
    {                                       \
        .type = XF_SLE_UUID_TYPE_128,       \
        .value = { uuid128 },               \
    }

/**
 * @brief SLE 声明一个 16-bit UUID
 * @note 通常用于填充服务结构时填入常量 UUID
 */
#define XF_SLE_DECLARE_UUID16(uuid16) \
    ((xf_sle_uuid_info_t *) (&(xf_sle_uuid16_info_t) _XF_SLE_UUID16_INIT(uuid16)))

/**
 * @brief SLE 声明一个 128-bit UUID
 * @note 通常用于填充服务结构时填入常量 UUID
 */
#define XF_SLE_DECLARE_UUID128(uuid128...) \
    ((xf_sle_uuid_info_t *) (&(xf_sle_uuid128_info_t) _XF_SLE_UUID128_INIT(uuid128)))

/**
 * @brief SLE 无效属性句柄
 * @note 通常用于对接服务结构添加时，判断服务、特征、特征描述符等属性的句柄是否有效
 */
#define XF_SLE_INVALID_ATTR_HANDLE  (0x0000)

/**
 * @brief SLE 属性句柄
 */
typedef uint16_t xf_sle_attr_handle_t;

/**
 * @brief SLE 中类型可变的数据
 *
 * @note 基于 uintptr 类型，可变数据的最大大小等于 uintptr 类型的大小
 */
typedef union _xf_sle_var_uintptr_t {
    uintptr_t   _untyped;
    bool        val_bool;
    uint8_t     val_u8;
    uint16_t    val_u16;
    uint32_t    val_u32;

    uint8_t     *ptr_u8;
    uint16_t    *ptr_u16;
    uint32_t    *ptr_u32;

    uint8_t     array_u8[sizeof(uintptr_t)];
    uint16_t    array_u16[sizeof(uintptr_t) / sizeof(uint16_t)];
    uint32_t    array_u32[sizeof(uintptr_t) / sizeof(uint32_t)];
} xf_sle_var_uintptr_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_wal_sle
 * @}
 */

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_TYPES_H__ */
