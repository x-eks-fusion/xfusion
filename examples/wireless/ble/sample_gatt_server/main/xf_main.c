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

#define TAG "gatts"

#define DEFAULT_BLE_GAP_ADV_MIN_INTERVAL 0x30   // BLE GAP 广播最小间隔
#define DEFAULT_BLE_GAP_ADV_MAX_INTERVAL 0x60   // BLE GAP 广播最大间隔
#define DEFAULT_BLE_GAP_ADV_DURATION_FOREVER 0  // BLE GAP 广播持续时间: 永久

#define DEFAULT_SERVICE_UUID        (0x6677)    // 服务 UUID
#define DEFAULT_CHARA_READ_UUID     (0xAABB)    // 特征 UUID , 用于读
#define DEFAULT_CHARA_WRITE_UUID    (0xCCDD)    // 特征 UUID , 用于写

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static xf_ble_evt_res_t sample_ble_gap_event_cb(
    xf_ble_gap_evt_t event,
    xf_ble_gap_evt_cb_param_t *param);

static xf_ble_evt_res_t sample_ble_gatts_event_cb(
    xf_ble_gatts_evt_t event,
    xf_ble_gatts_evt_cb_param_t *param);

/* ==================== [Static Variables] ================================== */

static uint8_t s_adv_id = XF_BLE_ADV_ID_INVALID;
static xf_ble_uuid_info_t s_app_uuid = XF_BLE_UUID16_INIT(0x0000);
static uint8_t s_app_id = XF_BLE_APP_ID_INVALID;

uint8_t sample_gatts_device_name[] = "XF_GATTS";
uint8_t sample_adv_data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
uint8_t sample_adv_rsp_data[] = {0x11, 0x00, 0x00, 0x00, 0x00, 0x66};

static uint8_t s_chara_value_set[][10] = {
    {0x12, 0xAB},
    {0x34, 0xCD}
};

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
    .tx_power = 1,
};

