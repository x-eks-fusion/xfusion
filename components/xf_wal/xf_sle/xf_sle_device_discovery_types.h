/**
 * @file xf_sle_device_discovery_types.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_SLE_DEVICE_DISCOVERY_TYPES_H__
#define __XF_SLE_DEVICE_DISCOVERY_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sle_types.h"

#if XF_SLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief
 *  |   Public Information Data Structure   |
 *  | Data Substructure 1 | Data Substructure 2| ...
 *  |                      \
 *  |                       \__________________________________
 *  |                                                          \
 *  |                 (len of content)                          \
 *  | Type (1 octet)| Length (1 octet) | Content (Length octets) |
 */

typedef enum {
    XF_SLE_ADV_STRUCT_TYPE_DISCOVERY_LEVEL                              = 0x01,   /*!< 发现等级 */
    XF_SLE_ADV_STRUCT_TYPE_ACCESS_MODE                                  = 0x02,   /*!< 接入层能力 */
    XF_SLE_ADV_STRUCT_TYPE_SERVICE_DATA_16BIT_UUID                      = 0x03,   /*!< 标准服务数据信息 */
    XF_SLE_ADV_STRUCT_TYPE_SERVICE_DATA_128BIT_UUID                     = 0x04,   /*!< 自定义服务数据信息 */
    XF_SLE_ADV_STRUCT_TYPE_COMPLETE_LIST_OF_16BIT_SERVICE_UUIDS         = 0x05,   /*!< 完整标准服务标识列表 */
    XF_SLE_ADV_STRUCT_TYPE_COMPLETE_LIST_OF_128BIT_SERVICE_UUIDS        = 0x06,   /*!< 完整自定义服务标识列表 */
    XF_SLE_ADV_STRUCT_TYPE_INCOMPLETE_LIST_OF_16BIT_SERVICE_UUIDS       = 0x07,   /*!< 部分标准服务标识列表 */
    XF_SLE_ADV_STRUCT_TYPE_INCOMPLETE_LIST_OF_128BIT_SERVICE_UUIDS      = 0x08,   /*!< 部分自定义服务标识列表 */
    XF_SLE_ADV_STRUCT_TYPE_SERVICE_STRUCTURE_HASH_VALUE                 = 0x09,   /*!< 服务结构散列值 */
    XF_SLE_ADV_STRUCT_TYPE_SHORTENED_LOCAL_NAME                         = 0x0A,   /*!< 设备缩写本地名称 */
    XF_SLE_ADV_STRUCT_TYPE_COMPLETE_LOCAL_NAME                          = 0x0B,   /*!< 设备完整本地名称 */
    XF_SLE_ADV_STRUCT_TYPE_TX_POWER_LEVEL                               = 0x0C,   /*!< 广播发送功率 */
    XF_SLE_ADV_STRUCT_TYPE_SLB_COMMUNICATION_DOMAIN                     = 0x0D,   /*!< SLB通信域域名 */
    XF_SLE_ADV_STRUCT_TYPE_SLB_MEDIA_ACCESS_LAYER_ID                    = 0x0E,   /*!< SLB媒体接入层标识 */
    XF_SLE_ADV_STRUCT_TYPE_EXTENDED                                     = 0xFE,   /*!< 数据类型扩展 */
    XF_SLE_ADV_STRUCT_TYPE_MANUFACTURER_SPECIFIC_DATA                   = 0xFF    /*!< 厂商自定义信息 */
} xf_sle_adv_struct_type_t;

#define XF_SLE_ADV_STRUCT_TYPE_FILED_SIZE       1
#define XF_SLE_ADV_STRUCT_LEN_FILED_SIZE        1

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

typedef union _xf_sle_adv_struct_data_t {
    xf_sle_var_uintptr_t adv_var;   // 通用类型（以下未暂未囊括的类型可以使用该通用类型）

    uint8_t *local_name;    // 0x0B, type:name short; 0x09, type:name all
    uint8_t discovery_level;    // 0x0B, type:name short; 0x09, type:name all
} xf_sle_adv_struct_data_t;

