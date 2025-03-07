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

/* ==================== [Defines] =========================================== */

#define TAG "hid_dev"

#define DEFAULT_INPUT_IO                3
#define DEFAULT_INPUT_IO_MODE           XF_HAL_GPIO_PULL_DOWN
#define DEFAULT_INPUT_IO_TRIGGER_TYPE   XF_HAL_GPIO_INTR_TYPE_RISING

/* ble 最小广播间隔 */
#define DEFAULT_BLE_GAP_ADV_MIN_INTERVAL    0x30
/* ble 最大广播间隔 */
#define DEFAULT_BLE_GAP_ADV_MAX_INTERVAL    0x60
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

static xf_ble_evt_res_t sample_ble_gap_event_cb(
    xf_ble_gap_evt_t event,
    xf_ble_gap_evt_cb_param_t *param);
static xf_ble_evt_res_t sample_ble_gatts_event_cb(
    xf_ble_gatts_evt_t event,
    xf_ble_gatts_evt_cb_param_t *param);
static void sample_gpio_irq(xf_gpio_num_t gpio_num, bool level, void *user_data);

/* ==================== [Static Variables] ================================== */

static uint8_t s_gatts_device_name[] = "XF_BLE_MOUSE";
static uint8_t s_adv_id = XF_BLE_ADV_ID_INVALID;
static uint8_t s_app_id = XF_BLE_APP_ID_INVALID;
static uint16_t s_conn_id = 0;

static xf_ble_gap_adv_param_t s_adv_param = {
    /* 广播间隔 [N * 0.625ms] */
    .min_interval = DEFAULT_BLE_GAP_ADV_MIN_INTERVAL,
    .max_interval = DEFAULT_BLE_GAP_ADV_MAX_INTERVAL,

    // 广播类型
    .type = XF_BLE_GAP_ADV_TYPE_CONN_SCAN_UNDIR,
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
    .filter_policy = XF_BLE_GAP_ADV_FILTER_POLICY_ANY_SCAN_ANY_CONN,
    .tx_power = 1,  // 发送功率,单位dbm,范围-127~20
};

static xf_ble_gap_adv_data_t s_adv_data = {
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
    .scan_rsp_struct_set = NULL,
};

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
        .uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_HID_INFORMATION),
        .perms = XF_BLE_GATT_ATTR_PERM_READ,
        .props = HID_INFORMATION_PROPERTIES,
        .value = s_hid_infor_val,
        .value_len = sizeof(s_hid_infor_val),
    },
    /* 特征：HID report map */
    [HID_CHARA_INDEX_REPORT_MAP] =
    {
        .uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_REPORT_MAP),
        .perms = XF_BLE_GATT_ATTR_PERM_READ
        // | XF_BLE_GATT_ATTR_PERM_AUTHEN
        ,    // 需要认证
        .props = HID_REPORT_MAP_PROPERTIES,
        .value = s_hid_report_map_mouse,
        .value_len = sizeof(s_hid_report_map_mouse),
    },
    /* 特征：HID protocol mode */
    [HID_CHARA_INDEX_PROTOCOL_MODE] =
    {
        .uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_PROTOCOL_MODE),
        .perms = XF_BLE_GATT_ATTR_PERM_READ,
        .props = HID_PROTOCOL_MODE_PROPERTIES,
        .value = s_hid_protocol_mode_val,
        .value_len = sizeof(s_hid_protocol_mode_val),
    },
    /* 特征：HID input report (device to host)*/
    [HID_CHARA_INDEX_INPUT_REPORT] =
    {
        .uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_REPORT),
        .perms = XF_BLE_GATT_ATTR_PERM_READ,
        .props = HID_INPUT_REPORT_PROPERTIES,
        .value = s_hid_input_report_val,
        .value_len = sizeof(s_hid_input_report_val),
        .desc_set = (xf_ble_gatts_desc_t [])
        {
            {
                // CCC: Client Characteristic Configuration
                .uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_CLIENT_CHARACTERISTIC_CONFIGURATION),
                .perms = XF_BLE_GATT_ATTR_PERM_READ | XF_BLE_GATT_ATTR_PERM_WRITE,
                .value = s_ccc_val,
                .value_len = sizeof(s_ccc_val),
            }, {
                // report reference, input
                .uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_REPORT_REFERENCE),
                .perms = XF_BLE_GATT_ATTR_PERM_READ,
                .value = s_hid_report_ref_input_val,
                .value_len = sizeof(s_hid_report_ref_input_val),
            },
            {0}
        },
    },
    /* 特征：HID output report (host to device)*/
    [HID_CHARA_INDEX_OUTPUT_REPORT] =
    {
        .uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_REPORT),
        .perms = XF_BLE_GATT_ATTR_PERM_READ,
        .props = HID_OUTPUT_REPORT_PROPERTIES,
        .value = s_hid_output_report_val,
        .value_len = sizeof(s_hid_output_report_val),
        .desc_set = (xf_ble_gatts_desc_t [])
        {
            {
                // report reference, output
                .uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_REPORT_REFERENCE),
                .perms = XF_BLE_GATT_ATTR_PERM_READ,
                .value = s_hid_report_ref_output_val,
                .value_len = sizeof(s_hid_report_ref_output_val),
            },
            {0}
        },
    },
    /* 特征：HID control point */
    [HID_CHARA_INDEX_CTRL_POINT] =
    {
        .uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_HID_CONTROL_POINT),
        .perms = XF_BLE_GATT_ATTR_PERM_READ,
        .props = HID_CONTROL_POINT_PROPERTIES,
        .value = s_hid_control_point_val,
        .value_len = sizeof(s_hid_control_point_val),
    },
    {0}
};

