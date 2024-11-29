/**
 * @file xf_ble_gap_types.h
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-08-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_BLE_GAP_TYPES_H__
#define __XF_BLE_GAP_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#include "xf_ble_types.h"

#if XF_BLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief   BLE 外观值，完全遵循蓝牙标准进行定义
 *
 * @note 这里仅列出部分常用的值，更多可选值参见蓝牙官方文档
 *  《Assigned Numbers》 >| 2.6 Appearance Values
 * @ref https://www.bluetooth.com/specifications/assigned-numbers/
 */
typedef uint16_t xf_ble_appearance_t;

#define XF_BLE_APPEARANCE_UNKNOWN                               0x0000
#define XF_BLE_APPEARANCE_GENERIC_PHONE                         0x0040
#define XF_BLE_APPEARANCE_GENERIC_COMPUTER                      0x0080
#define XF_BLE_APPEARANCE_GENERIC_WATCH                         0x00C0
#define XF_BLE_APPEARANCE_SPORTS_WATCH                          0x00C1
#define XF_BLE_APPEARANCE_GENERIC_CLOCK                         0x0100
#define XF_BLE_APPEARANCE_GENERIC_DISPLAY                       0x0140
#define XF_BLE_APPEARANCE_GENERIC_REMOTE                        0x0180
#define XF_BLE_APPEARANCE_GENERIC_EYEGLASSES                    0x01C0
#define XF_BLE_APPEARANCE_GENERIC_TAG                           0x0200
#define XF_BLE_APPEARANCE_GENERIC_KEYRING                       0x0240
#define XF_BLE_APPEARANCE_GENERIC_MEDIA_PLAYER                  0x0280
#define XF_BLE_APPEARANCE_GENERIC_BARCODE_SCANNER               0x02C0
#define XF_BLE_APPEARANCE_GENERIC_THERMOMETER                   0x0300
#define XF_BLE_APPEARANCE_THERMOMETER_EAR                       0x0301
#define XF_BLE_APPEARANCE_GENERIC_HEART_RATE                    0x0340
#define XF_BLE_APPEARANCE_HEART_RATE_BELT                       0x0341
#define XF_BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE                0x0380
#define XF_BLE_APPEARANCE_BLOOD_PRESSURE_ARM                    0x0381
#define XF_BLE_APPEARANCE_BLOOD_PRESSURE_WRIST                  0x0382
#define XF_BLE_APPEARANCE_GENERIC_HID                           0x03C0
#define XF_BLE_APPEARANCE_HID_KEYBOARD                          0x03C1
#define XF_BLE_APPEARANCE_HID_MOUSE                             0x03C2
#define XF_BLE_APPEARANCE_HID_JOYSTICK                          0x03C3
#define XF_BLE_APPEARANCE_HID_GAMEPAD                           0x03C4
#define XF_BLE_APPEARANCE_HID_DIGITIZER_TABLET                  0x03C5
#define XF_BLE_APPEARANCE_HID_CARD_READER                       0x03C6
#define XF_BLE_APPEARANCE_HID_DIGITAL_PEN                       0x03C7
#define XF_BLE_APPEARANCE_HID_BARCODE_SCANNER                   0x03C8
#define XF_BLE_APPEARANCE_GENERIC_GLUCOSE                       0x0400
#define XF_BLE_APPEARANCE_GENERIC_WALKING                       0x0440
#define XF_BLE_APPEARANCE_WALKING_IN_SHOE                       0x0441
#define XF_BLE_APPEARANCE_WALKING_ON_SHOE                       0x0442
#define XF_BLE_APPEARANCE_WALKING_ON_HIP                        0x0443
#define XF_BLE_APPEARANCE_GENERIC_CYCLING                       0x0480
#define XF_BLE_APPEARANCE_CYCLING_COMPUTER                      0x0481
#define XF_BLE_APPEARANCE_CYCLING_SPEED                         0x0482
#define XF_BLE_APPEARANCE_CYCLING_CADENCE                       0x0483
#define XF_BLE_APPEARANCE_CYCLING_POWER                         0x0484
#define XF_BLE_APPEARANCE_CYCLING_SPEED_CADENCE                 0x0485
#define XF_BLE_APPEARANCE_GENERIC_PULSE_OXIMETER                0x0C40
#define XF_BLE_APPEARANCE_PULSE_OXIMETER_FINGERTIP              0x0C41
#define XF_BLE_APPEARANCE_PULSE_OXIMETER_WRIST                  0x0C42
#define XF_BLE_APPEARANCE_GENERIC_WEIGHT                        0x0C80
#define XF_BLE_APPEARANCE_GENERIC_PERSONAL_MOBILITY_DEVICE      0x0CC0
#define XF_BLE_APPEARANCE_POWERED_WHEELCHAIR                    0x0CC1
#define XF_BLE_APPEARANCE_MOBILITY_SCOOTER                      0x0CC2
#define XF_BLE_APPEARANCE_GENERIC_CONTINUOUS_GLUCOSE_MONITOR    0x0D00
#define XF_BLE_APPEARANCE_GENERIC_INSULIN_PUMP                  0x0D40
#define XF_BLE_APPEARANCE_INSULIN_PUMP_DURABLE_PUMP             0x0D41
#define XF_BLE_APPEARANCE_INSULIN_PUMP_PATCH_PUMP               0x0D44
#define XF_BLE_APPEARANCE_INSULIN_PEN                           0x0D48
#define XF_BLE_APPEARANCE_GENERIC_MEDICATION_DELIVERY           0x0D80
#define XF_BLE_APPEARANCE_GENERIC_OUTDOOR_SPORTS                0x1440
#define XF_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION               0x1441
#define XF_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_AND_NAV       0x1442
#define XF_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD           0x1443
#define XF_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD_AND_NAV   0x1444

