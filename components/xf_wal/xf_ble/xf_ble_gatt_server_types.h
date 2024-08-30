/**
 * @file xf_ble_gatt_server_types.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_BLE_GATT_SERVER_TYPES_H__
#define __XF_BLE_GATT_SERVER_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#include "xf_ble_types.h"
#include "xf_ble_gap_types.h"
#include "xf_ble_gatt_common.h"

#if XF_BLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief 特征描述符信息。
 *  特征描述符（不知道是不是属性）定义
 *  |标识：特征 UUID（特征声明中的）.                               【O】
 *  |权限：由高层 profile 指定（如服务）                            【O】
 */
typedef struct {
    xf_bt_attr_handle_t chara_desc_handle; /*<<<<<<<<<<<<<<<<<<<<<<<<*/    
    /**
     * 指向描述符 UUID 的指针；使用BLE_UUIDxx_DECLARE宏声明 
     * 如果服务中没有更多特征，则为NULL
     */
    xf_bt_uuid_info_t *desc_uuid; /*!< @if Eng UUID of GATT descriptor.
                                @else   GATT 描述符 UUID。 @endif */
    xf_ble_gatt_attr_permission_t permissions;    
    uint8_t *value;        // chara value Attr - 属性值
    uint16_t value_len;    // chara value Attr - 属性值长度
    xf_ble_gatt_chara_desc_type_t desc_type;
}xf_ble_gatts_chara_desc_t;


#define XF_BLE_ATTR_SET_END_FLAG    NULL

/**
 * @brief 
 *  特征定义（属性句柄和权限暂不关注）
 *  |特征声明（属性）| 特征值声明（属性）| [特征描述符声明（属性）] |
 *  
 * 
 *  特征声明（属性）定义
 *  |属性句柄                                                    【X】
 *  |属性类型：<特征>类型的属性 UUID（0x2803）（2字节）             【X】
 *  |属性值：
 *          特征特性（Properties）（1字节）                        【C】
 *      +   特征值属性句柄（2字节）                                【O】
 *      +   特征 UUID（2或16字节）                                【O】  
 *  |属性权限：已由 GATT 规定：只读，无需认证或授权                  【X】 
 * 
 *  特征值声明（属性）定义
 *  |属性句柄                                                     【X】
 *  |属性类型：特征 UUID（特征声明中的）.                           【X】
 *  |属性值：特征值                                               【C】
 *  |属性权限：由高层 profile 指定（如服务）                        【O】
 * 
 *  [特征描述符声明（属性）]
 */ 

/**
 * @brief 特征值信息。
 * 注意，这里省略掉了 特征值声明属性中的属性类型，即特征 UUID（特征声明中的），
 * 因为此类型仅包含于特征描述声明中，便于查看。
 */
typedef struct {
    // chara value Attr - chara uuid 特征声明中使用的特征值的16位蓝牙或128位UUID
    // const xf_bt_uuid_info_t *chara_uuid;  
    uint8_t *value;        // chara value Attr - value - 指向特征值
    uint16_t value_len;    // chara value Attr - value - 特征值长度
    xf_ble_gatt_attr_permission_t permission; // chara value Attr - permission
}xf_ble_gatts_chara_value_t; 

typedef struct _xf_ble_gatts_chara_t{
    /* chara declare Attr */
    xf_bt_attr_handle_t chara_handle;       /*<<<<<<<<<<<<<<<<<<<<<<<<*/    
     /**
     * 指向特征 UUID 的指针；使用 BLE_UUIDxx_DECLARE 宏声明 
     */
    xf_ble_gatt_chara_property_t properties;    // chara declare Attr - properties

    /* chara declare Attr - chara value handle 
    是包含特征值的属性的属性句柄 感觉可以去掉，因为下面就是 特征值描述
    */
    xf_bt_attr_handle_t chara_value_handle; /*<<<<<<<<<<<<<<<<<<<<<<<<*/    
    xf_bt_uuid_info_t *chara_uuid;             // chara declare Attr - chara uuid
    
    xf_ble_gatts_chara_value_t chara_value;

    /**
     * @brief 此特征的描述符数组。如果没有描述符，则为NULL。
     * 不包括CCCD；如果设置了此特性的通知或指示标志，则会自动添加它。
     */
    xf_ble_gatts_chara_desc_t *desc_set;
}xf_ble_gatts_chara_t;

