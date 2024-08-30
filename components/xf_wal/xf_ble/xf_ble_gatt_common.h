/**
 * @file xf_ble_gatt_common.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_BLE_GATT_COMMON_H__
#define __XF_BLE_GATT_COMMON_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#include "xf_ble_types.h"
#include "xf_ble_gap_types.h"

#if XF_BLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/** Type of UUID */
typedef enum {
    /** 16-bit UUID (BT SIG assigned) */
    XF_BLE_UUID_TYPE_16 = 2,

    /** 32-bit UUID (BT SIG assigned) */
    XF_BLE_UUID_TYPE_32 = 4,

    /** 128-bit UUID */
    XF_BLE_UUID_TYPE_128 = 16,
}xf_bt_uuid_type_t;

/**
 * @brief  Struct of UUID.
 * @brief  定义UUID结构。
 */
typedef struct {
    union {
        xf_bt_uuid_type_t len_type;     /*!< UUID length, 16bit, 32bit or 128bit */
        uint8_t _invalid;
    };
    union {
        uint16_t    uuid16;                 /*!< 16bit UUID */
        uint32_t    uuid32;                 /*!< 32bit UUID */
        uint8_t     uuid128[XF_BLE_UUID_TYPE_128]; /*!< 128bit UUID */
    };									/*!< UUID */
} __attribute__((packed)) xf_bt_uuid_info_t;

/** 16-bit UUID */
typedef struct {
    union {
        xf_bt_uuid_type_t type;     /*!< UUID length, 16bit, 32bit or 128bit */
        uint8_t _invalid;
    };
    uint16_t value;
} __attribute__((packed)) xf_bt_uuid16_info_t;

/** 32-bit UUID */
typedef struct {
    union {
        xf_bt_uuid_type_t type;     /*!< UUID length, 16bit, 32bit or 128bit */
        uint8_t _invalid;
    };
    uint32_t value;
} __attribute__((packed)) xf_bt_uuid32_info_t;

/** 128-bit UUID */
typedef struct {
    union {
        xf_bt_uuid_type_t type;     /*!< UUID length, 16bit, 32bit or 128bit */
        uint8_t _invalid;
    };
    uint8_t value[XF_BLE_UUID_TYPE_128];
} __attribute__((packed)) xf_bt_uuid128_info_t;


#define _XF_BLE_UUID16_INIT(uuid16)         \
    {                                   \
        .type = XF_BLE_UUID_TYPE_16,    \
        .value = (uuid16),              \
    }

#define _XF_BLE_UUID32_INIT(uuid32)         \
    {                                   \
        .type = XF_BLE_UUID_TYPE_32,    \
        .value = (uuid32),              \
    }

#define _XF_BLE_UUID128_INIT(uuid128 ...)   \
    {                                   \
        .type = XF_BLE_UUID_TYPE_128,   \
        .value = { uuid128 },           \
    }

#define XF_BLE_DECLARE_UUID16(uuid16) \
    ((xf_bt_uuid_info_t *) (&(xf_bt_uuid16_info_t) _XF_BLE_UUID16_INIT(uuid16)))

#define XF_BLE_DECLARE_UUID32(uuid32) \
    ((xf_bt_uuid_info_t *) (&(xf_bt_uuid32_info_t) _XF_BLE_UUID32_INIT(uuid32)))

#define XF_BLE_DECLARE_UUID128(uuid128...) \
    ((xf_bt_uuid_info_t *) (&(xf_bt_uuid128_info_t) _XF_BLE_UUID128_INIT(uuid128)))


#define XF_BLE_INVALID_ATTR_HANDLE  (0x0000)
typedef uint16_t xf_bt_attr_handle_t;

/**
 * @brief  定义了当前 Attribute 的操作权限。
 */