/**
 * @brief BLE GAP 链接角色类型
 */
typedef enum {
    XF_BLE_GAP_LINK_ROLE_MASTER = 0,        /*!< 主机 */
    XF_BLE_GAP_LINK_ROLE_SLAVE  = 1         /*!< 从机 */
} xf_ble_gap_link_role_type_t;

/**
 * @brief BLE 广播数据单元 ( AD Structure ) 类型，完全遵循蓝牙标准进行定义
 *
 * @note  这里仅列出部分常用的类型，更多可选类型参见蓝牙官方文档
 *  《Assigned Numbers》 > 2.3 Common Data Types
 * @ref https://www.bluetooth.com/specifications/assigned-numbers/
 */
typedef enum {
    XF_BLE_ADV_STRUCT_TYPE_FLAGS                   = 0x01,
    XF_BLE_ADV_STRUCT_TYPE_LOCAL_NAME_SHORT        = 0x08,
    XF_BLE_ADV_STRUCT_TYPE_LOCAL_NAME_ALL          = 0x09,
    XF_BLE_ADV_STRUCT_TYPE_TX_POWER_LEVEL          = 0x0A,
    XF_BLE_ADV_STRUCT_TYPE_CLASS_OF_DEVICE         = 0x0D,
    XF_BLE_ADV_STRUCT_TYPE_DEVICE_ID               = 0x10,
    XF_BLE_ADV_STRUCT_TYPE_APPEARANCE              = 0x19,
} xf_ble_gap_adv_struct_type_t;

/**
 * @brief BLE GAP 广播数据单元类型 (AD_TYPE) 字段的大小
 */
#define XF_BLE_GAP_ADV_STRUCT_AD_TYPE_FIELD_SIZE  1
/**
 * @brief BLE GAP 广播数据单元数据长度 (LEN) 字段的大小
 */
#define XF_BLE_GAP_ADV_STRUCT_LEN_FIELD_SIZE      1

/**
 * @brief BLE GAP 广播数据单元的数据 ( AD Data )
 *
 * @note 以下暂时仅列出部分类型的广播数据单元数据成员
 * @details 以下为蓝牙标准定义的广播数据结构及广播数据单元数据 ( AD Data ) 所在的位置
 * @code
 *  | AdvData                                                                                                   |
 *  | AD Structure 1                            | AD Structure 2                | ... |（无效数据 000...000b）   |
 *  | Length              | Data                | Length | Data                 | ......                        |
 *  | Length(type + data) | AD type | AD Data   | Length | AD type | AD Data    | ......                        |
 *                                  |    ^      |
 * @endcode
 */
