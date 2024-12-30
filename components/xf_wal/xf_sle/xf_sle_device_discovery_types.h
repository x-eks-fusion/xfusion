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

#if XF_SLE_IS_ENABLE || defined(__DOXYGEN__)

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

/* ==================== [Typedefs] ========================================== */

/**
 * @brief SLE 广播单元类型
 * @note 广播单元 Data Structure，类似 BLE AD Structure
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

/**
 * @brief SLE 广播数据单元类型字段的大小
 */
#define XF_SLE_ADV_STRUCT_TYPE_FILED_SIZE       1

/**
 * @brief SLE 广播数据单元数据长度字段的大小
 */
#define XF_SLE_ADV_STRUCT_LEN_FILED_SIZE        1

/**
 * @brief SLE 广播数据单元数据的最大长度
 * @note 该宏一般只用于对接广播数据设置时，XF 便捷方法的处理。
 */
#define XF_SLE_ADV_STRUCT_DATA_MAX_SIZE        (254)

/**
 * @brief SLE 广播数据单元的数据
 *
 * @note 以下暂时仅列出部分类型的广播数据单元数据成员
 * @details 以下为星闪标准定义的广播数据结构及广播数据单元数据 ( AD Data ) 所在的位置
 * （SLE、BLE 广播数据结构类似，但也有些区别，可与蓝牙标准中的广播数据结构进行对比观看）
 * @code
 *  | Public Information Data Structure                                                                     ......  |
 *  | Data Substructure 1                                                           | Data Substructure 2|  ......  |
 *  | Type (1 octet) | Length (1 octet, len of content) | Content (Length octets)   | ......                        |
 *                                                      |             ^             |
 * @endcode
 */
typedef union _xf_sle_adv_struct_data_t {
    xf_sle_var_uintptr_t adv_var;       /*!< 通用类型 (类型不定) 的广播数据单元数据 */

    uint8_t *local_name;
    uint8_t discovery_level;
} xf_sle_adv_struct_data_t;

/**
 * @brief SLE 广播数据单元 ( AD structure ）
 *
 * @note 广播数据包与扫描响应数据包均是这个结构
 * @warning 这里的内存空间结构及成员并非严格按星闪标准定义的广播数据单元结构进行定义
 * @details 以下为星闪标准定义的广播数据结构及广播数据单元 ( AD structure ）所在的位置
 * （SLE、BLE 广播数据结构类似，但也有些区别，可与蓝牙标准中的广播数据结构进行对比观看）
 * @code
 *  | Public Information Data Structure                                                                     ......  |
 *  | Data Substructure 1                                                           | Data Substructure 2|  ......  |
 *  | Type (1 octet) | Length (1 octet, len of content) | Content (Length octets)   | ......                        |
 *  |                          ^                        |
 * @endcode
 */
typedef struct {
    uint8_t is_ptr              :1;     /*!< 传入的广播单元数据是值还是指针()，如果是指针则 true ；否则 false */
    uint8_t ad_data_len         :7;     /*!< 广播数据单元的数据 (AD Data) 的长度 */
    xf_sle_adv_struct_type_t ad_type;   /*!< 广播数据单元的类型，见 @ref xf_sle_adv_struct_type_t */
    xf_sle_adv_struct_data_t ad_data;   /*!< 广播数据单元的数据 (AD Data)，见 @ref xf_sle_adv_struct_data_t */
} xf_sle_adv_struct_t;

/**
 * @brief 定义一个严格遵循星闪标准的广播数据单元结构，单元数据 ( AD Data ) 为数组的类型
 *
 * @param type_name 指定定义的类型名
 * @param adv_data_array_size 单元数据 ( AD Data ) 数组的大小
 * @note 一般仅用于平台对接时使用，便于 XF SLE 广播数据单元结构与符号标准的广播数据结构间的转换
 */
#define XF_SLE_ADV_STRUCT_TYPE_ARRAY_U8(type_name, adv_data_array_size)   \
typedef struct {                                                \
    xf_sle_adv_struct_type_t ad_type;                           \
    uint8_t ad_data_len;                                        \
    uint8_t ad_data[adv_data_array_size];                       \
}type_name

/**
 * @brief 定义一个严格遵循星闪标准的广播数据单元结构，单元数据 ( AD Data ) 为固定大小数组的类型
 *
 * @param type_name 指定定义的类型名
 * @note 一般仅用于平台对接时使用，便于 XF SLE 广播数据单元结构与符号标准的广播数据结构间的转换
 * @warning 注意，此处数组大小为广播包最大大小，并不代表广播数据单元 (adv struct) 的实际有效大小
 */
