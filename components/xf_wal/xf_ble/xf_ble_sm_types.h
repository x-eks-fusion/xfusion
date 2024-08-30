#ifndef __XF_BLE_SM_TYPES_H__
#define __XF_BLE_SM_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_ble_gap_types.h"

#if XF_BLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

// 使能 BLE 
xf_err_t xf_ble_enable(void);
// 失能 BLE
xf_err_t xf_ble_disable(void);

// 设置本地设备地址
// xf_err_t xf_ble_gap_set_local_addr(xf_bt_dev_addr_t addr_info);
xf_err_t xf_ble_gap_set_local_addr(
    uint8_t addr[XF_BT_DEV_ADDR_LEN],
    xf_bt_dev_addr_type_t type );
// 获取本地设备地址
xf_err_t xf_ble_gap_get_local_addr(
    xf_bt_dev_addr_t *addr);

// 设置本地设备的外观
/**
 * @param[in]       appearance   - External appearance value, these values are defined by the Bluetooth SIG, please refer to
 *                  https://www.bluetooth.com/specifications/assigned-numbers/
 */
xf_err_t xf_ble_gap_set_local_appearance(xf_ble_appearance_t appearance);

// ？获取本地设备的外观
xf_ble_appearance_t xf_ble_gap_get_local_appearance(void);

// 设置本地设备名称 （ IDF 只有 name 参数，然后内部 strlen 获取长度）
xf_err_t xf_ble_gap_set_local_name(const uint8_t *name, const uint8_t len);

// 获取本地设备名称
xf_err_t xf_ble_gap_get_local_name(uint8_t *name, uint8_t *len);


/* 广播 */
// 设置广播数据 WS63 有个 adv_id 用于多广播？
xf_err_t xf_ble_gap_set_adv_data(const xf_ble_gap_adv_data_t *data);

// 设置广播参数
xf_err_t xf_ble_gap_set_adv_param(const xf_ble_gap_adv_param_t *param);

// 开始广播 (WS63 会有 adv_id)
xf_err_t xf_ble_gap_start_adv(void);
// 停止广播 (WS63 会有 adv_id)
xf_err_t xf_ble_gap_stop_adv(void);

/* 扫描 */
// 设置扫描参数
xf_err_t xf_ble_gap_set_scan_param(const xf_ble_gap_scan_param_t *param);

// 开始扫描   IDF 会把 duration 提出来放在这作为参数传入
xf_err_t xf_ble_gap_start_scan(void);

// 停止扫描
xf_err_t xf_ble_gap_stop_scan(void);

/* 连接 */
// 更新连接参数
xf_err_t xf_ble_gap_update_conn_params(xf_ble_gap_conn_param_update_t *params);
// ？获取连接参数

// 与设备建立（ACL）连接  IDF: 没有 直接嵌在 gattc_open之中 FRQ：有，且需传入上面的各项连接参数
xf_err_t xf_ble_gap_connect(const xf_bt_dev_addr_t *addr);

// 断开设备连接，包括所有profile连接
xf_err_t xf_ble_gap_disconnect(const xf_bt_dev_addr_t *addr);

/* 配对 */
// 发起/启动配对
xf_err_t xf_ble_gap_add_pair(const xf_bt_dev_addr_t *addr);

// 取消指定设备的配对
xf_err_t xf_ble_gap_del_pair(const xf_bt_dev_addr_t *addr);

// 取消所有设备的配对
xf_err_t xf_ble_gap_del_pair_all(void);

// 获取已配对的设备
xf_err_t xf_ble_gap_ble_get_pair_list(const xf_bt_dev_addr_t *addr, uint16_t *number);

// 获取已绑定的设备
xf_err_t xf_ble_gap_ble_get_bond_list(int *dev_num, xf_bt_dev_addr_t *dev_list);


