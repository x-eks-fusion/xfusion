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
 * @brief 属性集合的结束标记值
 *
 * @note 一般出现在构造服务结构时，
 * 多个服务、特征、描述符等属性的集合的结尾部分，
 * 一般是标记属性的 UUID 项（关键项）为
 * 'XF_BLE_ATTR_SET_END_FLAG' 表示结束(主要是为了更好显示结束的位置)
 *
 * @note 由于结束标记的特殊性（NULL），用户构造时也可以
 * 对属性集合中表示结束的最后一个属性成员结构直接置 0，如：
 *      ([attr_set_type])
 *      {
 *          {...},
 *          {...},
 *          {0}
 *      }
 */
#define XF_BLE_ATTR_SET_END_FLAG                NULL

/**
 * @brief BLE GATTS 特征描述符信息
 */
typedef struct {
    xf_ble_attr_handle_t chara_desc_handle;      /*!< 特征描述符句柄，见 @ref xf_ble_attr_handle_t */
    /**
     * 指向描述符 UUID 的指针；使用BLE_UUIDxx_DECLARE宏声明
     * 如果服务中没有更多特征，则为NULL
     */
    xf_ble_uuid_info_t *desc_uuid;               /*!< 特征描述符 UUID，见 @ref xf_ble_uuid_info_t */
    xf_ble_gatt_attr_permission_t permissions;  /*!< 特征描述符权限，见 @ref xf_ble_gatt_attr_permission_t */
    uint8_t *value;                             /*!< 属性值 */
    uint16_t value_len;                         /*!< 属性值长度 */
    xf_ble_gatt_chara_desc_type_t desc_type;    /*!< 特征描述符类型，见 @ref xf_ble_gatt_chara_desc_type_t */
} xf_ble_gatts_chara_desc_t;

/**
 * @brief BLE GATTS 特征值信息。
 * 注意，这里省略掉了 特征值声明属性中的属性类型，即特征 UUID（特征声明中的），
 * 因为此类型仅包含于特征描述声明中，便于查看。
 */
typedef struct {
    uint8_t *value;                             /*!< 特征值 */
    uint16_t value_len;                         /*!< 特征值长度 */
    xf_ble_gatt_attr_permission_t permission;   /*!< 特征值权限，见 @ref xf_ble_gatt_attr_permission_t */
} xf_ble_gatts_chara_value_t;

/**
 * @brief BLE GATTS 特征信息
 */
typedef struct _xf_ble_gatts_chara_t {
    xf_ble_attr_handle_t chara_handle;           /*!< 特征句柄，见 @ref xf_ble_attr_handle_t */
    xf_ble_gatt_chara_property_t properties;    /*!< 特征特性，见 @ref xf_ble_gatt_chara_property_t */
    xf_ble_attr_handle_t chara_value_handle;     /*!< 特征值句柄，见 @ref xf_ble_attr_handle_t */
    xf_ble_uuid_info_t *chara_uuid;              /*!< 特征 UUID ，见 @ref xf_ble_uuid_info_t */
    xf_ble_gatts_chara_value_t chara_value;     /*!< 特征值信息，见 @ref xf_ble_gatts_chara_value_t */
    xf_ble_gatts_chara_desc_t
    *desc_set;        /*!< 特征描述符集合信息，如无，则填 NULL，见 @ref xf_ble_gatts_chara_desc_t */
} xf_ble_gatts_chara_t;

/**
 * @brief BLE GATTS 服务信息
 */
typedef struct _xf_ble_gatts_service_t xf_ble_gatts_service_t;
typedef struct _xf_ble_gatts_service_t {
    xf_ble_attr_handle_t service_handle;         /*!< 服务句柄，见 @ref xf_ble_attr_handle_t */
    xf_ble_gatt_service_type_t service_type;    /*!< 服务类型，见 @ref xf_ble_gatt_service_type_t */
    xf_ble_uuid_info_t *service_uuid;            /*!< 服务UUID ，见 @ref xf_ble_uuid_info_t */
    xf_ble_gatts_service_t
    **include_set;       /*!< 包含服务 (include service) 集合信息，见 @ref xf_ble_gatts_service_t */
    xf_ble_gatts_chara_t *chara_set;            /*!< 特征集合信息 ，见 @ref xf_ble_gatts_chara_t */
} xf_ble_gatts_service_t;

/**
 * @brief BLE GATTS 发送通知或指示的信息
 */
typedef struct {
    xf_ble_attr_handle_t handle;     /*!< 属性句柄，见 @ref xf_ble_attr_handle_t */
    uint16_t value_len;                         /*!< 通知/指示的值长度 */
    uint8_t *value;                             /*!< 发送的通知/指示的值 */
} xf_ble_gatts_ntf_ind_t;

