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

#if 1//XF_BLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

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


/* ==================== [Typedefs] ========================================== */

/**
 * ble地址类型有2大类：
 *  1、公共设备地址（类似USB VID，需申请）
 *  2、随机设备地址：设备启动后随机生成
 *      2.A、静态设备地址：在启动后地址不变，下次复位后地址可能会变（非强制要求）
 *      2.B、私有设备地址：地址会更新
 *          2.B.1、不可解析私有地址：地址定时更新
 *          2.B.2、可解析私有地址：地址加密生成
 */
typedef enum {
    XF_BT_ADDR_TYPE_PUBLIC_DEV          =  0x00, /*!< @if Eng bluetooth public address.
                                                                 @else   蓝牙公有地址。 @endif */
    XF_BT_ADDR_TYPE_RANDOM_DEV          =  0x01, /*!< @if Eng bluetooth random address.
                                                                 @else   蓝牙随机地址。 @endif */
    XF_BT_ADDR_TYPE_RPA_PUBLIC    = 0x02,     /*!< Resolvable Private Address (RPA) with public identity address */
    XF_BT_ADDR_TYPE_RPA_RANDOM    = 0x03,     /*!< Resolvable Private Address (RPA) with random identity address. To set this address, use the function esp_ble_gap_set_rand_addr(esp_bd_addr_t rand_addr) */

}xf_bt_dev_addr_type_t;

/// Bluetooth device address length 蓝牙设备的地址长度。
#define XF_BT_DEV_ADDR_LEN     6

/**
 * @brief 蓝牙设备地址
 * 
 */
typedef struct {
    uint8_t addr[XF_BT_DEV_ADDR_LEN];     /*!< @if Eng bluetooth device address.
                                           *   @else   蓝牙地址。 @endif */
    xf_bt_dev_addr_type_t type;           /*!< @if Eng bluetooth device address type, See { @ref xf_bt_dev_addr_type_t }.
                                           *   @else   蓝牙地址类型，参考 { @ref xf_bt_dev_addr_type_t }。 @endif */
}xf_bt_dev_addr_t;

// #define ARRAY_EXPAND_TO_ARG(array, element_num,)  
#define XF_BT_ADDR_PRINT_FMT  "%02X:%02X:%02X:%02X:%02X:%02X"
#define XF_BT_ADDR_EXPAND_TO_ARG(addr)  (addr)[0],(addr)[1],(addr)[2],(addr)[3],(addr)[4],(addr)[5]

typedef enum{
    XF_BLE_ADV_STRUCT_TYPE_FLAGS                   = 0x01,
    XF_BLE_ADV_STRUCT_TYPE_LOCAL_NAME_SHORT        = 0x08,
    XF_BLE_ADV_STRUCT_TYPE_LOCAL_NAME_ALL          = 0x09,
    XF_BLE_ADV_STRUCT_TYPE_TX_POWER_LEVEL          = 0x0A,
    XF_BLE_ADV_STRUCT_TYPE_CLASS_OF_DEVICE         = 0x0D,

    XF_BLE_ADV_STRUCT_TYPE_DEVICE_ID               = 0x10,

    XF_BLE_ADV_STRUCT_TYPE_APPEARANCE              = 0x19,
}xf_ble_gap_adv_struct_type_t;

#define XF_BLE_GAP_ADV_STRUCT_AD_TYPE_SIZE   1
#define XF_BLE_GAP_ADV_STRUCT_LEN_SIZE      1

typedef union _xf_ble_var_uintptr_t
{
    uintptr_t   _untyped;
    bool        val_bool;
    uint8_t     val_u8;
    uint16_t    val_u16;
    uint32_t    val_u32;

    uint8_t     *ptr_u8;
    uint16_t    *ptr_u16;
    uint32_t    *ptr_u32;

    uint8_t     array_u8[sizeof(uintptr_t)];
    uint16_t    array_u16[sizeof(uintptr_t)/sizeof(uint16_t)];
    uint32_t    array_u32[sizeof(uintptr_t)/sizeof(uint32_t)];
}xf_ble_var_uintptr_t;

typedef union _xf_ble_gap_adv_struct_data_t
{
    xf_ble_var_uintptr_t adv_var;   // 通用类型（以下未暂未囊括的类型可以使用该通用类型）

    uint8_t flag;           // 0x01, type:flag
    uint8_t *local_name;    // 0x08, type:name short; 0x09, type:name all
    xf_ble_appearance_t appearance;   // 0x19, type:appearance
}xf_ble_gap_adv_struct_data_t;

/**
 * @brief BLE GAP 广播 AD Structure 信息（并非严格广播包结构）
 */
