/**
 * @file ble_hid.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __BLE_HID_TYPES_H__
#define __BLE_HID_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_ble_gatt_common.h"

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_wal_ble
 * @endcond
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* Human Interface Device UUID */
#define BLE_UUID_HUMAN_INTERFACE_DEVICE                 0x1812
/* HID Information UUID */              
#define BLE_UUID_HID_INFORMATION                        0x2A4A
/* Report Map UUID */               
#define BLE_UUID_REPORT_MAP                             0x2A4B
/* HID Control Point UUID */                
#define BLE_UUID_HID_CONTROL_POINT                      0x2A4C
/* Report UUID */       
#define BLE_UUID_REPORT                                 0x2A4D
/* Protocol Mode UUID */
#define BLE_UUID_PROTOCOL_MODE                          0x2A4E
/* Client Characteristic Configuration UUID */
#define BLE_UUID_CLIENT_CHARACTERISTIC_CONFIGURATION    0x2902
/* External Report Reference UUID */
#define BLE_UUID_EXTERNAL_REPORT_REFERENCE              0x2907
/* Report Reference UUID */
#define BLE_UUID_REPORT_REFERENCE                       0x2908

/* Boot Keyboard Input Report UUID */       
#define BLE_UUID_BOOT_KEYBOARD_INPUT_REPORT             0x2A22
/* Boot Keyboard Output Report UUID */      
#define BLE_UUID_BOOT_KEYBOARD_OUTPUT_REPORT            0x2A32
/* Boot Mouse Input Report UUID */      
#define BLE_UUID_BOOT_MOUSE_INPUT_REPORT                0x2A33

/* HID information flag remote wakeup */
#define BLE_HID_INFO_FLAG_REMOTE_WAKE_UP_MSK 0x01
/* HID information flag normally connectable */
#define BLE_HID_INFO_FLAG_NORMALLY_CONNECTABLE_MSK 0x02
/* HID information country code */
#define BLE_HID_INFO_COUNTRY_CODE 0x00
/* HID spec version 1.11 */
#define BLE_HID_VERSION  0x0101
/* HID input report id */
#define BLE_HID_REPORT_ID 2
/* HID input report type */
#define BLE_REPORT_REFERENCE_REPORT_TYPE_INPUT_REPORT 1
/* HID output report type */
#define BLE_REPORT_REFERENCE_REPORT_TYPE_OUTPUT_REPORT 2

/* ==================== [Typedefs] ========================================== */

enum {
    /* HID information characteristic properties */
    HID_INFORMATION_PROPERTIES   = XF_BLE_GATT_CHARA_PROP_READ,
    /* HID protocol mode characteristic properties */
    HID_PROTOCOL_MODE_PROPERTIES = XF_BLE_GATT_CHARA_PROP_READ | XF_BLE_GATT_CHARA_PROP_WRITE_NO_RSP,
    /* HID report map characteristic properties */
    HID_REPORT_MAP_PROPERTIES    = XF_BLE_GATT_CHARA_PROP_READ,
    /* HID input report characteristic properties */
    HID_INPUT_REPORT_PROPERTIES  = XF_BLE_GATT_CHARA_PROP_READ | XF_BLE_GATT_CHARA_PROP_NOTIFY |
                                   XF_BLE_GATT_CHARA_PROP_WRITE,
    /* HID output report characteristic properties */
    HID_OUTPUT_REPORT_PROPERTIES = XF_BLE_GATT_CHARA_PROP_READ | XF_BLE_GATT_CHARA_PROP_WRITE |
                                   XF_BLE_GATT_CHARA_PROP_WRITE_NO_RSP,
    /* HID control point characteristic properties */
    HID_CONTROL_POINT_PROPERTIES = XF_BLE_GATT_CHARA_PROP_WRITE_NO_RSP,
};

#define uint16_to_byte(n) ((uint8_t)(n)), ((uint8_t)((n) >> 8))

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BLE_HID_TYPES_H__ */