typedef union _xf_ble_gap_adv_struct_data_t {
    xf_ble_var_uintptr_t adv_var;       /*!< 类型不定的广播数据单元数据（暂不建议使用）*/
    uint8_t flag;                       /*!< 类型为: 0x01, type:flag */
    uint8_t *local_name;                /*!< 类型为: 0x08, type:name short; 0x09, type:name all */
    xf_ble_appearance_t appearance;     /*!< 类型为: 0x19, type:appearance，见 @ref xf_ble_appearance_t */
} xf_ble_gap_adv_struct_data_t;

/**
 * @brief BLE GAP 广播数据单元 ( AD structure ）
 *
 * @warning 这里的内存空间结构及成员并非严格按蓝牙标准定义的广播数据单元结构进行定义
 * @details 以下为蓝牙标准定义的广播数据结构及广播数据单元 ( AD structure ）所在的位置
 * @code
 *  | AdvData                                                                                                   |
 *  | AD Structure 1                            | AD Structure 2                | ... |（无效数据 000...000b）   |
 *  | Length              | Data                | Length | Data                 | ......                        |
 *  | Length(type + data) | AD type | AD Data   | Length | AD type | AD Data    | ......                        |
 *  |               ^                           |
 * @endcode
 */
typedef struct {
    uint8_t ad_data_len;        /*!< 注意，这并不是蓝牙标准中广播数据结构 AD structure 长度（ Length ），
                                 * 仅是 AD data 字段的长度（不包含 AD type 字段的长度 ）*/
    xf_ble_gap_adv_struct_type_t ad_type;   /*!< 广播数据单元的类型，见 @ref xf_ble_gap_adv_struct_type_t */
    xf_ble_gap_adv_struct_data_t ad_data;   /*!< 广播数据单元的数据，见 @ref xf_ble_gap_adv_struct_data_t */
} xf_ble_gap_adv_struct_t;

/**
 * @brief 定义一个严格遵循蓝牙标准的广播数据单元结构，单元数据 ( AD Data ) 为数组的类型
 *
 * @param type_name 指定定义的类型名
 * @param adv_data_array_size 单元数据 ( AD Data ) 数组的大小
 * @note 一般仅用于平台对接时使用，便于 XF BLE 广播数据单元结构与符号标准的广播数据结构间的转换
 */
#define XF_BLE_ADV_STRUCT_TYPE_ARRAY(type_name, adv_data_array_size)   \
typedef struct {                                                \
    uint8_t struct_data_len;                                    \
    struct  /* struct_data */                                   \
    {                                                           \
        union                                                   \
        {                                                       \
            xf_ble_gap_adv_struct_type_t ad_type;               \
            uint8_t _fixed_size;                                \
        };                                                      \
        struct  /* adv_data */                                  \
        {                                                       \
            uint8_t ad_data[adv_data_array_size];               \
        };                                                      \
    };                                                          \
}type_name

/**
 * @brief 定义一个严格遵循蓝牙标准的广播数据单元结构，单元数据 ( AD Data ) 为 uint8_t 的类型
 *
 * @param type_name 指定定义的类型名
 * @note 一般仅用于平台对接时使用，便于 XF BLE 广播数据单元结构与符号标准的广播数据结构间的转换
 */
#define XF_BLE_ADV_STRUCT_TYPE_VAL_U8(type_name)                \
typedef struct {                                                \
    uint8_t struct_data_len;                                    \
    struct  /* struct_data */                                   \
    {                                                           \
        union                                                   \
        {                                                       \
            xf_ble_gap_adv_struct_type_t ad_type;               \
            uint8_t _fixed_size;                                \
        };                                                      \
        struct  /* adv_data */                                  \
        {                                                       \
            uint8_t val;                                        \
        };                                                      \
    };                                                          \
}type_name

/**
 * @brief 定义一个严格遵循蓝牙标准的广播数据单元结构，单元数据 ( AD Data ) 为 uint16_t 的类型
 *
 * @param type_name 指定定义的类型名
 * @note 一般仅用于平台对接时使用，便于 XF BLE 广播数据单元结构与符号标准的广播数据结构间的转换
 */