/**
 * @brief 服务（属性）定义
 * @details
 * ```
 *  服务（属性）定义
 *  |属性句柄                                                    【X】
 *  |属性类型：<主要服务>或<次要服务>的属性 UUID（2字节）           【C】
 *  |属性值：特征 UUID（2或16字节）                               【C】    
 *  |属性权限：已由 GATT 规定：只读，无需认证或授权                 【X】
 * 
 * 
 *  包含服务（属性）定义
 *  |属性句柄                                                    【X】
 *  |属性类型：<包含服务>的属性 UUID（2字节）                      【X】
 *  |属性值：
 *          包含服务属性句柄                                     【X?】
 *      +   结束组句柄                                          【X?】
 *      +   服务 UUID                                           【X?】    
 *  |属性权限：已由 GATT 规定：只读，无需认证或授权                 【X】
 * ```
 */ 

typedef struct _xf_ble_gatts_service_t xf_ble_gatts_service_t;
typedef struct _xf_ble_gatts_service_t{
    xf_bt_attr_handle_t service_handle;         /*<<<<<<<<<<<<<<<<<<<<<<<<*/
  
    // 主要服务、次要服务  服务集结束标志
    xf_ble_gatt_service_type_t service_type; // attr type

    /**
     * 指向服务 UUID 的指针；使用BLE_UUIDxx_DECLARE宏声明 
     */
    xf_bt_uuid_info_t *service_uuid; // attr value


    /* 以下为官方标准上的可选项，如需则填入 */

    // 包含服务 集合（数组）
    xf_ble_gatts_service_t **include_set; // 原为 includes 但s不够明显

    // 特征 集合（数组）
    xf_ble_gatts_chara_t *chara_set; // 原为 characteristics 但s不够明显
}xf_ble_gatts_service_t;

/**
 * @brief 发送通知/指示信息。
 */
typedef struct {
    xf_bt_attr_handle_t chara_value_handle; /*!< @if Eng Attribute handle.
                               @else   属性句柄。 @endif */
    uint16_t value_len;   /*!< @if Eng Length of notification/indication data.
                               @else   通知/指示数据长度。 @endif */
    uint8_t *value;       /*!< @if Eng Notification/indication data.
                               @else   发送的通知/指示数据。 @endif */
} xf_ble_gatts_ntf_ind_t;


typedef struct {
    uint8_t           *value;
    uint16_t          len;                                  /*!< Gatt attribute value length */
    uint16_t          offset;                               /*!< Gatt attribute value offset */
    // uint16_t          handle;                               /*!< Gatt attribute handle */
    // uint8_t           auth_req;                             /*!< Gatt authentication request */
    // uint8_t           value[ESP_GATT_MAX_ATTR_LEN];         /*!< Gatt attribute value */
} xf_ble_gatts_response_value_t;

typedef struct{
    uint16_t conn_id;               /*!< Connection id */
    uint16_t conn_handle;           /*!< HCI connection handle */
    uint8_t link_role;              /*!< Link role : master role = 0  ; slave role = 1*/
    xf_bt_dev_addr_t peer_addr;       /*!< Remote bluetooth device address */
    // conn_params; /*!< current connection parameters */
} xf_ble_evt_param_connect_t; 

typedef struct{
    uint16_t conn_id;               /*!< Connection id */
    xf_bt_dev_addr_t peer_addr;       /*!< Remote bluetooth device address */
    // conn_params; /*!< current connection parameters */
} xf_ble_evt_param_pair_end_t; 

typedef struct{
    uint16_t conn_id;               /*!< Connection id */
    xf_bt_dev_addr_t peer_addr;       /*!< Remote bluetooth device address */
    xf_ble_gap_disconnect_reason_t reason;  /*!< disconnection reason */
} xf_ble_evt_param_disconnect_t; 

typedef struct{
    uint8_t app_id;
    // status;       /*!< Operation status */
    uint16_t service_handle;        /*!< Service attribute handle */
    xf_bt_uuid_info_t service_id;  /*!< Service id, include service uuid and other information */
    xf_ble_gatt_service_type_t service_type;
} xf_ble_evt_param_gatts_add_service_t; 

typedef struct{
    uint8_t app_id;
    // status;       /*!< Operation status */
    uint16_t service_handle;        /*!< Service attribute handle */
} xf_ble_evt_param_gatts_del_service_t;   

typedef struct{
    uint8_t app_id;
    // status;       /*!< Operation status */
    uint16_t service_handle;        /*!< Service attribute handle */
    uint16_t chara_handle;           /*!< Characteristic attribute handle */
    xf_bt_uuid_info_t chara_uuid;        /*!< Characteristic uuid */
    uint16_t chara_value_handle;
} xf_ble_evt_param_gatts_add_chara_t; 