typedef enum {
    XF_BLE_GATT_ATTR_PERMISSION_READ = 0x01,               /*!< @if Eng readable
                                                                    @else   可读 @endif */
    XF_BLE_GATT_ATTR_PERMISSION_WRITE = 0x02,               /*!< @if Eng writable
                                                                    @else   可写 @endif */
    XF_BLE_GATT_ATTR_PERMISSION_ENCRYPTION_NEED = 0x04,    /*!< @if Eng need encryption
                                                                    @else   需要加密 @endif */
    XF_BLE_GATT_ATTR_PERMISSION_AUTHENTICATION_NEED = 0x08, /*!< @if Eng need authentication
                                                                    @else   需要认证 @endif */
    XF_BLE_GATT_ATTR_PERMISSION_AUTHORIZATION_NEED = 0x10,  /*!< @if Eng need authorization
                                                                    @else   需要授权 @endif */
    XF_BLE_GATT_ATTR_PERMISSION_MITM_NEED = 0x20,           /*!< @if Eng need MITM protection
                                                                    @else   需要MITM保护 @endif */
}xf_ble_gatt_attr_permission_t;


/**
 * @brief  gatt 特征特性（property）
 * 注意：此处
 *  并不是严格按照官方定义的 一字节 8 个特征的基本特性类型
 *  而是 多字节 8 个基本特性类型 + 拓展特性类型的类型定义
 *      拓展类型从 基于官方 1 字节基本特性类型的最高位( 0x80 )
 *      开始替换续接为拓展特性类型。
 *  
 *  拓展类型名字都带上 EXT （暂定），以便开发者查阅、对比官方描述
 */
typedef enum {
    XF_BLE_GATT_CHARA_PROPERTY_BROADCAST    = 0x01, /*!< @if Eng Broadcast characteristic value
                                                        @else   广播特征值 @endif */
    XF_BLE_GATT_CHARA_PROPERTY_READ         = 0x02, /*!< @if Eng Read characteristic value
                                                        @else   读特征值 @endif */
    XF_BLE_GATT_CHARA_PROPERTY_WRITE_NO_RSP = 0x04, /*!< @if Eng Write characteristic value with no response
                                                        @else   写特征值并且不需要响应 @endif */
    XF_BLE_GATT_CHARA_PROPERTY_WRITE        = 0x08, /*!< @if Eng Write characteristic value
                                                        @else   写特征值 @endif */
    XF_BLE_GATT_CHARA_PROPERTY_NOTIFY       = 0x10, /*!< @if Eng Notify client characteristic value
                                                        @else   通知特征值 @endif */
    XF_BLE_GATT_CHARA_PROPERTY_INDICATE     = 0x20, /*!< @if Eng Indicate characteristic value
                                                        @else   指示特征值 @endif */
    XF_BLE_GATT_CHARA_PROPERTY_SIGNED_WRITE = 0x40, /*!< @if Eng Signed write characteristic value
                                                        @else   签名写特征值 @endif */

    /* 以下为 特征的拓展特性类型  */
    XF_BLE_GATT_CHARA_PROPERTY_EXT_RELIABLE_WRITE   = 0x0080, 
    XF_BLE_GATT_CHARA_PROPERTY_EXT_AUX_WRITE        = 0x0100,
    XF_BLE_GATT_CHARA_PROPERTY_EXT_READ_ENC         = 0x0200,
    XF_BLE_GATT_CHARA_PROPERTY_EXT_READ_AUTHEN      = 0x0400,
    XF_BLE_GATT_CHARA_PROPERTY_EXT_READ_AUTHOR      = 0x0800,
    XF_BLE_GATT_CHARA_PROPERTY_EXT_WRITE_ENC        = 0x1000,
    XF_BLE_GATT_CHARA_PROPERTY_EXT_WRITE_AUTHEN     = 0x2000,
    XF_BLE_GATT_CHARA_PROPERTY_EXT_WRITE_AUTHOR     = 0x4000,

}xf_ble_gatt_chara_property_t;


