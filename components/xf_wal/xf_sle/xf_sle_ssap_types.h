/**
 * @file xf_sle_ssap_types.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_SLE_SSAP_TYPES_H__
#define __XF_SLE_SSAP_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sle_types.h"

#if XF_SLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/** Type of UUID */
typedef enum {
    /** 16-bit UUID */
    XF_SLE_UUID_TYPE_16 = 2,

    /** 32-bit UUID  */
    // XF_SLE_UUID_TYPE_32 = 4,     // 星闪标准手册貌似没写 32 bit

    /** 128-bit UUID */
    XF_SLE_UUID_TYPE_128 = 16,
} xf_sle_uuid_type_t;

/**
 * @brief  Struct of UUID.
 * @brief  定义UUID结构。
 */
typedef struct {
    union {
        xf_sle_uuid_type_t len_type;     /*!< UUID length, 16bit, 32bit or 128bit */
        uint8_t _invalid;
    };
    union {
        uint16_t    uuid16;                 /*!< 16bit UUID */
        // uint32_t    uuid32;                 /*!< 32bit UUID */
        uint8_t     uuid128[XF_SLE_UUID_TYPE_128]; /*!< 128bit UUID */
    };                                  /*!< UUID */
} __attribute__((packed)) xf_sle_uuid_info_t;

/** 16-bit UUID */
typedef struct {
    union {
        xf_sle_uuid_type_t type;     /*!< UUID length, 16bit, 32bit or 128bit */
        uint8_t _invalid;
    };
    uint16_t value;
} __attribute__((packed)) xf_sle_uuid16_info_t;

// /** 32-bit UUID */
// typedef struct {
//     union {
//         xf_sle_uuid_type_t type;     /*!< UUID length, 16bit, 32bit or 128bit */
//         uint8_t _invalid;
//     };
//     uint32_t value;
// } __attribute__((packed)) xf_sle_uuid32_info_t;

/** 128-bit UUID */
typedef struct {
    union {
        xf_sle_uuid_type_t type;     /*!< UUID length, 16bit, 32bit or 128bit */
        uint8_t _invalid;
    };
    uint8_t value[XF_SLE_UUID_TYPE_128];
} __attribute__((packed)) xf_sle_uuid128_info_t;


#define _XF_SLE_UUID16_INIT(uuid16)         \
    {                                   \
        .type = XF_SLE_UUID_TYPE_16,    \
        .value = (uuid16),              \
    }

#define _XF_SLE_UUID128_INIT(uuid128 ...)   \
    {                                   \
        .type = XF_SLE_UUID_TYPE_128,   \
        .value = { uuid128 },           \
    }

#define XF_SLE_DECLARE_UUID16(uuid16) \
    ((xf_sle_uuid_info_t *) (&(xf_sle_uuid16_info_t) _XF_SLE_UUID16_INIT(uuid16)))

#define XF_SLE_DECLARE_UUID128(uuid128...) \
    ((xf_sle_uuid_info_t *) (&(xf_sle_uuid128_info_t) _XF_SLE_UUID128_INIT(uuid128)))


#define XF_SLE_INVALID_ATTR_HANDLE  (0x0000)
typedef uint16_t xf_sle_attr_handle_t;


/**
 * @brief  服务类型
 */
typedef enum {
    XF_SLE_SSAP_SERVICE_TYPE_PRIMARY,
    XF_SLE_SSAP_SERVICE_TYPE_SECONDARY,
} xf_sle_ssap_service_type_t;

/**
 * @if Eng
 * @brief  SSAP attribute permission.
 * @else
 * @brief  SSAP attribute操作权限。
 * @endif
 */
typedef enum {
    XF_SLE_SSAP_PERMISSION_READ                = 0x01, /*!< @if Eng readable
                                                     @else   可读 @endif */
    XF_SLE_SSAP_PERMISSION_WRITE               = 0x02, /*!< @if Eng writable
                                                     @else   可写 @endif */
    XF_SLE_SSAP_PERMISSION_ENCRYPTION_NEED     = 0x04, /*!< @if Eng need encryption
                                                     @else   需要加密 @endif */
    XF_SLE_SSAP_PERMISSION_AUTHENTICATION_NEED = 0x08, /*!< @if Eng need authentication
                                                     @else   需要认证 @endif */
    XF_SLE_SSAP_PERMISSION_AUTHORIZATION_NEED  = 0x10, /*!< @if Eng need authorization
                                                     @else   需要授权 @endif */
} xf_sle_ssap_permission_t;


/**
 * @if Eng
 * @brief  SSAP operation indication.
 * @else
 * @brief  SSAP 操作指示。
 * @endif
 */