#define XF_BLE_ADV_STRUCT_TYPE_VAL_U16(type_name)               \
typedef struct {                                                \
    uint8_t struct_data_len;                                    \
    struct  /* struct_data */                                   \
    {                                                           \
        union                                                   \
        {                                                       \
            xf_ble_gap_adv_struct_type_t ad_type;               \
            uint8_t _fixed_size;                                \
        };                                                      \
        struct  /* adv_data */                                  \
        {                                                       \
            uint16_t val;                                       \
        };                                                      \
    };                                                          \
}type_name

/**
 * @brief BLE GAP 广播数据 ( 包含响应数据 )
 *
 * @warning 目前 广播数据单元 并不是严格按标准的广播数据结构进行定义，
 *  而是从更方便使用的角度对标准的结构进行了微调
 * @warning 目前 响应数据 是需严格遵循标准结构，需要自行逐项填充
 */
/* TODO adv data 与 adv resp data 分离（包括api），优化响应数据的结构 */
typedef struct {
    xf_ble_gap_adv_struct_t *adv_struct_set;    /*!< 广播数据单元（ AD Structure ）的集合，
                                                 * 见 @ref xf_ble_gap_adv_struct_t */

    uint16_t scan_rsp_length;                   /*!< 扫描响应数据长度， */
    uint8_t *scan_rsp_data;                     /*!< 扫描响应数据 */
} xf_ble_gap_adv_data_t;


/**
 * @brief BLE GAP 广播类型，完全遵循蓝牙标准进行定义
 *
 * @ref 详参蓝牙核心文档 《Core_v5.4》>| Vol 4, Part E >| 7.8.5 LE Set Advertising Parameters command
 *  >| Command parameters >| Advertising_Type
 *  在线文档: https://www.bluetooth.com/specifications/specs/core54-html/
 *  离线文档: https://www.bluetooth.com/specifications/specs/core-specification-amended-5-4/
 */
typedef enum {
    XF_BLE_GAP_ADV_TYPE_CONN_SCAN_UNDIR             = 0x00, /*!< 可连接，   可扫描，    非定向 广播 (ADV_IND) (默认) */
    XF_BLE_GAP_ADV_TYPE_CONN_NONSCAN_DIR            = 0x01, /*!< 可连接，   不可扫描，  定向 广播   (ADV_DIRECT_IND) (高频) */
    XF_BLE_GAP_ADV_TYPE_NONCONN_SCAN_UNDIR          = 0x02, /*!< 不可连接， 可扫描，    非定向 广播 (ADV_SCAN_IND) */
    XF_BLE_GAP_ADV_TYPE_NONCONN_NONSCAN_UNDIR       = 0x03, /*!< 不可连接， 不可扫描，  非定向 广播 (ADV_NONCONN_IND) */
    XF_BLE_GAP_ADV_TYPE_CONN_NONSCAN_DIR_LOW_DUTY   = 0x04, /*!< 可连接，   不可扫描，  定向 广播   (ADV_DIRECT_IND) (低频) */
} xf_ble_gap_adv_type_t;

/**
 * @brief BLE GAP 广播过滤策略，完全遵循蓝牙标准进行定义
 *
 * @ref 详参蓝牙核心文档 《Core_v5.4》>| Vol 4, Part E >| 7.8.5 LE Set Advertising Parameters command
 *  >| Command parameters >| Advertising_Filter_Policy
 *  在线文档: https://www.bluetooth.com/specifications/specs/core54-html/
 *  离线文档: https://www.bluetooth.com/specifications/specs/core-specification-amended-5-4/
 */
typedef enum {
    XF_BLE_GAP_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY    = 0x00,   /*!< 处理所有设备的扫描和连接请求 */
    XF_BLE_GAP_ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY   = 0x01,  /*!< 处理所有 连接 请求，仅处理白名单的 扫描 请求 */
    XF_BLE_GAP_ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST   = 0x02,  /*!< 处理所有 扫描 请求，仅处理白名单的 连接 请求 */
    XF_BLE_GAP_ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST  = 0x03, /*!< 仅处理白名单中扫描请求和连接请求 */
} xf_ble_gap_adv_filter_policy_t;