static xf_ble_gap_adv_data_t s_adv_data = {
    .adv_struct_set = (xf_ble_gap_adv_struct_t [])
    {
        {
            .ad_type = XF_BLE_ADV_STRUCT_TYPE_FLAGS,
            .ad_data_len = sizeof(uint8_t),
            .ad_data.flag = (1 << 2) | (1 << 0),
            .is_ptr = false,
        }, 
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
};

static xf_ble_gatts_chara_t s_chara_set[] = {
    {
        .uuid = XF_BLE_UUID16_DECLARE(DEFAULT_CHARA_READ_UUID),
        .value = s_chara_value_set[0],
        .value_len = sizeof(s_chara_value_set[0]),
        .perms = XF_BLE_GATT_ATTR_PERM_READ,
        .props = XF_BLE_GATT_CHARA_PROP_READ, 
    },
    {
        .uuid = XF_BLE_UUID16_DECLARE(DEFAULT_CHARA_WRITE_UUID),
        .value = s_chara_value_set[1],
        .value_len = sizeof(s_chara_value_set[1]),
        .perms = XF_BLE_GATT_ATTR_PERM_READ | XF_BLE_GATT_ATTR_PERM_WRITE,
        .props = XF_BLE_GATT_CHARA_PROP_WRITE | XF_BLE_GATT_CHARA_PROP_WRITE_NO_RSP,
    },
    {0},    // 特征集合最后加入一个空元素，用于标记特征集合结束  
};

static xf_ble_gatts_service_t s_svc_set[] = {
    {
        .uuid = XF_BLE_UUID16_DECLARE(DEFAULT_SERVICE_UUID),
        .type = XF_BLE_GATT_SERVICE_TYPE_PRIMARY,
        .chara_set = s_chara_set,
    },
    {0} // 服务集合最后加入一个空元素，用于服务特征集合结束  
};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    XF_LOGI(TAG, "xf ble gatt server sample");
    xf_err_t ret = XF_OK;
    // 使能 ble
    xf_ble_enable();

    // 注册 GAP 事件回调
    ret = xf_ble_gap_event_cb_register(sample_ble_gap_event_cb, XF_BLE_EVT_ALL);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "REGISTER common event cb failed:%#X", ret);

    // 注册 gatts 事件回调
    ret = xf_ble_gatts_event_cb_register(sample_ble_gatts_event_cb, XF_BLE_EVT_ALL);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "REGISTER gatts event cb failed:%#X", ret);

    /* 设置本地名称、外观 */
    xf_ble_gap_set_local_name(sample_gatts_device_name, sizeof(sample_gatts_device_name));
    xf_ble_gap_set_local_appearance(XF_BLE_APPEARANCE_HID_DIGITIZER_TABLET);

    // 注册 gatts app_profile
    ret = xf_ble_gatts_app_register(&s_app_uuid, &s_app_id);
    XF_CHECK(ret != XF_OK || s_app_id == 0, XF_RETURN_VOID, TAG,
             "REGISTER app profile failed:%#X s_app_id:%d", ret, s_app_id);

    /* 获取 服务属性的个数 */
    xf_ble_gatts_svc_get_att_cnt(&s_svc_set[0]);
    XF_LOGI(TAG, "att item cnt[%d]", s_svc_set[0].att_cnt);

    /* 获取 服务属性的映射表 */
    xf_ble_gatts_svc_get_att_local_map(&s_svc_set[0]);

    // 添加服务至 app_profile
    ret = xf_ble_gatts_add_service(
              s_app_id, &s_svc_set[0]);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "ADD service failed:%#X", ret);

    // 启动服务
    ret = xf_ble_gatts_start_service(s_app_id, s_svc_set[0].handle);
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
            &s_svc_set[0], param->read_req.handle, 
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
            rsp.value = s_chara_set[chara_index].value;
            rsp.value_len = s_chara_set[chara_index].value_len;
        }
        /* 描述符 */
        else
        {
            offset = XF_BLE_GATT_CHARA_GET_DESC_INDEX(offset);    // 减去特征声明及特征值声明的句柄占位
            rsp.value = s_chara_set[chara_index].desc_set[offset].value;
            rsp.value_len = s_chara_set[chara_index].desc_set[offset].value_len;
        }
        
        xf_ble_gatts_send_read_rsp(s_app_id, param->read_req.conn_id, &rsp);
    } break;
    /* 事件: 写请求  */
    case XF_BLE_GATTS_EVT_WRITE_REQ: {
        XF_LOGI(TAG, "EV:RECV WRITE_REQ:s_app_id:%d,conn_id:%d,need_rsp:%d,attr_handle:%d",
                s_app_id, param->write_req.conn_id, param->write_req.need_rsp,
                param->write_req.handle);
        XF_LOG_BUFFER_HEXDUMP(param->write_req.value, param->write_req.value_len);

        /* 如果是需要响应的请求类型->返回响应 */
        if (param->write_req.need_rsp == true) {

            xf_ble_gatt_att_num_t chara_index = 0;
            xf_ble_gatt_att_num_t offset = 0;

            xf_ble_gatts_svc_att_get_pos_by_handle(
                &s_svc_set[0], param->read_req.handle, 
                &chara_index, &offset);

            /* 特征声明 */
            XF_ASSERT(likely(offset != XF_BLE_GATT_CHARA_ATT_OFFSET_DECL), XF_BLE_EVT_RES_ERR, 
                TAG, "hdl(%d) is chara declaration:chara[%d] offset:%d",
                    param->read_req.handle, chara_index, offset);
            uint8_t *value = NULL;
            uint32_t value_len = 0;
            /* 特征值 */
            if(offset == XF_BLE_GATT_CHARA_ATT_OFFSET_VALUE)
            {
                xf_memcpy(s_chara_set[chara_index].value,
                    param->write_req.value, param->write_req.value_len);
                value = s_chara_set[chara_index].value;
                value_len = param->write_req.value_len;

            }
            /* 描述符 */
            else
            {
                offset = XF_BLE_GATT_CHARA_GET_DESC_INDEX(offset);    // 减去特征声明及特征值声明的句柄占位
                xf_memcpy(s_chara_set[chara_index].desc_set[offset].value,
                    param->write_req.value, param->write_req.value_len);
                value = s_chara_set[chara_index].desc_set[offset].value;
                value_len = param->write_req.value_len;
            }

            if (param->write_req.need_rsp == true)
            {
                xf_ble_gatts_response_t rsp = 
                {
                    .handle = param->write_req.handle,
                    .trans_id = param->write_req.trans_id,
                    .err = XF_BLE_ATTR_ERR_SUCCESS,
                    .value = value,
                    .value_len = value_len,
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
        return XF_BLE_EVT_RES_NOT_HANDLED;
    }

    return XF_BLE_EVT_RES_HANDLED;
}