typedef enum {
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_READ = 0x01,              /*!< @if Eng data values can be read
                                                               @else   数据值可被读取 @endif */
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE_NO_RSP = 0x02,      /*!< @if Eng data values can be written without feedback.
                                                               @else   数据值可被写入，写入后无反馈 @endif */
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE = 0x04,             /*!< @if Eng data values can be written. After being written,
                                                                       a feedback is generated to the client.
                                                               @else   数据值可被写入，写入后产生反馈给客户端 @endif */
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_NOTIFY = 0x08,            /*!< @if Eng data value send to client by notification.
                                                               @else   数据值通过通知方式传递给客户端 @endif */
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_INDICATE = 0x10,          /*!< @if Eng data value send to client by indication.
                                                               @else   数据值通过指示方式传递给客户端 @endif */
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_BROADCAST = 0x20,         /*!< @if Eng data value can be carried in broadcast
                                                               @else   数据值可携带在广播中 @endif */
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_DESCRITOR_WRITE = 0x100,  /*!< @if Eng data value description descriptor can be
                                                                       written
                                                               @else   数据值说明描述符可被写入 @endif */
    XF_SLE_SSAP_OPERATE_INDICATION_MAX,                          /*!< @if Eng maximum of operation indication
                                                               @else   操作指示最大值 @endif */
} xf_sle_ssap_operate_indication_t;

typedef enum {
    XF_SLE_SSAP_DESC_TYPE_USER_DESCRIPTION     = 0x01, /*!< @if Eng user description descriptor
                                                      @else   属性说明描述符 @endif */
    XF_SLE_SSAP_DESC_TYPE_CLIENT_CONFIGURATION = 0x02, /*!< @if Eng client configuration descriptor
                                                      @else   客户端配置描述符 @endif */
    XF_SLE_SSAP_DESC_TYPE_SERVER_CONFIGURATION = 0x03, /*!< @if Eng server configuration descriptor
                                                      @else   服务端配置描述符 @endif */
    XF_SLE_SSAP_DESC_TYPE_PRESENTATION_FORMAT  = 0x04, /*!< @if Eng presentation format descriptor
                                                      @else   格式描述符 @endif */
    XF_SLE_SSAP_DESC_TYPE_RFU                  = 0x05, /*!< @if Eng RFU descriptor, 0x05 – 0x1F
                                                      @else   服务管理保留描述符，0x05 – 0x1F @endif */
    XF_SLE_SSAP_DESC_TYPE_CUSTOM               = 0xFF, /*!< @if Eng custom descriptor
                                                      @else   厂商自定义描述符 @endif */
} xf_sle_ssap_desc_type_t;

typedef uint16_t xf_sle_attr_handle_t;


/**
 * @if Eng
 * @brief  SLE SSAP protocol error code.
 * @else
 * @brief  SLE SSAP 协议错误码。
 * @endif
 */