static xf_ble_gatts_service_t s_svc_set = {
    .uuid = XF_BLE_UUID16_DECLARE(BLE_UUID_HUMAN_INTERFACE_DEVICE),
    .type = XF_BLE_GATT_SERVICE_TYPE_PRIMARY,
    .chara_set = s_chara_set,
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
    xf_hal_gpio_set_pull(DEFAULT_INPUT_IO, DEFAULT_INPUT_IO_MODE);
    xf_hal_gpio_set_intr_type(DEFAULT_INPUT_IO, DEFAULT_INPUT_IO_TRIGGER_TYPE);
    xf_hal_gpio_set_intr_isr(DEFAULT_INPUT_IO, sample_gpio_irq, NULL);

    // 注册 GAP 事件回调
    ret = xf_ble_gap_event_cb_register(sample_ble_gap_event_cb, XF_BLE_EVT_ALL);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "REGISTER common event cb failed:%#X", ret);

    // 注册 gatts 事件回调
    ret = xf_ble_gatts_event_cb_register(sample_ble_gatts_event_cb, XF_BLE_EVT_ALL);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "REGISTER gatts event cb failed:%#X", ret);

    /* 设置本地名称、外观 */
    xf_ble_gap_set_local_name(s_gatts_device_name, sizeof(s_gatts_device_name));
    xf_ble_gap_set_local_appearance(XF_BLE_APPEARANCE_HID_MOUSE);

    // 注册 gatts app_profile
    xf_ble_uuid_info_t app_uuid = XF_BLE_UUID16_INIT(0x0000);

    ret = xf_ble_gatts_app_register(&app_uuid, &s_app_id);
    XF_CHECK(ret != XF_OK || s_app_id == 0, XF_RETURN_VOID, TAG,
             "REGISTER app profile failed:%#X s_app_id:%d", ret, s_app_id);

    /* 获取 服务属性的个数 */
    xf_ble_gatts_svc_get_att_cnt(&s_svc_set);
    XF_LOGI(TAG, "att item cnt[%d]", s_svc_set.att_cnt);

    /* 获取 服务属性的映射表 */
    xf_ble_gatts_svc_get_att_local_map(&s_svc_set);

    // 添加服务至 app
    ret = xf_ble_gatts_add_service(
              s_app_id, &s_svc_set);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "ADD service failed:%#X", ret);

    // 启动服务
    ret = xf_ble_gatts_start_service(s_app_id, s_svc_set.handle);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "START service failed:%#X", ret);

    /* 设置广播数据及参数，创建广播 */
    ret = xf_ble_gap_create_adv(&s_adv_id, &s_adv_param, &s_adv_data);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "Create ADV failed:%#X", ret);
    XF_LOGI(TAG, "Create ADV CMPL");

    /* 开启广播 */
    ret = xf_ble_gap_start_adv(s_adv_id, DEFAULT_BLE_GAP_ADV_DURATION_FOREVER);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "START ADV failed:%#X", ret);
    XF_LOGI(TAG, "START ADV CMPL");
}