#define XF_SLE_ADV_STRUCT_TYPE_ARRAY_FIXED(type_name)           \
typedef struct {                                                \
    xf_sle_adv_struct_type_t ad_type;                           \
    uint8_t ad_data_len;                                        \
    uint8_t ad_data[XF_SLE_ADV_STRUCT_DATA_MAX_SIZE];           \
}type_name

/**
 * @brief  SLE 广播 (公开) 数据 ( 包含响应数据 )
 * @warning 目前 广播数据单元 与 扫描响应数据单元并不是严格按标准的广播数据结构进行定义，
 *  而是从更方便使用的角度对标准的结构进行了微调
 */
typedef struct _xf_sle_announce_data_t {
    xf_sle_adv_struct_t *announce_struct_set;   /*!< 广播数据单元（ AD Structure ）的集合，
                                                 * 见 @ref xf_sle_adv_struct_t */
    xf_sle_adv_struct_t *seek_rsp_struct_set;   /*!< 扫描响应数据单元（ AD Structure ）的集合，
                                                 * 见 @ref xf_sle_adv_struct_t */
} xf_sle_announce_data_t;

/**
 * @brief SLE 广播 (公开) 类型
 */
typedef enum {
    XF_SLE_ANNOUNCE_TYPE_NONCONN_NONSCAN      = 0x00,   /*!< 不可连接不可扫描 */
    XF_SLE_ANNOUNCE_TYPE_CONNECTABLE_NONSCAN  = 0x01,   /*!< 可连接不可扫描 */
    XF_SLE_ANNOUNCE_TYPE_NONCONN_SCANABLE     = 0x02,   /*!< 不可连接可扫描 */
    XF_SLE_ANNOUNCE_TYPE_CONNECTABLE_SCANABLE = 0x03,   /*!< 可连接可扫描 */
    XF_SLE_ANNOUNCE_TYPE_CONNECTABLE_DIRECTED = 0x07,   /*!< 可连接可扫描定向 */
} xf_sle_announce_type_t;

/**
 * @brief SLE 广播 (公开) 可发现等级 (被对方)
 */
typedef enum {
    XF_SLE_ANNOUNCE_LEVEL_NONE,     /*!< 不可见发现，预留 */
    XF_SLE_ANNOUNCE_LEVEL_NORMAL,   /*!< 一般可发现 */
    XF_SLE_ANNOUNCE_LEVEL_PRIORITY, /*!< 优先可发现，预留 */
    XF_SLE_ANNOUNCE_LEVEL_PAIRED,   /*!< 被曾配对过的设备发现，预留 */
    XF_SLE_ANNOUNCE_LEVEL_SPECIAL,  /*!< 被指定设备发现 */
} xf_sle_announce_level_t;

/**
 * @brief SLE 广播 (公开) 通道
 */
typedef enum {
    XF_SLE_ADV_CHANNEL_MAP_77       = 0x01,     /*!< 启用 77 通道 */
    XF_SLE_ADV_CHANNEL_MAP_78       = 0x02,     /*!< 启用 78 通道 */
    XF_SLE_ADV_CHANNEL_MAP_79       = 0x04,     /*!< 启用 79 通道 */
    XF_SLE_ADV_CHANNEL_MAP_DEFAULT  = 0x07      /*!< 启用所有通道 */
} xf_sle_adv_channel_map_t;

/**
 * @brief SLE G/T 角色协商指示
 */
typedef enum {
    XF_SLE_ANNOUNCE_ROLE_T_CAN_NEGO = 0, /*!< 期望做 T 可协商 */
    XF_SLE_ANNOUNCE_ROLE_G_CAN_NEGO,     /*!< 期望做 G 可协商 */
    XF_SLE_ANNOUNCE_ROLE_T_NO_NEGO,      /*!< 期望做 T 不可协商 */
    XF_SLE_ANNOUNCE_ROLE_G_NO_NEGO       /*!< 期望做 G 不可协商 */
} xf_sle_announce_gt_role_t;

/**
 * @brief SLE 广播 (公开) 参数
 */