typedef enum {
    XF_SLE_SSAP_ERR_INVALID_PDU = 0x01,                     /*!< @if Eng invalid PDU
                                                                @else   服务端接收的 PDU 无效
                                                                @endif */
    XF_SLE_SSAP_ERR_PDU_NOT_SUPPORT = 0x02,                 /*!< @if Eng PDU not support
                                                                @else   服务端不支持处理接收的
                                                                        PDU
                                                                @endif */
    XF_SLE_SSAP_ERR_UNKNOW = 0x03,                          /*!< @if Eng PDU unknow
                                                                @else   服务端执行请求时发生
                                                                        未知错误
                                                                @endif */
    XF_SLE_SSAP_ERR_INVALID_HANDLE = 0x04,                  /*!< @if Eng invalid handle
                                                                @else   请求中的句柄无效
                                                                @endif */
    XF_SLE_SSAP_ERR_INSUFFICIENT_RESOURCES = 0x05,          /*!< @if Eng insufficient resources
                                                                @else   服务端没有足够资源完成
                                                                        请求
                                                                @endif */
    XF_SLE_SSAP_ERR_PROHIBIT_READING = 0x06,                /*!< @if Eng prohibit reading
                                                                @else   服务端禁止客户端读取值
                                                                @endif */
    XF_SLE_SSAP_ERR_PROHIBIT_WRITE = 0x07,                  /*!< @if Eng prohibit writing
                                                                @else   服务端禁止客户端写入值
                                                                @endif */
    XF_SLE_SSAP_ERR_CLIENT_NOT_AUTHENTICATED = 0x08,        /*!< @if Eng client no authentication
                                                                @else   客户端未经过认证
                                                                @endif */
    XF_SLE_SSAP_ERR_CLIENT_NOT_AUTHORIZATION = 0x09,        /*!< @if Eng client no authorizaion
                                                                @else   客户端未被授权
                                                                @endif */
    XF_SLE_SSAP_ERR_BEARER_NOT_ENCRYPTED = 0x0A,            /*!< @if Eng bearer not encrypted
                                                                @else   传输 PDU 的承载未加密
                                                                @endif */
    XF_SLE_SSAP_ERR_ENTRIES_NOT_FOUND = 0x0B,               /*!< @if Eng entries not found
                                                                @else   服务端未找到对应条目
                                                                @endif */
    XF_SLE_SSAP_ERR_DATA_NOT_FOUND = 0x0C,                  /*!< @if Eng data not found
                                                                @else   服务端未找到对应类型数据
                                                                @endif */
    XF_SLE_SSAP_ERR_INCORRECT_DATA_TYPE = 0x0D,             /*!< @if Eng data type incorrected
                                                                @else   客户端发送写入数据类型
                                                                        不符的错误
                                                                @endif */
    XF_SLE_SSAP_ERR_INCORRECT_DATA_VALUE = 0x0E,            /*!< @if Eng data value incorrected
                                                                @else   客户端发送写入值不符
                                                                        的错误
                                                                @endif */
    XF_SLE_SSAP_ERR_VALUE_OUT_OF_RANGE = 0x0F,              /*!< @if Eng data value incorrected
                                                                @else   客户端写入的值超出范围
                                                                @endif */
    XF_SLE_SSAP_ERR_UPPERLAYER_APP_ERR_MIN = 0xAF,          /*!< @if Eng reserve
                                                                @else   预留给上层协议定义应用
                                                                        错误
                                                                @endif */
    XF_SLE_SSAP_ERR_UPPERLAYER_APPL_ERR_MAX = 0xFF,         /*!< @if Eng reserve
                                                                @else   预留给上层协议定义应用
                                                                        错误
                                                                @endif */
} xf_sle_ssap_err_t;

/**
 * @if Eng
 * @brief  SSAP property type.
 * @else
 * @brief  SSAP 特征类型。
 * @endif
 */
typedef enum {
    XF_SLE_SSAP_PROPERTY_TYPE_VALUE             = 0x00, /*!< @if Eng property value
                                                      @else   特征值 @endif */
    XF_SLE_SSAP_DESCRIPTOR_USER_DESCRIPTION     = 0x01, /*!< @if Eng user description descriptor
                                                      @else   属性说明描述符 @endif */
    XF_SLE_SSAP_DESCRIPTOR_CLIENT_CONFIGURATION = 0x02, /*!< @if Eng client configuration descriptor
                                                      @else   客户端配置描述符 @endif */
    XF_SLE_SSAP_DESCRIPTOR_SERVER_CONFIGURATION = 0x03, /*!< @if Eng server configuration descriptor
                                                      @else   服务端配置描述符 @endif */
    XF_SLE_SSAP_DESCRIPTOR_PRESENTATION_FORMAT  = 0x04, /*!< @if Eng presentation format descriptor
                                                      @else   格式描述符 @endif */
    XF_SLE_SSAP_DESCRIPTOR_RFU                  = 0x05, /*!< @if Eng RFU descriptor, 0x05 – 0x1F
                                                      @else   服务管理保留描述符，0x05 – 0x1F @endif */
    XF_SLE_SSAP_DESCRIPTOR_CUSTOM               = 0xFF, /*!< @if Eng custom descriptor
                                                      @else   厂商自定义描述符 @endif */
} xf_sle_ssap_property_type_t;

/**
 * @if Eng
 * @brief  Struct of ssap info exchange
 * @else
 * @brief  ssap 信息交换结构体。
 * @endif
 */
typedef struct {
    uint32_t mtu_size; /*!< @if Eng mtu size
        ·                   @else   mtu大小 @endif */
    uint16_t version;  /*!< @if Eng version
                            @else   版本 @endif */
} xf_sle_ssap_exchange_info_t;


typedef enum {
    XF_SLE_EVT_ALL,
    XF_SLE_CONN_EVT_CONNECT,
    XF_SLE_CONN_EVT_DISCONNECT,
    XF_SLE_CONN_EVT_CONN_PARAMS_UPDATE,
    XF_SLE_CONN_EVT_REQ_CONN_PARAMS_UPDATE,

    // XF_SLE_CONN_EVT_AUTH_CMPL,
    // XF_SLE_CONN_EVT_PAIR_CMPL,

    // XF_SLE_CONN_EVT_READ_RSSI,
    // XF_SLE_CONN_EVT_LOW_LATENCY,
    // XF_SLE_CONN_EVT_SET_PHY,

} xf_sle_ssap_common_event_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_SSAP_TYPES_H__ */