typedef struct{
    uint8_t app_id;
    // status;       /*!< Operation status */
    uint16_t service_handle;        /*!< Service attribute handle */
    xf_bt_uuid_info_t desc_uuid;       /*!< Characteristic descriptor uuid */
    uint16_t desc_handle;           /*!< Descriptor attribute handle */
} xf_ble_evt_param_gatts_add_chara_desc_t;    

typedef struct{
    uint8_t app_id;
    // status;       /*!< Operation status */
    uint16_t service_handle;        /*!< Service attribute handle */
} xf_ble_evt_param_gatts_service_start_t; 

typedef struct{
    uint8_t app_id;
    // status;       /*!< Operation status */
    uint16_t service_handle;        /*!< Service attribute handle */
}xf_ble_evt_param_gatts_service_stop_t; 

typedef struct{
    uint8_t app_id;
    uint16_t conn_id;               /*!< Connection id */
    uint32_t trans_id;              /*!< Transfer id */
    // xf_bt_dev_addr_t bt_dev_addr;              /*!< The bluetooth device address which been read */
    uint16_t handle;                /*!< The attribute handle */
    uint16_t offset;                /*!< Offset of the value, if the value is too long */
    bool need_rsp;                  /*!< The read operation need to do response */
    bool need_authorize;
    bool is_long;                   /*!< The value is too long or not */
} xf_ble_evt_param_gatts_read_req_t;

typedef struct{
    uint8_t app_id;
    uint16_t conn_id;               /*!< Connection id */
    uint32_t trans_id;              /*!< Transfer id */
    // xf_bt_dev_addr_t bt_dev_addr;              /*!< The bluetooth device address which been read */
    uint16_t handle;                /*!< The attribute handle */
    uint16_t offset;                /*!< Offset of the value, if the value is too long */
    bool need_rsp;                  /*!< The write operation need to do response */
    bool need_authorize;
    bool is_prep;                   /*!< This write operation is prepare write */
    uint16_t value_len;               /*!< The write attribute value length */
    uint8_t *value;                 /*!< The write attribute value */
} xf_ble_evt_param_gatts_write_req_t;

typedef struct{
    uint8_t app_id;
    uint16_t conn_id;               /*!< Connection id */
    uint16_t mtu_size;                   /*!< MTU size */
} xf_ble_evt_param_gatts_mtu_t;

/**
 * @brief Gatt server callback parameters union
 */
typedef union {
    /* XF_BLE_GAP_EVT_CONNECT */
    xf_ble_evt_param_connect_t connect;   

    /* XF_BLE_GAP_EVT_DISCONNECT */
    xf_ble_evt_param_disconnect_t disconnect; 

    xf_ble_evt_param_pair_end_t pair_end;

    /* XF_BLE_GAP_ADV_STARTED */
    /* XF_BLE_GAP_ADV_STOPPED */
    /* XF_BLE_GAP_EVT_CONN_PARAMS_UPDATE */

    /* XF_BLE_GATTS_EVT_ADD_SERVICE */
    xf_ble_evt_param_gatts_add_service_t add_service; 
    
    /* XF_BLE_GATTS_EVT_DEL_SERVICE */
    xf_ble_evt_param_gatts_del_service_t del_service;                              /*!< Gatt server callback param of ESP_GATTS_DELETE_EVT */

    /* XF_BLE_GATTS_EVT_ADD_CHARA */
    xf_ble_evt_param_gatts_add_chara_t add_chara; 

    /* XF_BLE_GATTS_EVT_ADD_DESC */
    xf_ble_evt_param_gatts_add_chara_desc_t add_chara_desc;    

    /* XF_BLE_GATTS_EVT_SERVICE_START */
    xf_ble_evt_param_gatts_service_start_t service_start; 

    /* XF_BLE_GATTS_EVT_SERVICE_STOP */
    xf_ble_evt_param_gatts_service_stop_t service_stop; 
    
    /* XF_BLE_GATTS_EVT_REQ_READ */
    xf_ble_evt_param_gatts_read_req_t read_req;  

    /* XF_BLE_GATTS_EVT_REQ_WRITE */
    xf_ble_evt_param_gatts_write_req_t write_req;      
    
    /* XF_BLE_GATTS_EVT_MTU_CHANGED */
    xf_ble_evt_param_gatts_mtu_t mtu;   
}xf_ble_gatts_evt_cb_param_t;


/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GATT_SERVER_TYPES_H__ */