/**
 * @brief BLE GAP 广播 AD Structure 信息（并非严格广播包结构）
 */
typedef struct {
    uint8_t struct_data_len;    // 广播（子）结构中的数据的长度（仅数据主体的长度不包含存储类型的变量的长度）
    xf_sle_adv_struct_type_t type;
    xf_sle_adv_struct_data_t data;
} xf_sle_adv_struct_t;

#define XF_SLE_SSAP_STRUCT_INFO_BASE                            \
    union                                                       \
    {                                                           \
        xf_sle_adv_struct_type_t type;                          \
        uint8_t _fixed_size;                                    \
    };                                                          \
    uint8_t struct_data_len;                                    \

#define XF_SLE_SSAP_STRUCT_TYPE_ARRAY_U8(type_name, adv_data_array_size)   \
typedef struct {                                                \
    XF_SLE_SSAP_STRUCT_INFO_BASE                                \
    uint8_t data[adv_data_array_size];                          \
}type_name

#define XF_SLE_SSAP_STRUCT_TYPE_VAL_U8(type_name)               \
typedef struct {                                                \
    XF_SLE_SSAP_STRUCT_INFO_BASE                                \
    uint8_t data;                                               \
}type_name

/**
 * @if Eng
 * @brief  Announce data.
 * @else
 * @brief  设备公开数据。
 * @endif
 */
typedef struct _xf_sle_announce_data_t {
    xf_sle_adv_struct_t *announce_struct_set;   // 公开（广播）数据结构的集合
    uint16_t seek_rsp_data_len;  /*!< @if Eng scan response data length
                                      @else   扫描响应数据长度 @endif */
    uint8_t  *seek_rsp_data;     /*!< @if Eng scan response data
                                      @else   扫描响应数据 @endif */
} xf_sle_announce_data_t;


/**
 * @if Eng
 * @brief  announce type.
 * @else
 * @brief  设备公开类型。
 * @endif
 */
typedef enum {
    XF_SLE_ANNOUNCE_TYPE_NONCONN_NONSCAN      = 0x00, /*!< @if Eng non-connectable, non-scannable.
                                                         @else   不可连接不可扫描。 @endif */
    XF_SLE_ANNOUNCE_TYPE_CONNECTABLE_NONSCAN  = 0x01, /*!< @if Eng connectable, non-scannable.
                                                         @else   可连接不可扫描。 @endif */
    XF_SLE_ANNOUNCE_TYPE_NONCONN_SCANABLE     = 0x02, /*!< @if Eng non-connectable, scannable.
                                                         @else   不可连接可扫描。 @endif */
    XF_SLE_ANNOUNCE_TYPE_CONNECTABLE_SCANABLE = 0x03, /*!< @if Eng connectable, scannable.
                                                         @else   可连接可扫描。 @endif */
    XF_SLE_ANNOUNCE_TYPE_CONNECTABLE_DIRECTED = 0x07, /*!< @if Eng connectable, scannable, directed.
                                                         @else   可连接可扫描定向。 @endif */
} xf_sle_announce_type_t;

/**
 * @if Eng
 * @brief  announce level.
 * @else
 * @brief  被发现方可发现等级
 * @endif
 */
typedef enum {
    XF_SLE_ANNOUNCE_LEVEL_NONE,     /*!< @if Eng announce level none, reserve
                                       @else   不可见发现，预留 @endif */
    XF_SLE_ANNOUNCE_LEVEL_NORMAL,   /*!< @if Eng announce level normal
                                       @else   一般可发现 @endif */
    XF_SLE_ANNOUNCE_LEVEL_PRIORITY, /*!< @if Eng announce level priority, reserve
                                       @else   优先可发现，预留 @endif */
    XF_SLE_ANNOUNCE_LEVEL_PAIRED,   /*!< @if Eng announce level paired, reserve
                                       @else   被曾配对过的设备发现，预留 @endif */
    XF_SLE_ANNOUNCE_LEVEL_SPECIAL,  /*!< @if Eng announce level special
                                       @else   被指定设备发现 @endif */
} xf_sle_announce_level_t;

