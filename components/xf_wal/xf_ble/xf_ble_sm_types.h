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

/**
 * @brief BLE SM 认证请求 (绑定能力选择与安全模式)
 */
typedef uint8_t xf_ble_sm_authen_req_t;

#define XF_BLE_SM_AUTHEN_REQ_NO_BOND        0x00            /*!< 0  no bonding */
#define XF_BLE_SM_AUTHEN_REQ_BOND           0x01            /*!< 1 << 0 device in the bonding with peer */
#define XF_BLE_SM_AUTHEN_REQ_MITM           (1 << 2)        /*!< 1 << 2 man in the middle attack */
#define XF_BLE_SM_AUTHEN_REQ_BOND_MITM      (XF_BLE_SM_AUTHEN_REQ_BOND | XF_BLE_SM_AUTHEN_REQ_MITM) /*!< 0101 banding with man in the middle attack */
#define XF_BLE_SM_AUTHEN_REQ_SC_ONLY        (1 << 3)        /*!< 1 << 3  secure connection */
#define XF_BLE_SM_AUTHEN_REQ_SC_BOND        (XF_BLE_SM_AUTHEN_REQ_BOND | XF_BLE_SM_AUTHEN_REQ_SC_ONLY)      /*!< 1001 secure connection with band*/
#define XF_BLE_SM_AUTHEN_REQ_SC_MITM        (XF_BLE_SM_AUTHEN_REQ_MITM | XF_BLE_SM_AUTHEN_REQ_SC_ONLY)      /*!< 1100 secure conn with MITM */
#define XF_BLE_SM_AUTHEN_REQ_SC_MITM_BOND   (XF_BLE_SM_AUTHEN_REQ_MITM | XF_BLE_SM_AUTHEN_REQ_SC_ONLY | XF_BLE_SM_AUTHEN_REQ_BOND)  /*!< 1101 SC with MITM and Bonding*/

/**
 * @brief BLE SM IO 能力
 */
typedef uint8_t xf_ble_sm_io_cap_t;

#define XF_BLE_SM_IO_CAP_OUT                0   /*!< DisplayOnly */
#define XF_BLE_SM_IO_CAP_IO                 1   /*!< DisplayYesNo */
#define XF_BLE_SM_IO_CAP_IN                 2   /*!< KeyboardOnly */
#define XF_BLE_SM_IO_CAP_NONE               3   /*!< NoInputNoOutput */
#define XF_BLE_SM_IO_CAP_KBDISP             4   /*!< Keyboard display */

/**
 * @brief BLE SM 认证选项 (配对能力)
 */
typedef uint8_t xf_ble_sm_authen_option_t;

#define XF_BLE_SM_AUTHEN_OPTION_DISABLE 0       /*!< 认证开启 */
#define XF_BLE_SM_AUTHEN_OPTION_ENABLE  1       /*!< 认证关闭 */

/**
 * @brief BLE SM OOB (out of bond) 使能
 */
typedef uint8_t xf_ble_sm_oob_enable_t;

#define XF_BLE_SM_OOB_DISABLE 0                 /*!< disbale the out of bond */
#define XF_BLE_SM_OOB_ENABLE  1                 /*!< enable the out of bond */

/**
 * @brief BLE SM 参数值
 */
typedef union _xf_ble_sm_param_val_t {
    xf_ble_sm_authen_req_t authen_req;          /*!< 绑定能力选择与安全模式：
                                                 *  0x01----支持绑定
                                                 *  0x00----不支持绑定
                                                 * 见 @ref xf_ble_sm_authen_req_t
                                                 */
    xf_ble_sm_io_cap_t io_capability;           /*!< 输入输出能力，见 @ref xf_ble_sm_io_cap_t */
    xf_ble_sm_authen_option_t authen_option;    /*!< 安全配对能力选择：
                                                 *  0x01----支持安全配对
                                                 *  0x00----不支持安全配对
                                                 * 见 @ref xf_ble_sm_authen_option_t
                                                 */
} xf_ble_sm_param_val_t;

typedef enum {
    XF_BLE_SM_PARAM_AUTHEN_REQ_MODE,            /*!< Authentication requirements of local device */
    XF_BLE_SM_PARAM_IO_CAP_MODE,                /*!< The IO capability of local device */
    XF_BLE_SM_PARAM_AUTHEN_OPTION,              /*!< Accept only specified SMP Authentication requirement */
    XF_BLE_SM_PARAM_MAX_PARAM,                  /*!< authentication max param */
} xf_ble_sm_param_type_t;

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_SM_TYPES_H__ */