/* ==================== [Static Functions] ================================== */

static void sample_gpio_irq(xf_gpio_num_t gpio_num, bool level, void *user_data)
{
    XF_LOGI(TAG, "HID report");

    static uint8_t cnt = 0;
    uint8_t base = 1;
    if (cnt++%6 == 0)
    {
        base *= -1;
    }

    s_hid_report_mouse.ac_pan = 0;
    s_hid_report_mouse.button_mask = 0;
    s_hid_report_mouse.wheel = 0;
    s_hid_report_mouse.x = 60*base;
    s_hid_report_mouse.y = 0;

    xf_ble_gatts_ntf_t ntf_param = {
        .value = (uint8_t *)&s_hid_report_mouse,
        .value_len = sizeof(s_hid_report_mouse),
        .handle = s_svc_set.chara_set[HID_CHARA_INDEX_INPUT_REPORT].value_handle,
    };
    xf_err_t ret = xf_ble_gatts_send_notification(
                       s_app_id, s_conn_id, &ntf_param);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "send_notify_indicate failed:%#X", ret);
}

static xf_ble_evt_res_t sample_ble_gap_event_cb(
    xf_ble_gap_evt_t event,
    xf_ble_gap_evt_cb_param_t *param)
{
    UNUSED(s_app_id);
    UNUSED(param);
    switch (event) {
    /* 事件: 连接  */
    case XF_BLE_GAP_EVT_CONNECT: {
        XF_LOGI(TAG, "EV:peer connect:s_app_id:%d,conn_id:%d,"
                "addr_type:%d,addr:"XF_BLE_ADDR_PRINT_FMT,
                s_app_id, param->connect.conn_id,
                param->connect.addr->type,
                XF_BLE_ADDR_EXPAND_TO_ARG(param->connect.addr->addr));

        s_conn_id = param->connect.conn_id;
        xf_hal_gpio_set_intr_enable(DEFAULT_INPUT_IO);
    } break;
    /* 事件: 断连  */
    case XF_BLE_GAP_EVT_DISCONNECT: {
        XF_LOGI(TAG, "EV:peer disconnect:s_app_id:%d,conn_id:%d,reason:%u,"
                "addr_type:%d,addr:"XF_BLE_ADDR_PRINT_FMT,
                s_app_id, param->disconnect.conn_id,
                param->disconnect.reason,
                param->disconnect.addr->type,
                XF_BLE_ADDR_EXPAND_TO_ARG(param->disconnect.addr->addr));
        XF_LOGI(TAG, "It will restart ADV");
        xf_ble_gap_start_adv(s_adv_id, DEFAULT_BLE_GAP_ADV_DURATION_FOREVER);
    } break;
    default:
        return XF_BLE_EVT_RES_NOT_HANDLED;
    }

    return XF_BLE_EVT_RES_HANDLED;
}