// 属性由四部分组成：属性句柄、属性类型、属性值和属性权限
/**
 * @brief 
 * |    2 octets | 2 or 16 octets| variable length  | implementation specific   |
 * | attr handle |  attr type    | attr value       | attr permissions          |  
 */
typedef struct _xf_ble_gatt_attr_t{
    uint16_t handle;    // 属性句柄
    xf_bt_uuid_info_t type;  // 属性类型
    /** 
     * @brief 
     *  用于存放数据。
     * 如果属性类型是:
     *  GATT特定类型（如服务项类型或特征值声明类型）:
     *      那么属性值就是UUID（各自类型的UUID，如服务UUID、特征UUID）等信息。
     *  如果是普通的特征值，则属性值是用户的数据。
     */
    uint8_t *value; 
    uint16_t permissions;
    // uint16_t max_length;               /*!< Maximum length of the element*/
    // uint16_t length;                   /*!< Current length of the element*/
}xf_ble_gatt_attr_t;


typedef enum {
    XF_BLE_GATT_CHARA_DESC_TYPE_CHARA_EXT_PROPERTIES,       // 0x2900, CEP:Characteristic Extended Properties
    XF_BLE_GATT_CHARA_DESC_TYPE_CHARA_USER_DESC,            // 0x2901, CUD:Characteristic User Description
    XF_BLE_GATT_CHARA_DESC_TYPE_CLIENT_CHARA_CFG,           // 0x2902, CCC:Client Characteristic Configuration
    XF_BLE_GATT_CHARA_DESC_TYPE_SERVER_CHARA_CFG,           // 0x2903, SCC:Server Characteristic Configuration
    XF_BLE_GATT_CHARA_DESC_TYPE_CHARA_PRESENTATION_FORMAT,  // 0x2904, CPF:Characteristic Presentation Format
    XF_BLE_GATT_CHARA_DESC_TYPE_CHARA_AGGREGATE_FORMAT,     // 0x2903, CAF:Characteristic Aggregate Format
}xf_ble_gatt_chara_desc_type_t;

/**
 * @brief  服务类型
 */
typedef enum {
    XF_BLE_GATT_SERVICE_TYPE_PRIMARY,
    XF_BLE_GATT_SERVICE_TYPE_SECONDARY,
}xf_ble_gatt_service_type_t;

#define XF_BLE_GATT_MAX_ATTR_LEN    (600)
#define XF_BLE_GATT_INVALID_HANDLE    (0x00)

/**
 * @if Eng
 * @brief  Enum of gatt operation error code.
 * @else
 * @brief  定义gatt操作错误码。
 * @endif
 */
