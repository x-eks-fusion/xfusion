/**
 * @example examples/wireless/ble/sample_gatt_server/main/xf_main.c
 * xf_wal ble gatt 服务端 示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"
#include "xf_ble_gap.h"
#include "xf_ble_gatt_server.h"
#include "string.h"

/* ==================== [Defines] =========================================== */

#define TAG "sample_gatts"

#define DEFAULT_BLE_GAP_ADV_ID  0x01

/* Ble adv min interval */
#define DEFAULT_BLE_GAP_ADV_MIN_INTERVAL 0x30
/* Ble adv max interval */
#define DEFAULT_BLE_GAP_ADV_MAX_INTERVAL 0x60
/* Ble adv handle */
#define DEFAULT_BLE_GAP_ADV_HANDLE_DEFAULT 0x01
/* Ble adv duration */
#define DEFAULT_BLE_GAP_ADV_FOREVER_DURATION 0

/* Server HANDLE */
#define DEFAULT_SERVER_HANDLE   (1)
/* Server ID */
#define DEFAULT_SERVER_ID       (1)
/* Ser UUID */
#define DEFAULT_SERVICE_UUID    (0x1234)
/* Characteristic UUID */
#define DEFAULT_CHARA_UUID      (0xCDEF)
/* Client Characteristic Configuration UUID */
#define DEFAULT_CHARA_CFG_UUID  (0x2902)

uint8_t sample_gatts_device_name[] = "XF_GATTS";
uint8_t sample_adv_data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
uint8_t sample_adv_rsp_data[] = {0x11, 0x00, 0x00, 0x00, 0x00, 0x66};

static uint8_t read_req_indication[] = "REQ_READ RSP";
static uint8_t write_req_indication[] = "REQ_WRITE RSP";

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void sample_ble_set_adv_data(void);

static void sample_ble_set_adv_param(void);

static xf_err_t sample_ble_gatts_event_cb(
    xf_ble_gatts_event_t event,
    xf_ble_gatts_evt_cb_param_t param);

/* ==================== [Static Variables] ================================== */

static uint8_t chara_value_array[][10] = {
    {0x12, 0x34},
    {0xAA, 0xBB}
};

static xf_ble_gatts_service_t service_set[] = {
    {
        .service_uuid = XF_BLE_DECLARE_UUID16(DEFAULT_SERVICE_UUID),
        .service_type = XF_BLE_GATT_SERVICE_TYPE_PRIMARY,
        .chara_set = (xf_ble_gatts_chara_t [])
        {
            {
                .chara_uuid = XF_BLE_DECLARE_UUID16(DEFAULT_CHARA_UUID),
                .chara_value = {
                    .value = chara_value_array[0],
                    .value_len = sizeof(chara_value_array[0]),
                    .permission =
                    (
                    XF_BLE_GATT_ATTR_PERMISSION_READ
                    | XF_BLE_GATT_ATTR_PERMISSION_WRITE
                    ),
                },
                .properties =
                    (
                        XF_BLE_GATT_CHARA_PROPERTY_READ
                        |   XF_BLE_GATT_CHARA_PROPERTY_NOTIFY
                    ),
            },
            {.chara_uuid = XF_BLE_ATTR_SET_END_FLAG}
        }
    },
    {.service_uuid = XF_BLE_ATTR_SET_END_FLAG}
};

static xf_ble_uuid_info_t app_uuid = {
    .len_type = XF_BLE_UUID_TYPE_16,
    .uuid16 = 0x0000
};
static uint8_t app_id = 0;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    XF_LOGI(TAG, "xf ble gatt server sample");
    xf_err_t ret = XF_OK;
    // 使能 ble
    xf_ble_enable();

    // 注册 gatts 事件回调
    ret = xf_ble_gatts_event_cb_register(sample_ble_gatts_event_cb, XF_BLE_COMMON_EVT_ALL);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "REGISTER event cb failed:%#X", ret);

    /* 设置本地名称、外观 */
    xf_ble_gap_set_local_name(sample_gatts_device_name, sizeof(sample_gatts_device_name));
    xf_ble_gap_set_local_appearance(XF_BLE_APPEARANCE_HID_DIGITIZER_TABLET);

    // 注册 gatts app_profile
    ret = xf_ble_gatts_app_register(&app_uuid, &app_id);
    XF_CHECK(ret != XF_OK || app_id == 0, XF_RETURN_VOID, TAG,
             "REGISTER app profile failed:%#X app_id:%d", ret, app_id);

    // 添加服务至 app_profile
    ret = xf_ble_gatts_add_service_to_app(
              app_id, &service_set[0]);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "ADD service failed:%#X", ret);

    // 启动服务
    ret = xf_ble_gatts_start_service(app_id, service_set[0].service_handle);
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