static xf_ble_evt_res_t sample_ble_gatts_event_cb(
    xf_ble_gatts_evt_t event,
    xf_ble_gatts_evt_cb_param_t *param)
{
    UNUSED(s_app_id);
    UNUSED(param);
    switch (event) {
    /* 事件: 读请求  */
    case XF_BLE_GATTS_EVT_READ_REQ: {
        XF_LOGI(TAG, "EV:RECV READ_REQ:s_app_id:%d,conn_id:%d,need_rsp:%d,attr_handle:%d",
                s_app_id, param->read_req.conn_id, param->read_req.need_rsp,
                param->read_req.handle);

        xf_ble_gatt_att_num_t chara_index = 0;
        xf_ble_gatt_att_num_t offset = 0;

        xf_ble_gatts_svc_att_get_pos_by_handle(
            &s_svc_set, param->read_req.handle, 
            &chara_index, &offset);

        /* 特征声明 */
        XF_ASSERT(likely(offset != XF_BLE_GATT_CHARA_ATT_OFFSET_DECL), XF_BLE_EVT_RES_ERR, 
            TAG, "hdl(%d) is chara declaration:chara[%d] offset:%d",
                param->read_req.handle, chara_index, offset);

        xf_ble_gatts_response_t rsp = 
        {
            .handle = param->read_req.handle,
            .trans_id = param->read_req.trans_id,
            .err = XF_BLE_ATTR_ERR_SUCCESS,
        };

        /* 特征值 */
        if(offset == XF_BLE_GATT_CHARA_ATT_OFFSET_VALUE)
        {
            rsp.value_len = s_chara_set[chara_index].value_len;
            rsp.value = s_chara_set[chara_index].value;
        }
        /* 描述符 */
        else
        {
            offset = XF_BLE_GATT_CHARA_GET_DESC_INDEX(offset);    // 减去特征声明及特征值声明的句柄占位
            rsp.value_len = s_chara_set[chara_index].desc_set[offset].value_len;
            rsp.value = s_chara_set[chara_index].desc_set[offset].value;
        }
        
        xf_ble_gatts_send_read_rsp(s_app_id, param->read_req.conn_id, &rsp);
    } break;
    /* 事件: 写请求  */
    case XF_BLE_GATTS_EVT_WRITE_REQ: {
        XF_LOGI(TAG, "EV:RECV WRITE_REQ:s_app_id:%d,conn_id:%d,need_rsp:%d,attr_handle:%d",
                s_app_id, param->write_req.conn_id, param->write_req.need_rsp,
                param->write_req.handle);

        if(s_chara_set[HID_CHARA_INDEX_INPUT_REPORT].desc_set[0].handle == param->write_req.handle)
        {
            if(param->write_req.value_len > s_chara_set[HID_CHARA_INDEX_INPUT_REPORT].desc_set[0].value_len)
            {
                XF_LOGE(TAG, "write_req value_len(%d) > desc_set value_len(%d)", 
                    param->write_req.value_len,
                    s_chara_set[HID_CHARA_INDEX_INPUT_REPORT].desc_set[0].value_len);
            }
            xf_memcpy(s_chara_set[HID_CHARA_INDEX_INPUT_REPORT].desc_set[0].value,
                param->write_req.value, param->write_req.value_len);
            
            if (param->write_req.need_rsp == true)
            {
                xf_ble_gatts_response_t rsp = 
                {
                    .handle = param->write_req.handle,
                    .trans_id = param->write_req.trans_id,
                    .err = XF_BLE_ATTR_ERR_SUCCESS,
                    .value = s_chara_set[HID_CHARA_INDEX_INPUT_REPORT].desc_set[0].value,
                    .value_len = param->write_req.value_len,
                };
                xf_ble_gatts_send_write_rsp(s_app_id, param->write_req.conn_id, &rsp);
            }
        }
        
    } break;
    /* 事件: MTU 协商  */
    case XF_BLE_GATTS_EVT_EXCHANGE_MTU: {
        XF_LOGI(TAG, "EV:mtu changed:s_app_id:%d,conn_id:%d,mtu_size:%d",
                s_app_id, param->mtu.conn_id, param->mtu.mtu_size);
    } break;
    default:
        XF_LOGW(TAG, "EV: OTHERS:%d", event);
        return XF_BLE_EVT_RES_NOT_HANDLED;
    }

    return XF_BLE_EVT_RES_HANDLED;
}