/**
 * @if Eng
 * @brief Definitaion of BLE ADV Channel mapping.
 * @else
 * @brief SLE 广播信道映射。
 * @endif
 */
typedef enum {
    XF_SLE_ADV_CHANNEL_MAP_77                 = 0x01,
    XF_SLE_ADV_CHANNEL_MAP_78                 = 0x02,
    XF_SLE_ADV_CHANNEL_MAP_79                 = 0x04,
    XF_SLE_ADV_CHANNEL_MAP_DEFAULT            = 0x07
} xf_sle_adv_channel_map;

/**
 * @if Eng
 * @brief  G/T role negotiation indication.
 * @else
 * @brief  G/T 角色协商指示。
 * @endif
 */
typedef enum {
    XF_SLE_ANNOUNCE_ROLE_T_CAN_NEGO = 0, /*!< @if Eng Expect to be T, negotiable
                                               @else   期望做T可协商 @endif */
    XF_SLE_ANNOUNCE_ROLE_G_CAN_NEGO,     /*!< @if Eng Expect to be G, negotiable
                                               @else   期望做G可协商 @endif */
    XF_SLE_ANNOUNCE_ROLE_T_NO_NEGO,      /*!< @if Eng Expect to be T, non-negotiable
                                               @else   期望做T不可协商 @endif */
    XF_SLE_ANNOUNCE_ROLE_G_NO_NEGO       /*!< @if Eng Expect to be G, non-negotiable
                                               @else   期望做G不可协商 @endif */
} xf_sle_announce_gt_role_t;

/**
 * @if Eng
 * @brief  Announce parameter.
 * @else
 * @brief  设备公开参数。
 * @endif
 */
typedef struct {
    uint32_t announce_interval_min;         /*!< @if Eng minimum of announce interval
                                                 @else   最小设备公开周期, 0x000020~0xffffff, 单位125us @endif */
    uint32_t announce_interval_max;         /*!< @if Eng maximum of announce interval
                                                 @else   最大设备公开周期, 0x000020~0xffffff, 单位125us @endif */
    xf_sle_announce_type_t  announce_type;  /*!< 设备公开类型 */
    xf_sle_addr_t own_addr;                /*!< @if Eng own address
                                                @else   本端地址 @endif */
    xf_sle_addr_t peer_addr;                /*!< @if Eng peer address
                                                @else   对端地址 @endif */
    xf_sle_adv_channel_map  announce_channel_map;   /*!< 设备公开信道, 0:76, 1:77, 2:78 */
    int8_t   announce_tx_power;             /*!< @if Eng adv transmit power
                                                 @else   广播发射功率，单位dbm, 取值范围[-127, 20],
                                                         0x7F：不设置特定发送功率 @endif */


    uint8_t announce_handle;               /*!< @if Eng announce handle
                                                 @else   设备公开句柄，取值范围[0, 0xFF] @endif */
    xf_sle_announce_gt_role_t announce_gt_role;     /*!< G/T 角色协商指示 */
    xf_sle_announce_level_t announce_level; /*! 发现等级 */


    uint16_t conn_interval_min;             /*!< @if Eng minimum of connection interval
                                                 @else   连接间隔最小取值，取值范围[0x001E,0x3E80]，
                                                         announce_gt_role 为 SLE_ANNOUNCE_ROLE_T_NO_NEGO
                                                         时无需配置 @endif */
    uint16_t conn_interval_max;             /*!< @if Eng maximum of connection interval
                                                 @else   连接间隔最大取值，取值范围[0x001E,0x3E80]，
                                                         announce_gt_role 为 SLE_ANNOUNCE_ROLE_T_NO_NEGO
                                                         无需配置 @endif */
    uint16_t conn_max_latency;              /*!< @if Eng max connection latency
                                                 @else   最大休眠连接间隔，取值范围[0x0000,0x01F3]，
                                                         announce_gt_role 为 SLE_ANNOUNCE_ROLE_T_NO_NEGO
                                                         无需配置 @endif */
    uint16_t conn_supervision_timeout;      /*!< @if Eng connect supervision timeout
                                                 @else   最大超时时间，取值范围[0x000A,0x0C80]，
                                                         announce_gt_role 为 SLE_ANNOUNCE_ROLE_T_NO_NEGO
                                                         无需配置 @endif */
    void *ext_param;                        /*!< @if Eng extend parameter, default value is NULL
                                                 @else   扩展设备公开参数, 缺省时置空 @endif */
} xf_sle_announce_param_t;