/**
 * @brief BLE GAP 广播通道，完全遵循蓝牙标准进行定义
 *
 * @ref 详参蓝牙核心文档 《Core_v5.4》>| Vol 4, Part E >| 7.8.5 LE Set Advertising Parameters command
 *  >| Command parameters >| Advertising_Channel_Map
 *  在线文档: https://www.bluetooth.com/specifications/specs/core54-html/
 *  离线文档: https://www.bluetooth.com/specifications/specs/core-specification-amended-5-4/
 */
typedef enum {
    XF_BLE_GAP_ADV_CH_37     = 0x01,    /*!< 启用 37 通道 */
    XF_BLE_GAP_ADV_CH_38     = 0x02,    /*!< 启用 38 通道 */
    XF_BLE_GAP_ADV_CH_39     = 0x04,    /*!< 启用 39 通道 */
    XF_BLE_GAP_ADV_CH_ALL    = 0x07,    /*!< 启用所有通道 */
} xf_ble_gap_adv_channel_t;

/**
 * @brief BLE GAP PHY 类型
 */
typedef enum {
    XF_BLE_GAP_PHY_NO_PACKET    = 0x00, /*!< 无数据包 */
    XF_BLE_GAP_PHY_1M           = 0x01, /*!< 1M PHY */
    XF_BLE_GAP_PHY_2M           = 0x02, /*!< 2M PHY */
    XF_BLE_GAP_PHY_CODED        = 0x03, /*!< Coded PHY */
} xf_ble_gap_phy_type_t;

/**
 * @brief BLE GAP 广播参数
 */
typedef struct {
    uint32_t min_interval;                  /*!< 最小的广播间隔 [N * 0.625ms] */
    uint32_t max_interval;                  /*!< 最大的广播间隔 [N * 0.625ms] */
    xf_ble_gap_adv_type_t adv_type;             /*!< 广播类型，见 @ref xf_ble_gap_adv_type_t */
    xf_ble_addr_t own_addr;              /*!< 本端地址，见 @ref xf_ble_addr_t */
    xf_ble_addr_t peer_addr;             /*!< 对端地址，见 @ref xf_ble_addr_t */
    xf_ble_gap_adv_channel_t channel_map;   /*!< 广播通道，见 @ref xf_ble_gap_adv_channel_t */
    xf_ble_gap_adv_filter_policy_t
    adv_filter_policy;   /*!< 白名单过滤策略，见 @ref xf_ble_gap_adv_filter_policy_t */
    uint32_t tx_power;                      /*!< 发送功率,单位dbm,范围-127~20 */

    /* FIXME FRQ 在起广播时才传入 IDF 在 esp_ble_gap_ext_adv_start 才有传入 */
    uint32_t duration;                      /*!< 广播持续时间 */
} xf_ble_gap_adv_param_t;

/**
 * @brief BLE GAP 扫描过滤策略，完全遵循蓝牙标准进行定义
 *
 * @ref 详参蓝牙核心文档 《Core_v5.4》
 *  1. >| Vol 6, Part B >| 4.3.3 Scanning filter policy
 *  2. >| Vol 4, Part E >| 7.8.10 LE Set Scan Parameters command >| Command parameters >| Scanning_Filter_Policy
 *  在线文档: https://www.bluetooth.com/specifications/specs/core54-html/
 *  离线文档: https://www.bluetooth.com/specifications/specs/core-specification-amended-5-4/
 */
typedef enum {
    XF_BLE_GAP_SCAN_FILTER_ACCEPT_ALL           = 0x00, /*!< 接收所有广播不接收目标地址不是本设备地址的定向广播 (默认) */
    XF_BLE_GAP_SCAN_FILTER_ONLY_WHITE_LIST      = 0x01, /*!< 只接收白名单里设备的广播，不接收目标地址不是本设备地址的定向广播 */
    XF_BLE_GAP_SCAN_FILTER_ACCEPT_ALL_AND_RPA   = 0x02, /*!< 接收所有的非定向广播、
                                                         *  地址是可解析私有地址的广播方发送的定向广播、
                                                         *  发给该设备的定向广播 */
    XF_BLE_GAP_SCAN_FILTER_ONLY_WHITE_LIST_AND_RPA = 0x03, /*!< 接收白名单中的所有非定向广播、
                                                         *  地址是可解析私有地址的广播方发送的定向广播、
                                                         *  发给该设备的定向广播 */
} xf_ble_gap_scan_filter_t;