typedef struct {
    uint8_t ad_data_len;   /*!< 注意这不是标准中 AD structure 长度，
                                仅仅是 AD data 的长度（不包含 AD type）*/
    xf_ble_gap_adv_struct_type_t ad_type;
    xf_ble_gap_adv_struct_data_t ad_data;
}xf_ble_gap_adv_struct_t;

/**
 * @brief 
 * 
 *  以下如 struct 仅留着用于提示及理解 BLE 标准 ADV structure 的组成
 *  
 */
#define XF_BLE_ADV_STRUCT_TYPE_ARRAY(type_name, adv_data_array_size)   \
typedef struct {                                                \
    uint8_t struct_data_len;                                    \
    struct  /* struct_data */                                   \
    {                                                           \
        union                                                   \
        {                                                       \
            xf_ble_gap_adv_struct_type_t ad_type;                   \
            uint8_t _fixed_size;                                \
        };                                                      \
        struct  /* adv_data */                                  \
        {                                                       \
            uint8_t ad_data[adv_data_array_size];              \
        };                                                      \
    };                                                          \
}type_name  

#define XF_BLE_ADV_STRUCT_TYPE_VAL_U8(type_name)            \
typedef struct {                                                \
    uint8_t struct_data_len;                                    \
    struct  /* struct_data */                                   \
    {                                                           \
        union                                                   \
        {                                                       \
            xf_ble_gap_adv_struct_type_t ad_type;                   \
            uint8_t _fixed_size;                                \
        };                                                      \
        struct  /* adv_data */                                  \
        {                                                       \
            uint8_t val;                                        \
        };                                                      \
    };                                                          \
}type_name  

#define XF_BLE_ADV_STRUCT_TYPE_VAL_U16(type_name)            \
typedef struct {                                                \
    uint8_t struct_data_len;                                    \
    struct  /* struct_data */                                   \
    {                                                           \
        union                                                   \
        {                                                       \
            xf_ble_gap_adv_struct_type_t ad_type;                   \
            uint8_t _fixed_size;                                \
        };                                                      \
        struct  /* adv_data */                                  \
        {                                                       \
            uint16_t val;                                        \
        };                                                      \
    };                                                          \
}type_name 

/**
 * @brief BLE 广播数据。
 */
typedef struct {
    xf_ble_gap_adv_struct_t *adv_struct_set; // adv 的 ad structure

    uint16_t scan_rsp_length; /*!< @if Eng Length of scan response data
                                   @else   扫描响应数据长度 @endif */
    uint8_t *scan_rsp_data;   /*!< @if Eng Scan response data
                                   @else   扫描响应数据 @endif */
}xf_ble_gap_adv_data_t;
/* TODO adv data 与 adv resp data 分离（包括api） */

/**
 * @brief 广播类型。
 */
typedef enum {
    XF_BLE_ADV_TYPE_CONN_SCAN_UNDIR = 0,        /*!<    
                                                        @if Eng 
                                                            Connectable, scanable, undirected advertising(default).
                                                        @else
                                                            可连接，可扫描，非定向 广播(默认)。 (ADV_IND) 
                                                        @endif */
    XF_BLE_ADV_TYPE_CONN_NONSCAN_DIR,           /*!< 
                                                        @if Eng 
                                                            Connectable, nonscanble, high duty directed advertising.
                                                        @else
                                                            可连接，不可扫描，定向 广播。（高频）(ADV_DIRECT_IND, high duty cycle)
                                                        @endif */
    XF_BLE_ADV_TYPE_NONCONN_SCAN_UNDIR,         /*!< 
                                                        @if Eng 
                                                            Nonconnectable, scanable, undirected advertising.
                                                        @else  
                                                            不可连接，可扫描，非定向 广播。(ADV_SCAN_IND) 
                                                        @endif */
    XF_BLE_ADV_TYPE_NONCONN_NONSCAN_UNDIR,      /*!< 
                                                        @if Eng
                                                            Nonconnectable, nonscanable, undirected advertising.
                                                        @else
                                                            不可连接，不可扫描，非定向 广播。(ADV_NONCONN_IND)
                                                        @endif */
    XF_BLE_ADV_TYPE_CONN_NONSCAN_DIR_LOW_DUTY,  /*!<
                                                        @if Eng 
                                                            Connectable, nonscanble, low duty directed advertising.
                                                        @else
                                                            可连接，不可扫描，定向 广播。（低频）(ADV_DIRECT_IND, low duty cycle)    
                                                        @endif */
}xf_ble_adv_type_t;

/**
 * @brief 广播过滤策略。
 */