#define XF_BLE_SM_AUTHEN_REQ_NO_BOND            0x00        /*!< 0  no bondingv*/
#define XF_BLE_SM_AUTHEN_REQ_BOND               0x01        /*!< 1 << 0 device in the bonding with peer */
#define XF_BLE_SM_AUTHEN_REQ_MITM               (1 << 2)    /*!< 1 << 2 man in the middle attack */
#define XF_BLE_SM_AUTHEN_REQ_BOND_MITM          (XF_BLE_SM_AUTHEN_REQ_BOND | XF_BLE_SM_AUTHEN_REQ_MITM) /*!< 0101 banding with man in the middle attack */
#define XF_BLE_SM_AUTHEN_REQ_SC_ONLY            (1 << 3)                                  /*!< 1 << 3  secure connection */
#define XF_BLE_SM_AUTHEN_REQ_SC_BOND            (XF_BLE_SM_AUTHEN_REQ_BOND | XF_BLE_SM_AUTHEN_REQ_SC_ONLY)            /*!< 1001 secure connection with band*/
#define XF_BLE_SM_AUTHEN_REQ_SC_MITM            (XF_BLE_SM_AUTHEN_REQ_MITM | XF_BLE_SM_AUTHEN_REQ_SC_ONLY)        /*!< 1100 secure conn with MITM */
#define XF_BLE_SM_AUTHEN_REQ_SC_MITM_BOND       (XF_BLE_SM_AUTHEN_REQ_MITM | XF_BLE_SM_AUTHEN_REQ_SC_ONLY | XF_BLE_SM_AUTHEN_REQ_BOND)   /*!< 1101 SC with MITM and Bonding*/
typedef uint8_t xf_ble_sm_authen_req_t;         /*!< combination of the above bit pattern */


#define XF_BLE_SM_IO_CAP_OUT                    0   /*!< DisplayOnly */
#define XF_BLE_SM_IO_CAP_IO                     1   /*!< DisplayYesNo */
#define XF_BLE_SM_IO_CAP_IN                     2   /*!< KeyboardOnly */
#define XF_BLE_SM_IO_CAP_NONE                   3   /*!< NoInputNoOutput */
#define XF_BLE_SM_IO_CAP_KBDISP                 4   /*!< Keyboard display */
typedef uint8_t xf_ble_sm_io_cap_t;  

#define XF_BLE_SM_AUTHEN_OPTION_DISABLE 0     /*!< authentication disable*/
#define XF_BLE_SM_AUTHEN_OPTION_ENABLE  1     /*!< authentication enable*/
typedef uint8_t xf_ble_sm_authen_option_t;  

#define XF_BLE_SM_OOB_DISABLE 0     /*!< disbale the out of bond*/
#define XF_BLE_SM_OOB_ENABLE  1     /*!< enable the out of bond*/
typedef uint8_t xf_ble_sm_oob_enable_t;  

/**
 * @if Eng
 * @brief Struct of BLE advertising parameters.
 * @else
 * @brief BLE广播参数。
 * @endif
 */
typedef union _xf_ble_sm_param_val_t
{
    xf_ble_sm_authen_req_t authen_req;  /*!< 绑定能力选择与安全模式：
                                            0x01----支持绑定
                                            0x00----不支持绑定 */
    xf_ble_sm_io_cap_t io_capability;   /*!< 输入输出能力 */
    xf_ble_sm_authen_option_t authen_option;    /*!< 安全配对能力选择：
                                                    0x01----支持安全配对
                                                    0x00----不支持安全配对 */
} xf_ble_sm_param_val_t;

typedef enum {
    // XF_BLE_SM_PARAM_PASSKEY = 0,
    /*!< Authentication requirements of local device */
    XF_BLE_SM_PARAM_AUTHEN_REQ_MODE,
    /*!< The IO capability of local device */
    XF_BLE_SM_PARAM_IO_CAP_MODE,
    // /*!< Initiator Key Distribution/Generation */
    // XF_BLE_SM_PARAM_SET_INIT_KEY,
    // /*!< Responder Key Distribution/Generation */
    // XF_BLE_SM_PARAM_SET_RSP_KEY,
    // /*!< Maximum Encryption key size to support */
    // XF_BLE_SM_PARAM_MAX_KEY_SIZE,
    // /*!< Minimum Encryption key size requirement from Peer */
    // XF_BLE_SM_PARAM_MIN_KEY_SIZE,
    // /*!< Set static Passkey */
    // XF_BLE_SM_PARAM_SET_STATIC_PASSKEY,
    // /*!< Reset static Passkey */
    // XF_BLE_SM_PARAM_CLEAR_STATIC_PASSKEY,
    // /*!< Accept only specified SMP Authentication requirement */
    XF_BLE_SM_PARAM_AUTHEN_OPTION,
    // /*!< Enable/Disable OOB support */
    // XF_BLE_SM_PARAM_OOB_SUPPORT,
    // /*!< Appl encryption key size */
    // XF_BLE_SM_PARAM_APP_ENC_KEY_SIZE,
    // /*!< authentication max param */
    XF_BLE_SM_PARAM_MAX_PARAM,
} xf_ble_sm_param_type_t;

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_SM_TYPES_H__ */