/**
 * @brief BLE GAP 扫描类型，完全遵循蓝牙标准进行定义
 *
 * @ref 详参蓝牙核心文档 《Core_v5.4》>| Vol 4, Part E
 *  >| 7.8.10 LE Set Scan Parameters command >| Command parameters >| Scan_Type[
 *  在线文档: https://www.bluetooth.com/specifications/specs/core54-html/
 *  离线文档: https://www.bluetooth.com/specifications/specs/core-specification-amended-5-4/
 */
typedef enum {
    XF_BLE_GAP_SCAN_TYPE_PASSIVE    = 0x0,  /*!< 被动扫描 */
    XF_BLE_GAP_SCAN_TYPE_ACTIVE     = 0x1,  /*!< 主动扫描 */
} xf_ble_gap_scan_type_t;

/**
 * @brief BLE GAP 扫描参数
 */
typedef struct {
    xf_ble_gap_phy_type_t scan_phy;                 /*!< PHY类型，见 @ref xf_ble_gap_phy_type_t */
    xf_ble_gap_scan_filter_t scan_filter_policy;    /*!< 扫描过滤策略，见 @ref xf_ble_gap_scan_filter_t */
    xf_ble_gap_scan_type_t scan_type;               /*!< 扫描类型 @ref xf_ble_gap_scan_type_t */
    uint16_t scan_interval;                         /*!< 扫描间隔，[N * 0.625 ms]，
                                                     *  范围 (看具体标准)：[0x0004, 0x4000]，[2.5 ms, 10.24 s] */
    uint16_t scan_window;                           /*!< 扫描窗长，[N * 0.625 ms]，
                                                     *  范围 (看具体标准)：[0x0004, 0x4000]，[2.5 ms, 10.24 s] */
} xf_ble_gap_scan_param_t;

/**
 * @brief BLE GAP 连接参数更新数据结构
 */
typedef struct {
    uint16_t conn_handle;        /*!< 连接 ID */
    uint16_t min_interval;       /*!< 最小连接间隔，[N * 0.625 ms]，0xFFFF：表示没有特定值，
                                 *  范围：[0x0006,0x0C80] (看具体标准) */
    uint16_t max_interval;       /*!< 最大连接间隔，[N * 0.625 ms]，0xFFFF：表示没有特定值，
                                 *  范围：[0x0006,0x0C80] (看具体标准) */
    uint16_t slave_latency;     /*!< 从机链路的最小延迟（应答或响应延迟）（以连接事件数为单位)，
                                 *  范围：[0x0000,0x01F3]（看具体标准)，
                                 *  允许Slave（从设备）在没有数据要发的情况下，
                                 *  跳过一定数目的连接事件，在这些连接事件中不必回复Master（主设备）的包。
                                 */
    uint16_t timeout;           /*!< 链接超时（将进行）断连的时间，[N * 10 ms]，0xFFFF：表示没有特定值，
                                 *  范围：[0x000A,0x0C80]（看具体标准）
                                 */
} xf_ble_gap_conn_param_update_t;

/**
 * @brief 蓝牙断连原因
 *
 * @note 其他错误码值参见：具体平台说明或参考蓝牙标准
 * @ref 详参蓝牙核心文档 《Core_v5.4》>| Vol 1, Part F >| 1.3 LIST OF ERROR CODES
 *  在线文档: https://www.bluetooth.com/specifications/specs/core54-html/
 *  离线文档: https://www.bluetooth.com/specifications/specs/core-specification-amended-5-4/
 */
typedef enum {
    XF_BLE_GAP_DISCONNECT_UNKNOWN              = 0x00,    /*!< 未知原因断连 */
    XF_BLE_GAP_DISCONNECT_TIMEOUT              = 0x8,     /*!< 连接超时断连 */
    XF_BLE_GAP_DISCONNECT_ENDED_BY_REMOTE_USER = 0x13,    /*!< 远端用户断连 */
    XF_BLE_GAP_DISCONNECT_ENDED_BY_LOCAL_HOST  = 0x16,    /*!< 本端 HOST 断连 */
} xf_ble_gap_disconnect_reason_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GAP_TYPES_H__ */
