/**
 * @example examples/wireless/ble/sample_gatt_server/main/xf_main.c
 * xf_wal ble gatt 服务端 示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"
#include "xf_ble_gap.h"
#include "xf_ble_gatt_server.h"
#include "string.h"
#include "ble_hid_types.h"
#include "xf_osal.h"

/* ==================== [Defines] =========================================== */

#define TAG "sample_gatts"

#define DEFAULT_INPUT_IO                3
#define DEFAULT_INPUT_IO_MODE           XF_HAL_GPIO_PULL_UP
#define DEFAULT_INPUT_IO_TRIGGER_TYPE   XF_HAL_GPIO_INTR_TYPE_ANY

#define DEFAULT_BLE_GAP_ADV_ID              0x01

/* ble 最小广播间隔 */
#define DEFAULT_BLE_GAP_ADV_MIN_INTERVAL    0x30
/* ble 最大广播间隔 */
#define DEFAULT_BLE_GAP_ADV_MAX_INTERVAL    0x60
/* ble 广播 id */
#define DEFAULT_BLE_GAP_ADV_ID              0x01
/* ble 广播持续时间 */
#define DEFAULT_BLE_GAP_ADV_DURATION_FOREVER 0

/* HID protocol mode values */
#define HID_PROTOCOL_MODE_BOOT          0x00      // Boot Protocol Mode
#define HID_PROTOCOL_MODE_REPORT        0x01      // Report Protocol Mode

/* ==================== [Typedefs] ========================================== */

typedef struct __packed _usb_hid_report_mouse_t {
    uint16_t button_mask;
    uint16_t x : 12;
    uint16_t y : 12;
    uint8_t wheel;
    uint8_t ac_pan;
} usb_hid_report_mouse_t;

/* ==================== [Static Prototypes] ================================= */

static void sample_ble_set_adv_data(void);
static void sample_ble_set_adv_param(void);
static xf_err_t sample_ble_gatts_event_cb(
    xf_ble_gatts_event_t event,
    xf_ble_gatts_evt_cb_param_t param);
static void sample_gpio_irq(xf_gpio_num_t gpio_num, bool level, void *user_data);

/* ==================== [Static Variables] ================================== */

static uint8_t s_gatts_device_name[] = "XF_BLE_MOUSE";
static uint8_t s_app_id = 0;
static uint16_t s_conn_id = 0;

static usb_hid_report_mouse_t s_hid_report_mouse = {0};

/* HID information value for test */
static uint8_t s_hid_infor_val[] = {
    uint16_to_byte(BLE_HID_VERSION), BLE_HID_INFO_COUNTRY_CODE,
    BLE_HID_INFO_FLAG_REMOTE_WAKE_UP_MSK | BLE_HID_INFO_FLAG_NORMALLY_CONNECTABLE_MSK
};

/* HID Report Map (Descriptor) */
static uint8_t s_hid_report_map_mouse[] = {
    0x05, 0x01, /* Usage Page (Generic Desktop) */
    0x09, 0x02, /* Usage (Mouse) */
    0xA1, 0x01, /* Collection (Application) */
    0x85, 0x02, /* Report Id (2) */
    0x09, 0x01, /* Usage (Pointer) */
    0xA1, 0x00, /* Collection (Physical) */
    0x95, 0x10, /* Report Count (16) */
    0x75, 0x01, /* Report Size (1) */
    0x15, 0x00, /* Logical minimum (0) */
    0x25, 0x01, /* Logical maximum (1) */
    0x05, 0x09, /* Usage Page (Button) */
    0x19, 0x01, /* Usage Minimum (Button 1) */
    0x29, 0x10, /* Usage Maximum (Button 16) */
    0x81, 0x02, /* Input (data,Value,Absolute,Bit Field) */
    0x05, 0x01, /* Usage Page (Generic Desktop) */
    0x16, 0x01, 0xF8, /* Logical minimum (-2047) */
    0x26, 0xFF, 0x07, /* Logical maximum (2047) */
    0x75, 0x0C, /* Report Size (12) */
    0x95, 0x02, /* Report Count (2) */
    0x09, 0x30, /* Usage (X) */
    0x09, 0x31, /* Usage (Y) */
    0x81, 0x06, /* Input (data,Value,Relative,Bit Field) */
    0x15, 0x81, /* Logical minimum (-127) */
    0x25, 0x7F, /* Logical maximum (127) */
    0x75, 0x08, /* Report Size (8) */
    0x95, 0x01, /* Report Count (1) */
    0x09, 0x38, /* Usage (Wheel) */
    0x81, 0x06, /* Input (data,Value,Relative,Bit Field) */
    0x95, 0x01, /* Report Count (1) */
    0x05, 0x0C, /* Usage Page (Consumer) */
    0x0A, 0x38, 0x02, /* Usage (AC Pan) */
    0x81, 0x06, /* Input (data,Value,Relative,Bit Field) */
    0xC0, /* End Collection */
    0xC0, /* End Collection */
};