static xf_err_t sample_ble_gatts_event_cb(
    xf_ble_gatts_event_t event,
    xf_ble_gatts_evt_cb_param_t param)
{
    UNUSED(app_id);
    UNUSED(param);
    switch (event) {
    /* 事件: 读请求  */
    case XF_BLE_GATTS_EVT_READ_REQ: {
        XF_LOGI(TAG, "EV:RECV READ_REQ:app_id:%d,conn_id:%d,need_rsp:%d,attr_handle:%d",
                app_id, param.read_req.conn_id, param.read_req.need_rsp,
                param.read_req.handle);

        xf_ble_gatts_ntf_t ntf_param = {
            .value = read_req_indication,
            .value_len = sizeof(read_req_indication),
            .handle = param.read_req.handle
        };
        xf_err_t ret = xf_ble_gatts_send_notification(
                           app_id, param.read_req.conn_id, &ntf_param);
        XF_CHECK(ret != XF_OK, ret, TAG,
                 "send_notify_indicate failed:%#X", ret);
    } break;
    /* 事件: 写请求  */
    case XF_BLE_GATTS_EVT_WRITE_REQ: {
        XF_LOGI(TAG, "EV:RECV WRITE_REQ:app_id:%d,conn_id:%d,need_rsp:%d,attr_handle:%d",
                app_id, param.write_req.conn_id, param.write_req.need_rsp,
                param.write_req.handle);
        XF_LOG_BUFFER_HEXDUMP(param.write_req.value, param.write_req.value_len);

        /* 如果是需要响应的请求类型->返回响应 */
        if (param.write_req.need_rsp == true) {
            xf_ble_gatts_ntf_t ntf_param = {
                .value = write_req_indication,
                .value_len = sizeof(write_req_indication),
                .handle = param.write_req.handle,
            };
            xf_err_t ret = xf_ble_gatts_send_notification(
                               app_id, param.write_req.conn_id, &ntf_param);
            XF_CHECK(ret != XF_OK, ret, TAG,
                     "send_notify_indicate failed:%#X app_id:%d", ret);
        }
    } break;
    /* 事件: MTU 协商  */
    case XF_BLE_GATTS_EVT_EXCHANGE_MTU: {
        XF_LOGI(TAG, "EV:mtu changed:app_id:%d,conn_id:%d,mtu_size:%d",
                app_id, param.mtu.conn_id, param.mtu.mtu_size);
    } break;
    /* 事件: 连接  */
    case XF_BLE_COMMON_EVT_CONNECT: {
        XF_LOGI(TAG, "EV:peer connect:app_id:%d,conn_id:%d,"
                "addr_type:%d,addr:"XF_BLE_ADDR_PRINT_FMT,
                app_id, param.connect.conn_id,
                param.connect.peer_addr.type,
                XF_BLE_ADDR_EXPAND_TO_ARG(param.connect.peer_addr.addr));

        xf_ble_sm_authen_req_t authen_req = XF_BLE_SM_AUTHEN_REQ_SC_MITM_BOND;
        xf_ble_sm_io_cap_t io_capability = XF_BLE_SM_IO_CAP_NONE;
        xf_ble_sm_authen_option_t authen_option = XF_BLE_SM_AUTHEN_OPTION_DISABLE;
        xf_err_t ret = XF_OK;
        ret = xf_ble_gap_set_security_param(
                  XF_BLE_SM_PARAM_AUTHEN_REQ_MODE, &authen_req, sizeof(authen_req));
        XF_CHECK(ret != XF_OK, ret, TAG,
                 "set_security_param failed:%#X", ret);
        ret = xf_ble_gap_set_security_param(
                  XF_BLE_SM_PARAM_IO_CAP_MODE, &io_capability, sizeof(io_capability));
        XF_CHECK(ret != XF_OK, ret, TAG,
                 "set_security_param failed:%#X", ret);
    } break;
    case XF_BLE_COMMON_EVT_PAIR_END: {
        XF_LOGI(TAG, "EV:pair end:app_id:%d,conn_id:%d,"
                "addr_type:%d,addr:"XF_BLE_ADDR_PRINT_FMT,
                app_id, param.connect.conn_id,
                param.connect.peer_addr.type,
                XF_BLE_ADDR_EXPAND_TO_ARG(param.connect.peer_addr.addr));
    } break;
    /* 事件: 断连  */
    case XF_BLE_COMMON_EVT_DISCONNECT: {
        XF_LOGI(TAG, "EV:peer disconnect:app_id:%d,conn_id:%d,reason:%u,"
                "addr_type:%d,addr:"XF_BLE_ADDR_PRINT_FMT,
                app_id, param.disconnect.conn_id,
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
                .ad_type = XF_BLE_ADV_STRUCT_TYPE_LOCAL_NAME_ALL,
                .ad_data_len = sizeof(sample_gatts_device_name),
                .ad_data.local_name = sample_gatts_device_name,
                .is_ptr = true,
            },
            {
                .ad_type = XF_BLE_ADV_STRUCT_TYPE_TX_POWER_LEVEL,
                .ad_data_len = sizeof(uint8_t),
                .ad_data.adv_var.val_u8 = 18,
                .is_ptr = false,
            },
            {0}
        },
        .scan_rsp_struct_set = (xf_ble_gap_adv_struct_t [])
        {
            {
                .ad_type = XF_BLE_ADV_STRUCT_TYPE_APPEARANCE,
                .ad_data_len = sizeof(xf_ble_appearance_t),
                .ad_data.appearance = XF_BLE_APPEARANCE_GENERIC_PHONE,
                .is_ptr = false,
            },
            {0}
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
            .type = XF_BT_ADDR_TYPE_PUBLIC_DEV, // 本端地址类型
            .addr = {0x0},                       // 本端地址
        },
        .peer_addr =
        {
            .type = XF_BT_ADDR_TYPE_PUBLIC_DEV, // 对端地址类型
            .addr = { 0x0 },                    // 对端地址
        },
        .channel_map = XF_BLE_GAP_ADV_CH_ALL,   // 使用37/38/39三个通道
        .adv_filter_policy = XF_BLE_GAP_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
        .duration = DEFAULT_BLE_GAP_ADV_FOREVER_DURATION,
        .tx_power = 1,  // 发送功率,单位dbm,范围-127~20
    };
    xf_ble_gap_set_adv_param(DEFAULT_BLE_GAP_ADV_ID, &adv_param);
}