typedef enum {
    XF_BLE_GAP_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY = 0x00,   /*!< @if Eng Accepts all scan and connect requests
                                                             @else   处理所有设备的扫描和连接请求 @endif */
    XF_BLE_GAP_ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY = 0x01,  /*!< @if Eng Accepts all connect but white list scan requests
                                                             @else   处理所有连接请求，仅处理白名单的扫描请求 @endif */
    XF_BLE_GAP_ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST = 0x02,  /*!< @if Eng Accepts all scan but white list connect requests
                                                             @else   处理所有扫描请求，仅处理白名单的连接请求 @endif */
    XF_BLE_GAP_ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST = 0x03, /*!< @if Eng Accepts only white list connect and scan requests
                                                             @else   仅处理白名单中扫描请求和连接请求 @endif */
}xf_ble_gap_adv_filter_policy_t;


/**
 * @brief BLE 广播通道
 * [0x01, 0x07] 
 * 0x01----使用37通道 
 * 0x07----使用37/38/39三个通道
 */
typedef enum {
    XF_BLE_GAP_ADV_CH_37     = 0x01,
    XF_BLE_GAP_ADV_CH_38     = 0x02,
    XF_BLE_GAP_ADV_CH_39     = 0x04,
    XF_BLE_GAP_ADV_CH_ALL    = 0x07,
}xf_ble_gap_adv_channel_t;

/**
 * @brief BLE PHY类型。
 */
typedef enum {
    XF_BLE_GAP_PHY_NO_PACKET = 0x00, /*!< @if Eng No packet
                                       @else   无数据包 @endif */
    XF_BLE_GAP_PHY_1M = 0x01,        /*!< @if Eng 1M PHY
                                       @else   1M PHY @endif */
    XF_BLE_GAP_PHY_2M = 0x02,        /*!< @if Eng 2M PHY
                                       @else   2M PHY @endif */
    XF_BLE_GAP_PHY_CODED = 0x03      /*!< @if Eng Coded PHY
                                       @else   Coded PHY @endif */
}xf_ble_gap_phy_type_t;

/**
 * @brief BLE广播参数。
 */
typedef struct {
    /* FIXME 加上 ADV前缀？ 参考SLE */
    uint32_t min_interval;          /*!<@if Eng - Min interval[N * 0.625ms]
                                        @else   - 最小的广播间隔[N * 0.625ms]
                                        @endif */
    uint32_t max_interval;          /*!<@if Eng - Max interval[N * 0.625ms]
                                        @else   - 最大的广播间隔[N * 0.625ms]
                                        @endif */
    xf_ble_adv_type_t adv_type; /*!<@if Eng - Advertising type
                                        @else   - 广播类型
                                        @endif */
    xf_bt_dev_addr_t own_addr;      /*!<@if Eng - own address
                                        @else   - 本端地址
                                        @endif */
    xf_bt_dev_addr_t peer_addr;     /*!<@if Eng - Peer address
                                        @else   - 对端地址 @endif */
    xf_ble_gap_adv_channel_t channel_map;
                                    /*!<@if Eng - channel bitmap
                                        @else   - 广播通道选择
                                        @endif */
    xf_ble_gap_adv_filter_policy_t adv_filter_policy;      
                                    /*!<@if Eng - Advertising filter policy
                                        @else   - 白名单过滤策略
                                        @endif */
    uint32_t tx_power;              /*!<@if Eng - Transmissive power
                                        @else   - 发送功率,单位dbm,范围-127~20 @endif */
    // FRQ 在起广播时才传入 IDF 在 esp_ble_gap_ext_adv_start 才有传入
    uint32_t duration;              /*!< @if Eng Duration
                                        @else   - 广播持续发送时
                                        @endif*/
}xf_ble_gap_adv_param_t;

/**
 * @brief BLE扫描过滤策略。
 */
typedef enum {
    XF_BLE_GAP_SCAN_FILTER_ACCEPT_ALL = 0x00,       /*!< @if Eng Accept all advertising packets except directed
                                                                     advertising packets not addressed to this device
                                                                     (default)
                                                             @else   接收所有广播不接收目标地址不是本设备地址的定向广播
                                                                    （默认）
                                                             @endif */
    XF_BLE_GAP_SCAN_FILTER_ONLY_WHITE_LIST,         /*!< @if Eng Accept only advertisement packets from white list
                                                                     devices. Directed advertising packets which are
                                                                     not addressed for this device shall be ignored
                                                             @else   只接收白名单里设备的广播，
                                                                     不接收目标地址不是本设备地址的定向广播 @endif */
    XF_BLE_GAP_SCAN_FILTER_ACCEPT_ALL_AND_RPA,      /*!< @if Eng Accept all undirected advertisement packets, and
                                                                     all directed advertising packets where the
                                                                     initiator address is a resolvable private address,
                                                                     and all directed advertising packets addressed to
                                                                     this device
                                                             @else   接收所有的非定向广播、
                                                                     地址是可解析私有地址的广播方发送的定向广播、
                                                                     发给该设备的定向广播 @endif */
    XF_BLE_GAP_SCAN_FILTER_ONLY_WHITE_LIST_AND_RPA, /*!< @if Eng Accept all undirected advertisement packets from
                                                                     devices where the advertiser's address is in the
                                                                     White list, and all directed advertising packets
                                                                     where the initiator address is a resolvable
                                                                     private address, and all directed advertising
                                                                     packets addressed to this device
                                                             @else   接收白名单中的所有非定向广播、
                                                                     地址是可解析私有地址的广播方发送的定向广播、
                                                                     发给该设备的定向广播 @endif */
}xf_ble_gap_scan_filter_t;