typedef enum {
    XF_BLE_GATT_ERR_SUCCESS                             = 0x00, /*!< @if Eng operation error code of success
                                                                    @else   执行成功错误码 @endif */
    XF_BLE_GATT_ERR_INVALID_HANDLE                      = 0x01, /*!< @if Eng operation error code of invalid handle
                                                                    @else   无效handle错误码 @endif */
    XF_BLE_GATT_ERR_READ_NOT_PERMITTED                  = 0x02, /*!< @if Eng operation error code of read not permitted
                                                                    @else   不可读错误码 @endif */
    XF_BLE_GATT_ERR_WRITE_NOT_PERMITTED                 = 0x03, /*!< @if Eng operation error code of write not permitted
                                                                    @else   不可写错误码 @endif */
    XF_BLE_GATT_ERR_INVALID_PDU                         = 0x04, /*!< @if Eng operation error code of invalid PDU
                                                                    @else   无效PDU错误码 @endif */
    XF_BLE_GATT_ERR_INSUFFICIENT_AUTHENTICATION         = 0x05, /*!< @if Eng operation error code of insufficient authorization
                                                                    @else   未认证的读写错误码 @endif */
    XF_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED               = 0x06, /*!< @if Eng operation error code of not support the request
                                                                    by the client
                                                                    @else 不支持client发送的请求 @endif */
    XF_BLE_GATT_ERR_INVALID_OFFSET                      = 0x07, /*!< @if Eng operation error code of invalid offset
                                                                    @else   无效偏移的读写错误码 @endif */
    XF_BLE_GATT_ERR_INSUFFICIENT_AUTHORIZATION          = 0x08, /*!< @if Eng operation error code of insufficient
                                                                    authentication
                                                                    @else   未授权错误码 @endif */
    XF_BLE_GATT_ERR_PREPARE_QUEUE_FULL                  = 0x09, /*!< @if Eng operation error code of prepare queue full
                                                                    @else   排队的prepare writes太多，队列已满错误码 @endif */
    XF_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND                 = 0x0A, /*!< @if Eng operation error code of prepare queue full
                                                                    @else   排队的prepare writes太多，队列已满错误码 @endif */
    XF_BLE_GATT_ERR_ATTRIBUTE_NOT_LONG                  = 0x0B, /*!< @if Eng operation error code of can not use
                                                                    ATT_READ_BLOB_REQ PDU by not long attribute
                                                                    @else   该attribute不是long attribute, 不能使用
                                                                    ATT_READ_BLOB_REQ PDU @endif */
    XF_BLE_GATT_ERR_INSUFFICIENT_ENCRYPTION_KEY_SIZE    = 0x0C, /*!< @if Eng operation error code of the encryption key size is
                                                                    insufficient
                                                                    @else 用于加密此连接的加密密钥大小不足@endif */
    XF_BLE_GATT_ERR_INVALID_ATTRIBUTE_VALUE_LENGTH      = 0x0D, /*!< @if Eng operation error code of the encryption key size is
                                                                    insufficient
                                                                    @else   用于加密此连接的加密密钥大小不足@endif */
    XF_BLE_GATT_ERR_UNLIKELY_ERROR                      = 0x0E, /*!< @if Eng operation error code unlikely
                                                                    @else   不可能的错误码@endif */
    XF_BLE_GATT_ERR_INSUFFICIENT_ENCRYPTION             = 0x0F, /*!< @if Eng operation error code of insufficient encryption
                                                                    @else   加密不足错误码@endif */
    XF_BLE_GATT_ERR_UNSUPPORTED_GROUP_TYPE              = 0x10, /*!< @if Eng operation error code of unsupport grouping
                                                                    attribute type
                                                                    @else   不支持的 grouping attribute错误码@endif */
    XF_BLE_GATT_ERR_INSUFFICIENT_RESOURCES              = 0x11, /*!< @if Eng operation error code of insufficient resource
                                                                    @else   资源不足错误码@endif */
    XF_BLE_GATT_ERR_DATABASE_OUT_OF_SYNC                = 0x12, /*!< @if Eng operation error code of database out of sync
                                                                    @else   服务端与客户端未同步数据库错误码@endif */
    XF_BLE_GATT_ERR_VALUE_NOT_ALLOWED                   = 0x13, /*!< @if Eng operation error code of not allowed value
                                                                    @else   非法的参数值错误码@endif */
    
    /* 0x80 – 0x9F ：由更高层规范定义的应用程序错误代码。 */
    XF_BLE_GATT_ERR_APP_ERROR_START                     = 0x80,
    XF_BLE_GATT_ERR_APP_ERROR_END                       = 0x9F,   

    /* 0xE0 – 0xFF ：在[1]中定义的通用配置文件和服务错误代码 
        [1] Core Specification Supplement, Part B, Common Profile and Service Error Codes
    */    
    XF_BLE_GATT_ERR_COMMON_PROFILE_AND_SERVICE_START    = 0xE0,   
    XF_BLE_GATT_ERR_COMMON_PROFILE_AND_SERVICE_END      = 0xFF,                                           
}xf_ble_gatt_err_t;


/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GATT_COMMON_H__ */