/**
 * @brief BLE GATTS 响应 (回应) 值信息
 */
typedef struct {
    uint8_t           *value;                   /*!< 响应的值 */
    uint16_t          value_len;                /*!< 响应的值长度 */
    uint16_t          offset;                   /*!< 属性值的偏移 */
} xf_ble_gatts_response_value_t;

/**
 * @brief BLE GATTS 连接事件的参数
 */
typedef struct {
    uint16_t conn_id;                           /*!< 链接 (连接) ID */
    uint16_t conn_handle;                       /*!< 链接 (连接) 句柄 */
    xf_ble_gap_link_role_type_t link_role;      /*!< 链路角色，见 @ref xf_ble_gap_link_role_type_t */
    xf_ble_addr_t peer_addr;                 /*!< 对端地址， @ref xf_ble_addr_t */
} xf_ble_evt_param_connect_t;

/**
 * @brief BLE GATTS 配对结束事件的参数
 */
typedef struct {
    uint16_t conn_id;                           /*!< 链接 (连接) ID */
    xf_ble_addr_t peer_addr;                 /*!< 对端地址， @ref xf_ble_addr_t */
} xf_ble_evt_param_pair_end_t;

/**
 * @brief BLE GATTS 断连事件的参数
 */
typedef struct {
    uint16_t conn_id;                           /*!< 链接 (连接) ID */
    xf_ble_addr_t peer_addr;                 /*!< 对端地址， @ref xf_ble_addr_t */
    xf_ble_gap_disconnect_reason_t reason;      /*!< 断连原因， @ref xf_ble_gap_disconnect_reason_t */
} xf_ble_evt_param_disconnect_t;

/**
 * @brief BLE GATTS 服务添加事件的参数
 */
typedef struct {
    uint8_t app_id;                             /*!< 服务端 (应用) ID */
    uint16_t service_handle;                    /*!< 服务句柄 */
    xf_ble_uuid_info_t service_id;               /*!< 服务 UUID，见 @ref xf_ble_uuid_info_t */
    xf_ble_gatt_service_type_t service_type;    /*!< 服务类型 */
} xf_ble_evt_param_gatts_add_service_t;

/**
 * @brief BLE GATTS 服务删除事件的参数
 */
typedef struct {
    uint8_t app_id;                             /*!< 服务端 (应用) ID */
    uint16_t service_handle;                    /*!< 服务句柄 */
} xf_ble_evt_param_gatts_del_service_t;

/**
 * @brief BLE GATTS 服务删除事件的参数
 */
typedef struct {
    uint8_t app_id;                             /*!< 服务端 (应用) ID */
    uint16_t service_handle;                    /*!< 服务句柄 */
    uint16_t chara_handle;                      /*!< 特征句柄 */
    xf_ble_uuid_info_t chara_uuid;               /*!< 特征 UUID，见 @ref xf_ble_uuid_info_t */
    uint16_t chara_value_handle;                /*!< 特征值句柄 */
} xf_ble_evt_param_gatts_add_chara_t;

/**
 * @brief BLE GATTS 特征描述符添加事件的参数
 */
typedef struct {
    uint8_t app_id;                             /*!< 服务端 (应用) ID */
    uint16_t service_handle;                    /*!< 服务句柄 */
    xf_ble_uuid_info_t desc_uuid;                /*!< 特征描述符 UUID，见 @ref xf_ble_uuid_info_t */
    uint16_t desc_handle;                       /*!< 特征描述符句柄 */
} xf_ble_evt_param_gatts_add_chara_desc_t;

/**
 * @brief BLE GATTS 服务启动事件的参数
 */
typedef struct {
    uint8_t app_id;                             /*!< 服务端 (应用) ID */
    uint16_t service_handle;                    /*!< 服务句柄 */
} xf_ble_evt_param_gatts_service_start_t;

/**
 * @brief BLE GATTS 服务停止事件的参数
 */
typedef struct {
    uint8_t app_id;                             /*!< 服务端 (应用) ID */
    uint16_t service_handle;                    /*!< 服务句柄 */
} xf_ble_evt_param_gatts_service_stop_t;

/**
 * @brief BLE GATTS 接收到读请求事件的参数
 */
typedef struct {
    uint8_t app_id;                             /*!< 服务端 (应用) ID */
    uint16_t conn_id;                           /*!< 链接 (连接) ID */
    uint32_t trans_id;                          /*!< 传输 ID */
    uint16_t handle;                            /*!< 属性句柄 */
    uint16_t offset;                            /*!< 值偏移 (如果值过长) */
    bool need_rsp;                              /*!< 是否需要响应 (回应) */
    bool need_authorize;                        /*!< 是否需要授权 */
    bool is_long;                               /*!< 值是否过长 */
} xf_ble_evt_param_gatts_read_req_t;