/* HID protocol mode value for test */
static uint8_t s_hid_protocol_mode_val[] = {HID_PROTOCOL_MODE_REPORT};

/* HID input report value  for test */
static uint8_t s_hid_input_report_val[] = {0x00};

/* HID output report value for test */
static uint8_t s_hid_output_report_val[] = {0x00};

/* HID control point value for test */
static uint8_t s_hid_control_point_val[] = {0x00, 0x00};

/* HID client characteristic configuration value for test */
static uint8_t s_ccc_val[] = {0x00, 0x00};

/* HID input report reference value for test  [report id, input] */
static uint8_t s_hid_report_ref_input_val[] = {
    BLE_HID_REPORT_ID, BLE_REPORT_REFERENCE_REPORT_TYPE_INPUT_REPORT
};

/* HID output report reference value for test [report id, output] */
static uint8_t s_hid_report_ref_output_val[] = {
    BLE_HID_REPORT_ID, BLE_REPORT_REFERENCE_REPORT_TYPE_OUTPUT_REPORT
};

enum {
    HID_CHARA_INDEX_INFO = 0,
    HID_CHARA_INDEX_REPORT_MAP,
    HID_CHARA_INDEX_PROTOCOL_MODE,
    HID_CHARA_INDEX_INPUT_REPORT,
    HID_CHARA_INDEX_OUTPUT_REPORT,
    HID_CHARA_INDEX_CTRL_POINT,
};

static xf_ble_gatts_chara_t s_chara_set[] = {
    /* 特征：HID information */
    [HID_CHARA_INDEX_INFO] =
    {
        .chara_uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_HID_INFORMATION),
        .permission = XF_BLE_GATT_ATTR_PERMISSION_READ,
        .properties = HID_INFORMATION_PROPERTIES,
        .chara_value = {
            .value = s_hid_infor_val,
            .value_len = sizeof(s_hid_infor_val),
        },
    },
    /* 特征：HID report map */
    [HID_CHARA_INDEX_REPORT_MAP] =
    {
        .chara_uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_REPORT_MAP),
        .permission = XF_BLE_GATT_ATTR_PERMISSION_READ
        | XF_BLE_GATT_ATTR_PERMISSION_AUTHENTICATION_NEED,    // 需要认证
        .properties = HID_REPORT_MAP_PROPERTIES,
        .chara_value = {
            .value = s_hid_report_map_mouse,
            .value_len = sizeof(s_hid_report_map_mouse),
        },
    },
    /* 特征：HID protocol mode */
    [HID_CHARA_INDEX_PROTOCOL_MODE] =
    {
        .chara_uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_PROTOCOL_MODE),
        .permission = XF_BLE_GATT_ATTR_PERMISSION_READ,
        .properties = HID_PROTOCOL_MODE_PROPERTIES,
        .chara_value = {
            .value = s_hid_protocol_mode_val,
            .value_len = sizeof(s_hid_protocol_mode_val),
        },
    },
    /* 特征：HID input report (device to host)*/
    [HID_CHARA_INDEX_INPUT_REPORT] =
    {
        .chara_uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_REPORT),
        .permission = XF_BLE_GATT_ATTR_PERMISSION_READ,
        .properties = HID_INPUT_REPORT_PROPERTIES,
        .chara_value = {
            .value = s_hid_input_report_val,
            .value_len = sizeof(s_hid_input_report_val),
        },
        .desc_set = (xf_ble_gatts_chara_desc_t [])
        {
            {
                // CCC: Client Characteristic Configuration
                .desc_uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_CLIENT_CHARACTERISTIC_CONFIGURATION),
                .permissions = XF_BLE_GATT_ATTR_PERMISSION_READ | XF_BLE_GATT_ATTR_PERMISSION_WRITE,
                .value = s_ccc_val,
                .value_len = sizeof(s_ccc_val),
            }, {
                // report reference, input
                .desc_uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_REPORT_REFERENCE),
                .permissions = XF_BLE_GATT_ATTR_PERMISSION_READ,
                .value = s_hid_report_ref_input_val,
                .value_len = sizeof(s_hid_report_ref_input_val),
            },
            {0}
        },
    },
    /* 特征：HID output report (host to device)*/
    [HID_CHARA_INDEX_OUTPUT_REPORT] =
    {
        .chara_uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_REPORT),
        .permission = XF_BLE_GATT_ATTR_PERMISSION_READ,
        .properties = HID_OUTPUT_REPORT_PROPERTIES,
        .chara_value = {
            .value = s_hid_output_report_val,
            .value_len = sizeof(s_hid_output_report_val),
        },
        .desc_set = (xf_ble_gatts_chara_desc_t [])
        {
            {
                // report reference, output
                .desc_uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_REPORT_REFERENCE),
                .permissions = XF_BLE_GATT_ATTR_PERMISSION_READ,
                .value = s_hid_report_ref_output_val,
                .value_len = sizeof(s_hid_report_ref_output_val),
            },
            {0}
        },
    },
    /* 特征：HID control point */
    [HID_CHARA_INDEX_CTRL_POINT] =
    {
        .chara_uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_HID_CONTROL_POINT),
        .permission = XF_BLE_GATT_ATTR_PERMISSION_READ,
        .properties = HID_CONTROL_POINT_PROPERTIES,
        .chara_value = {
            .value = s_hid_control_point_val,
            .value_len = sizeof(s_hid_control_point_val),
        },
    },
    {0}
};