/// BLE scan type
typedef enum {
    XF_BLE_GAP_SCAN_TYPE_PASSIVE   =   0x0,            /*!< Passive scan */
    XF_BLE_GAP_SCAN_TYPE_ACTIVE    =   0x1,            /*!< Active scan */
}xf_ble_gap_scan_type_t;

/**
 * @brief BLE扫描参数。
 */
typedef struct {
    xf_ble_gap_phy_type_t scan_phy; 
                                /*!< @if Eng PHY type
                                     @else   PHY类型 @endif */
    xf_ble_gap_scan_filter_t scan_filter_policy;
                                /*!< @if Eng Scan fileter policy
                                     @else   扫描过滤策略 @endif */

    xf_ble_gap_scan_type_t scan_type;          
                                /*!< @if Eng Scan type
                                     @else   扫描类型 @endif */
    uint16_t scan_interval;     /*!< @if Eng Scan interval[N * 0.625ms]
                                     @else   扫描间隔，[N * 0.625 ms] @endif */
    uint16_t scan_window;       /*!< @if Eng Scan window[N * 0.625ms]
                                     @else   扫描窗长，[N * 0.625 ms] @endif */
}xf_ble_gap_scan_param_t;

/**
 * @brief ble连接参数更新数据结构。
 */
typedef struct {
    // IDF: esp_bd_addr_t bda;  FRQ: conidx
    uint16_t conn_handle;        /*!< 
                                  * @if Eng connection id
                                  * @else   连接 ID @endif */
    uint16_t min_interval;       /*!< 
                                  * @if Eng min connection  interval
                                  * @else   最小连接间隔 
                                  *     [N * 0.625 ms]
                                  *     0xFFFF：表示没有特定值
                                  *     // 范围：[0x0006,0x0C80]（看具体标准）
                                  * @endif */
    uint16_t max_interval;       /*!< 
                                  * @if Eng max connection  interval
                                  * @else   最大连接间隔
                                  *     [N * 0.625 ms]
                                  *     0xFFFF：表示没有特定值
                                  *     // 范围：[0x0006,0x0C80]（看具体标准）
                                  * @endif */
    uint16_t slave_latency;      /*!< 
                                  * @if Eng slave reply min latency
                                  * @else 从机的连接的延迟（延迟响应）（以连接事件数为单位）。 
                                  *     // 范围：[0x0000,0x01F3]（看具体标准）
                                  *     Slave Latency：允许Slave（从设备）在没有数据要发的情况下，
                                  *     跳过一定数目的连接事件，在这些连接事件中不必回复Master（主设备）的包。      
                                  * @endif */
    uint16_t timeout;           /*!< 
                                 * @if Eng interval for disconnection due to timeout
                                 * @else 连接管理超时（将进行断连）时间 
                                 *     [N * 10 ms]
                                 *     0xFFFF：表示没有特定值
                                 *     // 范围：[0x000A,0x0C80]（看具体标准）
                                 * @endif */
}xf_ble_gap_conn_param_update_t;

/**
 * @brief 蓝牙断链原因。
 * 其他错误码值参见：具体平台或
 * “BLUETOOTH CORE SPECIFICATION Version 5.4 | Vol 1, Part F, 1.3 LIST OF ERROR CODES”
 */
typedef enum {
    XF_BLE_GAP_DISCONNECT_UNKNOWN              = 0x00,    /*!< @if Eng disconnect by local
                                                      @else   未知原因断链 @endif */
    XF_BLE_GAP_DISCONNECT_TIMEOUT              = 0x8,     /*!< @if Eng disconnect by local
                                                      @else   连接超时断链 @endif */
    XF_BLE_GAP_DISCONNECT_ENDED_BY_REMOTE_USER = 0x13,    /*!< @if Eng disconnect by remote
                                                      @else   远端用户断链 @endif */
    XF_BLE_GAP_DISCONNECT_ENDED_BY_LOCAL_HOST  = 0x16,    /*!< @if Eng disconnect by remote
                                                         @else   本端HOST断链 @endif */
}xf_ble_gap_disconnect_reason_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GAP_TYPES_H__ */