/**
 * @if Eng
 * @brief  Seek filter type.
 * @else
 * @brief  设备发现过滤类型。
 * @endif
 */
typedef enum {
    XF_SLE_SEEK_FILTER_ALLOW_ALL   = 0x00, /*!< @if Eng allow all
                                                       @else   允许来自任何人的设备发现数据包 @endif */
    XF_SLE_SEEK_FILTER_ALLOW_WHITE_LIST  = 0x01, /*!< @if Eng allow only white list, reserve
                                                       @else   允许来自白名单设备的设备发现数据包，预留 @endif */
} xf_sle_seek_filter_t;

/**
 * @if Eng
 * @brief  Seek phy type.
 * @else
 * @brief  设备发现PHY类型。
 * @endif
 */
typedef enum {
    XF_SLE_SEEK_PHY_1M = 0x1, /*!< @if Eng 1M PHY
                                     @else   1M PHY @endif */
    XF_SLE_SEEK_PHY_2M = 0x2, /*!< @if Eng 2M PHY
                                     @else   2M PHY @endif */
    XF_SLE_SEEK_PHY_4M = 0x4, /*!< @if Eng 4M PHY
                                     @else   4M PHY @endif */
} xf_sle_seek_phy_t;

/**
 * @if Eng
 * @brief  seek type.
 * @else
 * @brief  设备发现类型。
 * @endif
 */
typedef enum {
    XF_SLE_SEEK_TYPE_PASSIVE = 0x00, /*!< @if Eng passive seek
                                            @else   被动扫描 @endif */
    XF_SLE_SEEK_TYPE_ACTIVE  = 0x01, /*!< @if Eng active seek
                                            @else   主动扫描 @endif */
} xf_sle_seek_type_t;

/**
 * @if Eng
 * @brief  Maximum of scan PHY num.
 * @else
 * @brief  设备发现PHY最大值。
 * @endif
 */
#define XF_SLE_SEEK_PHY_NUM_MAX 3       // FIXME 这个不知道是不是通用值 感觉与平台相关

/**
 * @if Eng
 * @brief  Seek scan parameter.
 * @else
 * @brief  设备发现扫描参数。
 * @endif
 */
typedef struct {
    xf_sle_addr_type_t own_addr_type;                        /*!< @if Eng own address type
                                                       @else   本端地址类型 @endif */
    bool filter_duplicates;                    /*!< @if Eng duplicates filter
                                                       @else   重复过滤开关，0：关闭，1：开启 @endif */
    xf_sle_seek_filter_t seek_filter_policy;    /*!< 扫描设备使用的过滤类型 */
    xf_sle_seek_phy_t seek_phy;                 /*!< 扫描设备所使用的PHY */

    struct {
        xf_sle_seek_type_t seek_type;        /*!< 扫描类型 */
        uint16_t seek_interval;             /*!< 扫描间隔，取值范围[0x0004, 0xFFFF]，time = N * 0.125ms */
        uint16_t seek_window;               /*!< 扫描窗口，取值范围[0x0004, 0xFFFF]，time = N * 0.125ms */
    } phy_param_set[XF_SLE_SEEK_PHY_NUM_MAX];
} xf_sle_seek_param_t;


/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_DEVICE_DISCOVERY_TYPES_H__ */