static xf_ble_gatts_service_t s_svc_set[] = {
    {
        .service_uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_HUMAN_INTERFACE_DEVICE),
        .service_type = XF_BLE_GATT_SERVICE_TYPE_PRIMARY,
        .chara_set = &s_chara_set,
    },
    {0}
};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    XF_LOGI(TAG, "xf ble gatt server HID Mouse");
    xf_err_t ret = XF_OK;
    // 使能 ble
    xf_ble_enable();

    /* 设置触发的 GPIO，设置中断任务回调位 HID 上报函数 */
    xf_hal_gpio_init(DEFAULT_INPUT_IO, XF_HAL_GPIO_DIR_IN);
    xf_hal_gpio_set_pull(DEFAULT_INPUT_IO, XF_HAL_GPIO_PULL_NONE);
    xf_hal_gpio_set_intr_type(DEFAULT_INPUT_IO, XF_HAL_GPIO_INTR_TYPE_RISING);
    xf_hal_gpio_set_intr_cb(DEFAULT_INPUT_IO, sample_gpio_irq, NULL);
    xf_hal_gpio_set_intr_enable(DEFAULT_INPUT_IO);

    // 注册 gatts 事件回调
    ret = xf_ble_gatts_event_cb_register(sample_ble_gatts_event_cb, XF_BLE_COMMON_EVT_ALL);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "REGISTER event cb failed:%#X", ret);

    /* 设置本地名称、外观 */
    xf_ble_gap_set_local_name(s_gatts_device_name, sizeof(s_gatts_device_name));
    xf_ble_gap_set_local_appearance(XF_BLE_APPEARANCE_HID_MOUSE);

    // 注册 gatts app_profile
    xf_ble_uuid_info_t app_uuid = XF_BLE_UUID16_INIT(0x0000);

    ret = xf_ble_gatts_app_register(&app_uuid, &s_app_id);
    XF_CHECK(ret != XF_OK || s_app_id == 0, XF_RETURN_VOID, TAG,
             "REGISTER app profile failed:%#X s_app_id:%d", ret, s_app_id);

    // 添加服务至 app_profile
    ret = xf_ble_gatts_add_service_to_app(
              s_app_id, &s_svc_set[0]);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "ADD service failed:%#X", ret);
    XF_LOGI(TAG, ">>> HDL %d", s_svc_set[0].service_handle);
    // 启动服务
    ret = xf_ble_gatts_start_service(s_app_id, s_svc_set[0].service_handle);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "START service failed:%#X", ret);

    /* 设置广播数据及参数 */
    sample_ble_set_adv_data();
    sample_ble_set_adv_param();
    XF_LOGI(TAG, "CONFIG ADV CMPL");

    // 开启广播
    ret = xf_ble_gap_start_adv(DEFAULT_BLE_GAP_ADV_ID);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "STAR ADV failed:%#X", ret);
    XF_LOGI(TAG, "STAR ADV CMPL");
}

/* ==================== [Static Functions] ================================== */

static void sample_gpio_irq(xf_gpio_num_t gpio_num, bool level, void *user_data)
{
    XF_LOGI(TAG, "HID report");

    s_hid_report_mouse.ac_pan = 0;
    s_hid_report_mouse.button_mask = 0;
    s_hid_report_mouse.wheel = 0;
    s_hid_report_mouse.x = 60;
    s_hid_report_mouse.y = 0;

    xf_ble_gatts_ntf_t ntf_param = {
        .value = &s_hid_report_mouse,
        .value_len = sizeof(s_hid_report_mouse),
        .handle = s_svc_set[0].chara_set[HID_CHARA_INDEX_INPUT_REPORT].chara_value_handle,
    };
    xf_err_t ret = xf_ble_gatts_send_notification(
                       s_app_id, s_conn_id, &ntf_param);
    XF_CHECK(ret != XF_OK, ret, TAG,
             "send_notify_indicate failed:%#X", ret);
}