typedef struct {
    uint32_t announce_interval_min;         /*!< 最小设备广播间隔, 0x000020~0xffffff, 单位125us */
    uint32_t announce_interval_max;         /*!< 最大设备广播间隔, 0x000020~0xffffff, 单位125us */
    xf_sle_announce_type_t  announce_type;  /*!< 广播类型，见 @ref xf_sle_announce_type_t */
    xf_sle_addr_t own_addr;                 /*!< 本端地址，见 @ref xf_sle_addr_t */
    xf_sle_addr_t peer_addr;                /*!< 对端地址，见 @ref xf_sle_addr_t */
    xf_sle_adv_channel_map_t  announce_channel_map;
    /*!< 广播通道，见 @ref xf_sle_adv_channel_map_t */
    int8_t   announce_tx_power;             /*!< 广播发射功率，单位dbm, 取值范围[-127, 20], 0x7F：不设置特定发送功率*/
    uint8_t announce_handle;                /*!< 广播 (公开) 句柄，取值范围[0, 0xFF] */
    xf_sle_announce_gt_role_t announce_gt_role; /*!< G/T 角色协商指示，见 @ref xf_sle_announce_gt_role_t */
    xf_sle_announce_level_t announce_level; /*!< 发现等级，见 @ref xf_sle_announce_level_t */
    uint16_t conn_interval_min;             /*!< 连接间隔最小取值，取值范围[0x001E,0x3E80]，
                                             * announce_gt_role 为 SLE_ANNOUNCE_ROLE_T_NO_NEGO 时无需配置  */
    uint16_t conn_interval_max;             /*!< 连接间隔最大取值，取值范围[0x001E,0x3E80]，
                                             * announce_gt_role 为 SLE_ANNOUNCE_ROLE_T_NO_NEGO 时无需配置  */
    uint16_t conn_max_latency;              /*!< 最大休眠连接间隔，取值范围[0x0000,0x01F3]，
                                             * announce_gt_role 为 SLE_ANNOUNCE_ROLE_T_NO_NEGO 时无需配置  */
    uint16_t conn_supervision_timeout;      /*!< 最大超时时间，取值范围[0x000A,0x0C80]，
                                             * announce_gt_role 为 SLE_ANNOUNCE_ROLE_T_NO_NEGO 时无需配置  */
    void *ext_param;                        /*!< 扩展设备公开参数, 缺省时置空 */
} xf_sle_announce_param_t;

/**
 * @brief 扫描 (发现， seek) 过滤类型
 */
typedef enum {
    XF_SLE_SEEK_FILTER_ALLOW_ALL   = 0x00,          /*!< 允许来自任何人的设备发现数据包 */
    XF_SLE_SEEK_FILTER_ALLOW_WHITE_LIST  = 0x01,    /*!< 允许来自白名单设备的设备发现数据包，预留 */
} xf_sle_seek_filter_t;

/**
 * @brief 扫描 (发现， seek) PHY 类型
 */
typedef enum {
    XF_SLE_SEEK_PHY_1M = 0x1,           /*!< 1M PHY */
    XF_SLE_SEEK_PHY_2M = 0x2,           /*!< 2M PHY */
    XF_SLE_SEEK_PHY_4M = 0x4,           /*!< 4M PHY */
} xf_sle_seek_phy_t;

/**
 * @brief 扫描 (发现， seek) 类型
 */
typedef enum {
    XF_SLE_SEEK_TYPE_PASSIVE = 0x00,    /*!< 被动扫描 */
    XF_SLE_SEEK_TYPE_ACTIVE  = 0x01,    /*!< 主动扫描 */
} xf_sle_seek_type_t;

/**
 * @brief 扫描 (发现， seek) PHY 最大值
 */
#define XF_SLE_SEEK_PHY_NUM_MAX 3       // FIXME 这个不知道是不是通用值 感觉与平台相关

/**
 * @brief 扫描 (发现， seek) 参数
 */
typedef struct {
    xf_sle_addr_type_t own_addr_type;           /*!< 对端地址，见 @ref xf_sle_addr_t */
    bool filter_duplicates;                     /*!< 重复过滤开关，0：关闭，1：开启 */
    xf_sle_seek_filter_t seek_filter_policy;    /*!< 扫描过滤类型，见 @ref xf_sle_seek_filter_t */
    xf_sle_seek_phy_t seek_phy;                 /*!< 扫描 PHY 类型，见 @ref xf_sle_seek_phy_t */

    struct {
        xf_sle_seek_type_t seek_type;           /*!< 扫描类型，见 @ref xf_sle_seek_type_t */
        uint16_t seek_interval;                 /*!< 扫描间隔，取值范围[0x0004, 0xFFFF]，time = N * 0.125ms */
        uint16_t seek_window;                   /*!< 扫描窗口，取值范围[0x0004, 0xFFFF]，time = N * 0.125ms */
    } phy_param_set[XF_SLE_SEEK_PHY_NUM_MAX];   /*!< 扫描 PHY 参数集合 */
} xf_sle_seek_param_t;

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

#endif /* __XF_SLE_DEVICE_DISCOVERY_TYPES_H__ */
