/**
 * @file xf_sle_ssap_types.h
 * @author dotc (dotchan@qq.com)
 * @brief SparkLink Service Access Protocol.
 * @version 1.0
 * @date 2024-08-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/**
 * @cond (XFAPI_USER || XFAPI_PORT || XFAPI_INTERNAL)
 * @ingroup group_xf_wal_sle
 * @defgroup group_xf_wal_sle_ssap ssap
 * @brief SparkLink Service Access Protocol.
 * @endcond
 */

#ifndef __XF_SLE_SSAP_TYPES_H__
#define __XF_SLE_SSAP_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sle_types.h"
#include "xf_sle_connection_manager_types.h"

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_wal_sle_ssap
 * @endcond
 * @{
 */

#if XF_SLE_IS_ENABLE || defined(__DOXYGEN__)

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief SLE SSAP 服务类型
 */
typedef enum {
    XF_SLE_SSAP_SERVICE_TYPE_PRIMARY,
    XF_SLE_SSAP_SERVICE_TYPE_SECONDARY,
} xf_sle_ssap_service_type_t;

/**
 * @brief SLE SSAP 属性权限
 */
typedef enum {
    XF_SLE_SSAP_PERMISSION_READ                = 0x01,  /*!< 可读 */
    XF_SLE_SSAP_PERMISSION_WRITE               = 0x02,  /*!< 可写 */
    XF_SLE_SSAP_PERMISSION_ENCRYPTION_NEED     = 0x04,  /*!< 需要加密 */
    XF_SLE_SSAP_PERMISSION_AUTHENTICATION_NEED = 0x08,  /*!< 需要认证 */
    XF_SLE_SSAP_PERMISSION_AUTHORIZATION_NEED  = 0x10,  /*!< 需要授权 */
} xf_sle_ssap_permission_t;

/**
 * @brief SLE SSAP 操作指示
 */
typedef enum {
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_READ             = 0x01,     /*!< 读数据值：允许读取数据值 */
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE_NO_RSP     = 0x02,     /*!< 写数据值且无需响应：允许写入数据值 (没有反馈/响应)  */
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE            = 0x04,     /*!< 写数据值且需要响应：允许写入数据值 (有反馈/响应) */
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_NOTIFY           = 0x08,     /*!< 通知数据值：允许向客户端进行数据值通知 */
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_INDICATE         = 0x10,     /*!< 指示数据值：允许向客户端进行数据值指示 */
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_BROADCAST        = 0x20,     /*!< 广播数据值：允许对该数据值进行广播，数据值可携带在广播中 */
    XF_SLE_SSAP_OPERATE_INDICATION_BIT_DESCRITOR_WRITE  = 0x100,    /*!< 写描述符：描述符可被写入 */
    XF_SLE_SSAP_OPERATE_INDICATION_MAX,                             /*!< SLE SSAP 操作指示最大值 */
} xf_sle_ssap_operate_indication_t;

/**
 * @brief SLE SSAP 描述符类型
 */
typedef enum {
    XF_SLE_SSAP_DESC_TYPE_USER_DESCRIPTION     = 0x01,          /*!< 属性说明描述符 (user description descriptor) */
    XF_SLE_SSAP_DESC_TYPE_CLIENT_CONFIGURATION = 0x02,          /*!< 客户端配置描述符 (client configuration descriptor) */
    XF_SLE_SSAP_DESC_TYPE_SERVER_CONFIGURATION = 0x03,          /*!< 服务端配置描述符 (server configuration descriptor) */
    XF_SLE_SSAP_DESC_TYPE_PRESENTATION_FORMAT  = 0x04,          /*!< 格式描述符 (presentation format descriptor) */
    XF_SLE_SSAP_DESC_TYPE_RFU                  = 0x05,          /*!< 服务管理保留描述符 (RFU descriptor, 0x05 – 0x1F) */
    XF_SLE_SSAP_DESC_TYPE_CUSTOM               = 0xFF,          /*!< 厂商自定义描述符 (custom descriptor) */
} xf_sle_ssap_desc_type_t;

/**
 * @brief SLE SSAP 协议错误码
 */
typedef enum {
    XF_SLE_SSAP_ERR_INVALID_PDU                 = 0x01,         /*!< 服务端接收的 PDU 无效 */
    XF_SLE_SSAP_ERR_PDU_NOT_SUPPORT             = 0x02,         /*!< 服务端不支持处理接收的 PDU */
    XF_SLE_SSAP_ERR_UNKNOW                      = 0x03,         /*!< 服务端执行请求时发生未知错误 (PDU unknow) */
    XF_SLE_SSAP_ERR_INVALID_HANDLE              = 0x04,         /*!< 请求中的句柄无效 */
    XF_SLE_SSAP_ERR_INSUFFICIENT_RESOURCES      = 0x05,         /*!< 服务端没有足够资源完成请求 */
    XF_SLE_SSAP_ERR_PROHIBIT_READING            = 0x06,         /*!< 服务端禁止客户端读取值 */
    XF_SLE_SSAP_ERR_PROHIBIT_WRITE              = 0x07,         /*!< 服务端禁止客户端写入值 */
    XF_SLE_SSAP_ERR_CLIENT_NOT_AUTHENTICATED    = 0x08,         /*!< 客户端未经认证 */
    XF_SLE_SSAP_ERR_CLIENT_NOT_AUTHORIZATION    = 0x09,         /*!< 客户端未被授权 */
    XF_SLE_SSAP_ERR_BEARER_NOT_ENCRYPTED        = 0x0A,         /*!< 传输 PDU 的承载未加密 */
    XF_SLE_SSAP_ERR_ENTRIES_NOT_FOUND           = 0x0B,         /*!< 服务端未找到对应条目 */
    XF_SLE_SSAP_ERR_DATA_NOT_FOUND              = 0x0C,         /*!< 服务端未找到对应类型数据 */
    XF_SLE_SSAP_ERR_INCORRECT_DATA_TYPE         = 0x0D,         /*!< 客户端发送写入数据类型不符的错误 */
    XF_SLE_SSAP_ERR_INCORRECT_DATA_VALUE        = 0x0E,         /*!< 客户端发送写入值不符的错误 */
    XF_SLE_SSAP_ERR_VALUE_OUT_OF_RANGE          = 0x0F,         /*!< 客户端写入的值超出范围 */
    XF_SLE_SSAP_ERR_UPPERLAYER_APP_ERR_MIN      = 0xAF,         /*!< 预留给上层协议定义应用错误 */
    XF_SLE_SSAP_ERR_UPPERLAYER_APPL_ERR_MAX     = 0xFF,         /*!< 预留给上层协议定义应用错误 */
} xf_sle_ssap_err_t;

/**
 * @brief SLE SSAP 特征类型 (property type)
 */
typedef enum {
    XF_SLE_SSAP_PROPERTY_TYPE_VALUE             = 0x00, /*!< 特征值 (property value) */
    XF_SLE_SSAP_DESCRIPTOR_USER_DESCRIPTION     = 0x01, /*!< 属性说明描述符 (user description descriptor) */
    XF_SLE_SSAP_DESCRIPTOR_CLIENT_CONFIGURATION = 0x02, /*!< 客户端配置描述符 (client configuration descriptor) */
    XF_SLE_SSAP_DESCRIPTOR_SERVER_CONFIGURATION = 0x03, /*!< 服务端配置描述符 (server configuration descriptor) */
    XF_SLE_SSAP_DESCRIPTOR_PRESENTATION_FORMAT  = 0x04, /*!< 格式描述符 (presentation format descriptor) */
    XF_SLE_SSAP_DESCRIPTOR_RFU                  = 0x05, /*!< 服务管理保留描述符，0x05 – 0x1F (RFU descriptor) */
    XF_SLE_SSAP_DESCRIPTOR_CUSTOM               = 0xFF, /*!< 厂商自定义描述符 (custom descriptor) */
} xf_sle_ssap_property_type_t;

/**
 * @brief SLE SSAP 信息交换 (协商) 信息 (info exchange)
 */
typedef struct {
    uint32_t mtu_size;      /*!< MTU 大小 */
    uint16_t version;       /*!< 版本 */
} xf_sle_ssap_exchange_info_t;


#define XF_SLE_EVT_ALL       (-1)        /*!< 所有事件 */
/**
 * @brief SLE SSAP 通用事件类型
 */
typedef enum {
    XF_SLE_COMMON_EVT_CONNECT,                  /*!< 连接事件 */
    XF_SLE_COMMON_EVT_DISCONNECT,               /*!< 断连事件 */
    XF_SLE_COMMON_EVT_SEEK_RESULT,              /*!< 收到扫描结果事件 */
    XF_SLE_COMMON_EVT_PAIR_END,                 /*!< 配对结束事件 */
    XF_SLE_COMMON_EVT_CONN_PARAMS_UPDATE,       /*!< 连接参数更新事件 */
    _XF_SLE_COMMON_EVT_ENUM_END,                /*!< SLE 通用事件枚举结束值 */
} xf_sle_common_event_t;

/**
 * @brief SLE 连接事件的参数
 */
typedef struct {
    uint16_t conn_id;                   /*!< 链接(连接) ID */
    xf_sle_addr_t peer_addr;            /*!< 对端地址，见 @ref xf_sle_addr_t */
    xf_sle_pair_state_t pair_state;     /*!< 配对状态，见 @ref xf_sle_pair_state_t */
} xf_sle_common_evt_param_connect_t;

/**
 * @brief SLE 断连事件的参数
 */
typedef struct {
    uint16_t conn_id;                   /*!< 链接(连接) ID */
    xf_sle_addr_t peer_addr;            /*!< 对端地址，见 @ref xf_sle_addr_t */
    xf_sle_pair_state_t pair_state;     /*!< 配对状态，见 @ref xf_sle_pair_state_t */
    xf_sle_disconnect_reason_t reason;  /*!< 断连原因，见 @ref xf_sle_disconnect_reason_t */
} xf_sle_common_evt_param_disconnect_t;

/**
 * @brief SLE 连接参数更新事件的参数
 */
typedef struct {
    uint16_t conn_id;
    uint16_t interval;              /*!< 链路调度间隔，单位slot */
    uint16_t latency;               /*!< 延迟周期，单位slot */
    uint16_t supervision_timeout;   /*!< 超时时间，单位10ms */
} xf_sle_common_evt_param_conn_param_update_t;

/**
 * @brief SLE 收到扫描结果事件的参数
 */
typedef struct {
    uint8_t evt_type;               /*!< 上报事件类型 */
    xf_sle_addr_t peer_addr;        /*!< 对端地址，见 @ref xf_sle_addr_t */
    xf_sle_addr_t direct_addr;      /*!< 定向发现地址，见 @ref xf_sle_addr_t */
    /* FIXME 这里不知道为啥官方 SDK 用的是 uint8_t */
    uint8_t rssi;                   /*!< 信号强度指示，取值范围[-127dBm, 20dBm]，0x7F表示不提供信号强度指示 */
    uint8_t data_status;            /*!< 数据状态 */
    uint8_t data_len;               /*!< 数据长度 */
    uint8_t *data;                  /*!< 数据内容 */
} xf_sle_common_evt_param_seek_result_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_wal_sle_ssap
 * @}
 */

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_SSAP_TYPES_H__ */
