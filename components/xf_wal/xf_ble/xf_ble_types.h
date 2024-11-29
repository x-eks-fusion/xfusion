/**
 * @file xf_ble_types.h
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-08-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_BLE_TYPES_H__
#define __XF_BLE_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_BLE_IS_ENABLE 1

/* ==================== [Typedefs] ========================================== */

/**
 * @brief BLE 地址长度
 */
#define XF_BLE_ADDR_LEN     6

/**
 * @brief BLE 地址打印格式
 */
#define XF_BLE_ADDR_PRINT_FMT  "%02X:%02X:%02X:%02X:%02X:%02X"

/**
 * @brief BLE 地址展开为参数
 *
 * @note 一般用于配合 'XF_BLE_ADDR_PRINT_FMT' 进行地址打印输出
 */
#define XF_BLE_ADDR_EXPAND_TO_ARG(addr)  (addr)[0],(addr)[1],(addr)[2],(addr)[3],(addr)[4],(addr)[5]

/**
 * @brief 蓝牙地址类型，完全遵循蓝牙标准进行定义
 *
 * @details 地址类型如下：
 * @code
 *  1、公共设备地址（类似USB VID，需申请）
 *  2、随机设备地址：设备启动后随机生成
 *      2.A、静态设备地址：在启动后地址不变，下次复位后地址可能会变（非强制要求）
 *      2.B、私有设备地址：地址会更新
 *          2.B.1、不可解析私有地址：地址定时更新
 *          2.B.2、可解析私有地址：地址加密生成
 * @endcode
 * @ref 详参蓝牙核心文档 《Core_v5.4》>| Vol 6, Part B >| 1.3 DEVICE ADDRESS
 *  在线文档: https://www.bluetooth.com/specifications/specs/core54-html/
 *  离线文档: https://www.bluetooth.com/specifications/specs/core-specification-amended-5-4/
 */
typedef enum {
    XF_BT_ADDR_TYPE_PUBLIC_DEV  = 0x00,     /*!< 公有地址 */
    XF_BT_ADDR_TYPE_RANDOM_DEV  = 0x01,     /*!< 随机地址 */
    XF_BT_ADDR_TYPE_RPA_PUBLIC  = 0x02,     /*!< 不可解析私有地址：地址定时更新 */
    XF_BT_ADDR_TYPE_RPA_RANDOM  = 0x03,     /*!< 可解析私有地址：地址加密生成 */
} xf_ble_addr_type_t;

/**
 * @brief BLE 地址信息
 */
typedef struct {
    uint8_t addr[XF_BLE_ADDR_LEN];     /*!< BLE 地址值 */
    xf_ble_addr_type_t type;           /*!< BLE 地址类型，见 @ref xf_ble_addr_type_t */
} xf_ble_addr_t;

/**
 * @brief BLE UUID 类型 (长度类型)
 */
typedef enum {
    XF_BLE_UUID_TYPE_16 = 2,        /*!< 长度 16-bit */
    XF_BLE_UUID_TYPE_32 = 4,        /*!< 长度 32-bit */
    XF_BLE_UUID_TYPE_128 = 16,      /*!< 长度 128-bit */
} xf_ble_uuid_type_t;

/**
 * @brief BLE UUID 信息 (多种长度类型通用)
 */
typedef struct {
    union {
        xf_ble_uuid_type_t len_type;                 /*!< UUID 长度类型，见 @ref xf_ble_uuid_type_t */
        uint8_t _invalid;                           /*!< 用于固定 UUID 长度类型变量的大小为 8-bit */
    };
    union {
        uint16_t    uuid16;                         /*!< 16-bit UUID */
        uint32_t    uuid32;                         /*!< 32-bit UUID */
        uint8_t     uuid128[XF_BLE_UUID_TYPE_128];  /*!< 128-bit UUID */
    };
} __attribute__((packed)) xf_ble_uuid_info_t;

/**
 * @brief BLE 16-bit UUID 信息
 */
typedef struct {
    union {
        xf_ble_uuid_type_t type;                     /*!< UUID 长度类型，见 @ref xf_ble_uuid_type_t */
        uint8_t _invalid;                           /*!< 用于固定 UUID 长度类型变量的大小为 8-bit */
    };
    uint16_t value;                                 /*!< 16-bit UUID */
} __attribute__((packed)) xf_ble_uuid16_info_t;

/**
 * @brief BLE 32-bit UUID 信息
 */
typedef struct {
    union {
        xf_ble_uuid_type_t type;                     /*!< UUID 长度类型，见 @ref xf_ble_uuid_type_t */
        uint8_t _invalid;                           /*!< 用于固定 UUID 长度类型变量的大小为 8-bit */
    };
    uint32_t value;                                 /*!< 32-bit UUID */
} __attribute__((packed)) xf_ble_uuid32_info_t;

/**
 * @brief BLE 128-bit UUID 信息
 */
typedef struct {
    union {
        xf_ble_uuid_type_t type;                     /*!< UUID 长度类型，见 @ref xf_ble_uuid_type_t */
        uint8_t _invalid;                           /*!< 用于固定 UUID 长度类型变量的大小为 8-bit */
    };
    uint8_t value[XF_BLE_UUID_TYPE_128];            /*!< 128-bit UUID */
} __attribute__((packed)) xf_ble_uuid128_info_t;