/**
 * @brief BLE GATTS 接收到写请求事件的参数
 */
typedef struct {
    uint8_t app_id;                             /*!< 服务端 (应用) ID */
    uint16_t conn_id;                           /*!< 链接 (连接) ID */
    uint32_t trans_id;                          /*!< 传输 ID */
    uint16_t handle;                            /*!< 属性句柄 */
    uint16_t offset;                            /*!< 值偏移 (如果值过长) */
    bool need_rsp;                              /*!< 是否需要响应 (回应) */
    bool need_authorize;                        /*!< 是否需要授权 */
    bool is_prep;                               /*!< 是否是 prepare write 操作 */
    uint16_t value_len;                         /*!< 属性值长度 */
    uint8_t *value;                             /*!< 属性值 */
} xf_ble_evt_param_gatts_write_req_t;

/**
 * @brief BLE GATTS MTU变更事件的参数
 */
typedef struct {
    uint8_t app_id;                             /*!< 服务端 (应用) ID */
    uint16_t conn_id;                           /*!< 链接 (连接) ID */
    uint16_t mtu_size;                          /*!< MTU 大小 */
} xf_ble_evt_param_gatts_mtu_t;

/**
 * @brief GATT 服务端事件回调参数
 */
typedef union {
    xf_ble_evt_param_connect_t connect;         /*!< 连接事件的参数，
                                                 *  @ref xf_ble_evt_param_connect_t
                                                 *  XF_BLE_GAP_EVT_CONNECT
                                                 */
    xf_ble_evt_param_disconnect_t disconnect;   /*!< 断连事件的参数，
                                                 *  @ref xf_ble_evt_param_disconnect_t
                                                 *  XF_BLE_GAP_EVT_DISCONNECT
                                                 */
    xf_ble_evt_param_pair_end_t pair_end;       /*!< 配对结束事件的参数，
                                                 *  @ref xf_ble_evt_param_pair_end_t
                                                 *  XF_BLE_GAP_EVT_PAIR_END
                                                 */
    xf_ble_evt_param_gatts_add_service_t add_service;
    /*!< 服务添加事件的参数，
     *  @ref xf_ble_evt_param_gatts_add_service_t
     *  XF_BLE_GATTS_EVT_ADD_SERVICE
     */
    xf_ble_evt_param_gatts_del_service_t del_service;
    /*!< 服务移除事件的参数，
     *  @ref xf_ble_evt_param_gatts_del_service_t
     *  XF_BLE_GATTS_EVT_DEL_SERVICE
     */
    xf_ble_evt_param_gatts_add_chara_t add_chara;
    /*!< 特征添加事件的参数，
     *  @ref xf_ble_evt_param_gatts_add_chara_t
     *  XF_BLE_GATTS_EVT_ADD_CHARA
     */
    xf_ble_evt_param_gatts_add_chara_desc_t add_chara_desc;
    /*!< 特征描述符添加事件的参数，
     *  @ref xf_ble_evt_param_gatts_add_chara_desc_t
     *  XF_BLE_GATTS_EVT_ADD_DESC
     */
    xf_ble_evt_param_gatts_service_start_t service_start;
    /*!< 服务启动事件的参数，
     *  @ref xf_ble_evt_param_gatts_service_start_t
     *  XF_BLE_GATTS_EVT_SERVICE_START
     */
    xf_ble_evt_param_gatts_service_stop_t service_stop;
    /*!< 服务停止事件的参数，
     *  @ref xf_ble_evt_param_gatts_service_stop_t
     *  XF_BLE_GATTS_EVT_SERVICE_STOP
     */
    xf_ble_evt_param_gatts_read_req_t read_req;
    /*!< 接收到读请求事件的参数，
     *  @ref xf_ble_evt_param_gatts_read_req_t
     *  XF_BLE_GATTS_EVT_REQ_READ
     */
    xf_ble_evt_param_gatts_write_req_t write_req;
    /*!< 接收到写请求事件的参数，
     *  @ref xf_ble_evt_param_gatts_write_req_t
     *  XF_BLE_GATTS_EVT_REQ_WRITE
     */
    xf_ble_evt_param_gatts_mtu_t mtu;           /*!< MTU 变更事件的参数，
                                                 *  @ref xf_ble_evt_param_gatts_mtu_t
                                                 *  XF_BLE_GATTS_EVT_MTU_CHANGED
                                                 */
} xf_ble_gatts_evt_cb_param_t;


/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_GATT_SERVER_TYPES_H__ */