static xf_err_t sample_ble_gatts_event_cb(
    xf_ble_gatts_event_t event,
    xf_ble_gatts_evt_cb_param_t param)
{
    UNUSED(s_app_id);
    UNUSED(param);
    switch (event) {
    /* 事件: 连接  */
    case XF_BLE_COMMON_EVT_CONNECT: {
        XF_LOGI(TAG, "EV:peer connect:s_app_id:%d,conn_id:%d,"
                "addr_type:%d,addr:"XF_BLE_ADDR_PRINT_FMT,
                s_app_id, param.connect.conn_id,
                param.connect.peer_addr.type,
                XF_BLE_ADDR_EXPAND_TO_ARG(param.connect.peer_addr.addr));

        s_conn_id = param.connect.conn_id;
    } break;
    /* 事件: 断连  */
    case XF_BLE_COMMON_EVT_DISCONNECT: {
        XF_LOGI(TAG, "EV:peer disconnect:s_app_id:%d,conn_id:%d,reason:%u,"
                "addr_type:%d,addr:"XF_BLE_ADDR_PRINT_FMT,
                s_app_id, param.disconnect.conn_id,
                param.disconnect.reason,
                param.disconnect.peer_addr.type,
                XF_BLE_ADDR_EXPAND_TO_ARG(param.disconnect.peer_addr.addr));
        XF_LOGI(TAG, "It will restart ADV");
        xf_ble_gap_start_adv(DEFAULT_BLE_GAP_ADV_ID);
    } break;
    default:
        break;
    }

    return XF_OK;
}

static void sample_ble_set_adv_data(void)
{
    xf_ble_gap_adv_data_t adv_data = {
        .adv_struct_set = (xf_ble_gap_adv_struct_t [])
        {
            {
                .ad_type = XF_BLE_ADV_STRUCT_TYPE_FLAGS,
                .ad_data_len = sizeof(uint8_t),
                .ad_data.flag = (1 << 2) | (1 << 0),
                .is_ptr = false,
            }, {
                .ad_type = XF_BLE_ADV_STRUCT_TYPE_LOCAL_NAME_ALL,
                .ad_data_len = sizeof(s_gatts_device_name),
                .ad_data.local_name = s_gatts_device_name,
                .is_ptr = true,
            }, {
                .ad_type = XF_BLE_ADV_STRUCT_TYPE_APPEARANCE,
                .ad_data_len = sizeof(xf_ble_appearance_t),
                .ad_data.appearance = XF_BLE_APPEARANCE_HID_MOUSE,
                .is_ptr = false,
            },
            {0}
        },
        .scan_rsp_struct_set = (xf_ble_gap_adv_struct_t [])
        {
            {
                0
            }
        },
    };
    xf_ble_gap_set_adv_data(DEFAULT_BLE_GAP_ADV_ID, &adv_data);
}
static void sample_ble_set_adv_param(void)
{
    xf_ble_gap_adv_param_t adv_param = {
        /* 广播间隔 [N * 0.625ms] */
        .min_interval = DEFAULT_BLE_GAP_ADV_MIN_INTERVAL,
        .max_interval = DEFAULT_BLE_GAP_ADV_MAX_INTERVAL,

        // 广播类型
        .adv_type = XF_BLE_GAP_ADV_TYPE_CONN_SCAN_UNDIR,
        .own_addr =
        {
            .type = XF_BLE_ADDR_TYPE_PUBLIC_DEV, // 本端地址类型
            .addr = {0x0},                       // 本端地址
        },
        .peer_addr =
        {
            .type = XF_BLE_ADDR_TYPE_PUBLIC_DEV, // 对端地址类型
            .addr = { 0x0 },                    // 对端地址
        },
        .channel_map = XF_BLE_GAP_ADV_CH_ALL,   // 使用37/38/39三个通道
        .adv_filter_policy = XF_BLE_GAP_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
        .duration = DEFAULT_BLE_GAP_ADV_DURATION_FOREVER,
        .tx_power = 1,  // 发送功率,单位dbm,范围-127~20
    };
    xf_ble_gap_set_adv_param(DEFAULT_BLE_GAP_ADV_ID, &adv_param);
}