#define _XF_BLE_UUID16_INIT(uuid16)         \
    {                                       \
        .type = XF_BLE_UUID_TYPE_16,        \
        .value = (uuid16),                  \
    }

#define _XF_BLE_UUID32_INIT(uuid32)         \
    {                                       \
        .type = XF_BLE_UUID_TYPE_32,        \
        .value = (uuid32),                  \
    }

#define _XF_BLE_UUID128_INIT(uuid128 ...)   \
    {                                       \
        .type = XF_BLE_UUID_TYPE_128,       \
        .value = { uuid128 },               \
    }

/**
 * @brief BLE 声明一个 16-bit UUID
 * @note 通常用于填充服务结构时填入常量 UUID
 */
#define XF_BLE_DECLARE_UUID16(uuid16) \
    ((xf_ble_uuid_info_t *) (&(xf_ble_uuid16_info_t) _XF_BLE_UUID16_INIT(uuid16)))

/**
 * @brief BLE 声明一个 32-bit UUID
 * @note 通常用于填充服务结构时填入常量 UUID
 */
#define XF_BLE_DECLARE_UUID32(uuid32) \
    ((xf_ble_uuid_info_t *) (&(xf_ble_uuid32_info_t) _XF_BLE_UUID32_INIT(uuid32)))

/**
 * @brief BLE 声明一个 128-bit UUID
 * @note 通常用于填充服务结构时填入常量 UUID
 */
#define XF_BLE_DECLARE_UUID128(uuid128...) \
    ((xf_ble_uuid_info_t *) (&(xf_ble_uuid128_info_t) _XF_BLE_UUID128_INIT(uuid128)))

/**
 * @brief BLE 无效属性句柄
 * @note 通常用于对接服务结构添加时，判断服务、特征、特征描述符等属性的句柄是否有效
 */
#define XF_BLE_INVALID_ATTR_HANDLE  (0x0000)

/**
 * @brief BLE 属性句柄
 */
typedef uint16_t xf_ble_attr_handle_t;

/**
 * @brief BLE 中类型可变的数据
 *
 * @note 基于 uintptr 类型，可变数据的最大大小等于 uintptr 类型的大小
 */
typedef union _xf_ble_var_uintptr_t {
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
} xf_ble_var_uintptr_t;

/**
 * @brief BLE GAP 事件
 */
typedef enum {
    XF_BLE_GAP_EVT_CONNECT,                 /*!< 连接事件 */
    XF_BLE_GAP_EVT_DISCONNECT,              /*!< 断连事件 */
    XF_BLE_GAP_EVT_PAIR_END,                /*!< 配对结束事件 */
    XF_BLE_GAP_EVT_CONN_PARAMS_UPDATE,      /*!< 连接参数更新事件 */
    XF_BLE_GAP_EVT_SCAN_PARAM_SET,          /*!< 扫描参数设置事件 */
    XF_BLE_GAP_EVT_SCAN_RESULT,             /*!< 收到扫描结果事件 */
    XF_BLE_GAP_ADV_STARTED,                 /*!< 广播开启事件 */
    XF_BLE_GAP_ADV_STOPPED,                 /*!< 广播停止事件 */
    _XF_BLE_GAP_EVT_ENUM_END,                /*!< GAP 事件枚举结束值 */
} xf_ble_gap_event_t;

/* FIXME gap 与 gatt 事件以及事件回调分离 */

/**
 * @brief BLE GATTS 事件
 */
typedef enum {
    XF_BLE_GATTS_EVT_ADD_SERVICE = _XF_BLE_GAP_EVT_ENUM_END,    /*!< 服务添加事件 */
    XF_BLE_GATTS_EVT_DEL_SERVICE,                               /*!< 服务移除事件 */
    XF_BLE_GATTS_EVT_ADD_CHARA,                                 /*!< 特征添加事件 */
    XF_BLE_GATTS_EVT_ADD_DESC,                                  /*!< 特征描述符事件 */
    XF_BLE_GATTS_EVT_SERVICE_START,                             /*!< 服务开启事件 */
    XF_BLE_GATTS_EVT_SERVICE_STOP,                              /*!< 服务停止事件 */
    XF_BLE_GATTS_EVT_REQ_READ,                                  /*!< 接收到读请求事件 */
    XF_BLE_GATTS_EVT_REQ_WRITE,                                 /*!< 接收到写请求事件 */
    XF_BLE_GATTS_EVT_MTU_CHANGED,                               /*!< MTU 变更事件 */
} xf_ble_gatts_event_t;

/**
 * @brief BLE GATTC 事件
 */
typedef enum {
    XF_BLE_GATTC_EVT_DISCOVER_SERVICE_COMPLETE = _XF_BLE_GAP_EVT_ENUM_END,  /*!< 服务结构搜寻完毕事件 */
    XF_BLE_GATTC_EVT_MTU_CHANGED,                                           /*!< MTU 变更事件 */
    XF_BLE_GATTC_EVT_WRITE_COMPLETE,                                        /*!< 写完成事件 */
    XF_BLE_GATTC_EVT_READ_COMPLETE,                                         /*!< 读完成事件 */
    XF_BLE_GATTC_EVT_RECV_NOTIFICATION_OR_INDICATION,                       /*!< 收到通知或指示事件 */
} xf_ble_gattc_event_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_BLE_TYPES_H__ */
