/**
 * @example examples/wireless/sle/sample_ssap_server/main/xf_main.c
 * xf_wal sle ssap 服务端 示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"
#include "xf_sle_connection_manager.h"
#include "xf_sle_device_discovery.h"
#include "xf_sle_ssap_server.h"
#include "string.h"

/* ==================== [Defines] =========================================== */

#define TAG "sample_ssaps"

#define SAMPLE_ADV_ID       1
#define SAMPLE_ADV_HANDLE   1

/* 连接调度间隔12.5ms，单位125us */
#define SAMPLE_SLE_CONN_INTV_MIN                0x64
/* 连接调度间隔12.5ms，单位125us */
#define SAMPLE_SLE_CONN_INTV_MAX                0x64
/* 连接调度间隔25ms，单位125us */
#define SAMPLE_SLE_ADV_INTERVAL_MIN             0xC8
/* 连接调度间隔25ms，单位125us */
#define SAMPLE_SLE_ADV_INTERVAL_MAX             0xC8
/* 超时时间5000ms，单位10ms */
#define SAMPLE_SLE_CONN_SUPERVISION_TIMEOUT     0x1F4
/* 超时时间4990ms，单位10ms */
#define SAMPLE_SLE_CONN_MAX_LATENCY             0
/* 广播发送功率 */
#define SAMPLE_SLE_ADV_TX_POWER  10

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static xf_err_t sample_ssaps_event_cb(
    xf_sle_ssaps_event_t event,
    xf_sle_ssaps_evt_cb_param_t *param);
static void sample_sle_set_adv_data(void);
static void sample_sle_set_adv_param(void);

/* ==================== [Static Variables] ================================== */

static uint8_t s_local_name[] = "XF_SSAPS";
static xf_sle_uuid_info_t s_app_uuid = {0};
static uint8_t s_aap_id = 0;
static uint8_t s_rw_prop_value[8] = {0xCC};
static uint8_t s_rw_desc_value[] = {0xC1, 0x0};

static uint8_t s_ntf_prop_value[8] = {0xDD};
static uint8_t s_ntf_desc_value[] = {0xD1, 0x0};

static uint8_t s_rsp_value[] = "I M SSAPS RSP";
static uint8_t s_ntf0_value[] = "I M SSAPS NTF 0";
static uint8_t s_ntf1_value[] = "I M SSAPS NTF 1";

static uint8_t s_prop_ntf_value[8] = {0xBB};
static uint8_t s_desc_ntf_value[] = {0xB1, 0x0};

static uint8_t s_sle_mac_addr[XF_SLE_ADDR_LEN] = {0x33, 0x22, 0x22, 0x66, 0x11, 0x22};
static bool is_connected = false;

static xf_sle_ssaps_service_t service_struct = {
    .service_uuid = XF_SLE_DECLARE_UUID16(0x2222),
    .service_type = XF_SLE_SSAP_SERVICE_TYPE_PRIMARY,
    .prop_set = (xf_sle_ssaps_property_t [])
    {
        {
            .prop_uuid = XF_SLE_DECLARE_UUID16(0x2323),
            .permissions = (XF_SLE_SSAP_PERMISSION_READ | XF_SLE_SSAP_PERMISSION_WRITE),
            .operate_indication = (
                    XF_SLE_SSAP_OPERATE_INDICATION_BIT_READ 
                |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE 
                |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE_NO_RSP
                |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_NOTIFY
                |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_INDICATE),
            .value = s_rw_prop_value,
            .value_len = sizeof(s_rw_prop_value),
            .desc_set = (xf_sle_ssaps_desc_t []) {
                {
                    .permissions = (XF_SLE_SSAP_PERMISSION_READ | XF_SLE_SSAP_PERMISSION_WRITE),
                    .desc_type = XF_SLE_SSAP_DESC_TYPE_CLIENT_CONFIGURATION,
                    .operate_indication = (
                            XF_SLE_SSAP_OPERATE_INDICATION_BIT_READ 
                        |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE 
                        |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE_NO_RSP
                        |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_NOTIFY
                        |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_INDICATE),
                    .value = s_rw_desc_value,
                    .value_len = sizeof(s_rw_desc_value),
                },
                {0}
            },
        },
        {
            .prop_uuid = XF_SLE_DECLARE_UUID16(0x2324),
            .permissions = (XF_SLE_SSAP_PERMISSION_READ | XF_SLE_SSAP_PERMISSION_WRITE),
            .operate_indication = (
                    XF_SLE_SSAP_OPERATE_INDICATION_BIT_READ 
                |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE 
                |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE_NO_RSP
                |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_NOTIFY
                |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_INDICATE),
            .value = s_ntf_prop_value,
            .value_len = sizeof(s_ntf_prop_value),
            .desc_set = (xf_sle_ssaps_desc_t []) {
                {
                    .permissions = (XF_SLE_SSAP_PERMISSION_READ | XF_SLE_SSAP_PERMISSION_WRITE),
                    .desc_type = XF_SLE_SSAP_DESC_TYPE_CLIENT_CONFIGURATION,
                    .operate_indication = (
                            XF_SLE_SSAP_OPERATE_INDICATION_BIT_READ 
                        |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE 
                        |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE_NO_RSP
                        |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_NOTIFY
                        |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_INDICATE),
                    .value = s_ntf_desc_value,
                    .value_len = sizeof(s_ntf_desc_value),
                },
                {0}
            },
        },
        {0}
        
    }
};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    XF_LOGI(TAG, "XF SLE SSAP Server");

    xf_err_t ret = XF_OK;
    ret = xf_sle_enable();
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_enable error!:%#X", ret);

    // 注册 ssaps 事件回调
    xf_sle_ssaps_event_cb_register(sample_ssaps_event_cb, XF_SLE_EVT_ALL);

    // 设置本地设备名
    ret = xf_sle_set_local_name(s_local_name, sizeof(s_local_name));
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_set_local_name error:%#X", ret);

    // 注册 ssaps 服务端 app
    ret = xf_sle_ssaps_app_register(&s_app_uuid, &s_aap_id);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_set_local_name error:%#X", ret);

    // 添加 服务（结构）到 服务端 app
    ret = xf_sle_ssaps_add_service_to_app(s_aap_id, &service_struct);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_ssaps_add_service_to_app error:%#X", ret);
    XF_LOGI(TAG, "property handle:[0]:%d,uuid:%#X",
            service_struct.prop_set[0].prop_handle,
            service_struct.prop_set[0].prop_uuid->uuid16);

    // 启动服务
    ret = xf_sle_ssaps_start_service(s_aap_id, service_struct.service_handle);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_ssaps_start_service error:%#X", ret);

    /* 设置广播数据及参数 */
    sample_sle_set_adv_param();
    sample_sle_set_adv_data();

    // 启动广播
    ret = xf_sle_start_announce(SAMPLE_ADV_ID);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_start_announce error:%#X", ret);
    XF_LOGI(TAG, "ADV STAR CMPL");
}

/* ==================== [Static Functions] ================================== */

static xf_err_t sample_ssaps_event_cb(
    xf_sle_ssaps_event_t event,
    xf_sle_ssaps_evt_cb_param_t *param)
{
    switch (event) {
    case XF_SLE_COMMON_EVT_DISCONNECT: {
        is_connected = false;
        XF_LOGI(TAG, "EV:disconnect, it will restart ADV");
        // 重新启动广播
        xf_err_t ret = xf_sle_start_announce(SAMPLE_ADV_ID);
        XF_CHECK(ret != XF_OK, ret, TAG,
                 "xf_sle_start_announce error:%#X", ret);
    } break;
    case XF_SLE_COMMON_EVT_CONNECT: {
        XF_LOGI(TAG, "EV:connect!");
        is_connected = true;
    } break;
    case XF_SLE_SSAPS_EVT_WRITE_REQ: {
        XF_LOGI(TAG, "EV:REQ WRITE:need_rsp:%d,hdl:%d,conn_id:%d,trans_id:%d",
                param->write_req.need_rsp, param->write_req.handle,
                param->write_req.conn_id, param->write_req.trans_id);
        XF_LOG_BUFFER_HEXDUMP_ESCAPE(param->write_req.value, param->write_req.value_len);
        XF_LOGI(TAG, ">>> [0].HDL:%d", service_struct.prop_set[0].prop_handle);
        xf_sle_ssaps_ntf_ind_t param_ntf = 
        {
            .handle = param->write_req.handle,
            .type = XF_SLE_SSAP_PROPERTY_TYPE_VALUE,
            .value = s_ntf0_value,
            .value_len = sizeof(s_ntf0_value),
        };
        xf_err_t ret = xf_sle_ssaps_send_notify_indicate(s_aap_id, param->write_req.conn_id, &param_ntf);
        XF_CHECK(ret != XF_OK, ret, TAG,
                 "xf_sle_ssaps_send_notify_indicate error:%#X", ret);
    } break;
    case XF_SLE_SSAPS_EVT_READ_REQ: {
        XF_LOGI(TAG, "EV:REQ READ:need_rsp:%d,hdl:%d,conn_id:%d,trans_id:%d",
                param->read_req.need_rsp, param->read_req.handle,
                param->read_req.conn_id, param->read_req.trans_id);

        XF_LOGI(TAG, ">>> [1].HDL:%d", service_struct.prop_set[1].prop_handle);
        xf_sle_ssaps_ntf_ind_t param_ntf = 
        {
            .handle = service_struct.prop_set[1].prop_handle,
            .type = XF_SLE_SSAP_PROPERTY_TYPE_VALUE,
            .value = s_ntf1_value,
            .value_len = sizeof(s_ntf1_value),
        };
        xf_err_t ret = xf_sle_ssaps_send_notify_indicate(s_aap_id, param->read_req.conn_id, &param_ntf);
        XF_CHECK(ret != XF_OK, ret, TAG,
                 "xf_sle_ssaps_send_notify_indicate error:%#X", ret);
    } break;
    default:
        break;
    }
    return XF_OK;
}

static void sample_sle_set_adv_data(void)
{
    xf_sle_announce_data_t announce_data = {
        .announce_struct_set = (xf_sle_adv_struct_t [])
        {
            {
                .ad_type = XF_SLE_ADV_STRUCT_TYPE_DISCOVERY_LEVEL,
                .ad_data.discovery_level = XF_SLE_ANNOUNCE_LEVEL_NORMAL,
                .ad_data_len = 1,
                .is_ptr = false,
            },
            {
                .ad_type = XF_SLE_ADV_STRUCT_TYPE_COMPLETE_LOCAL_NAME,
                .ad_data.local_name = s_local_name,
                .ad_data_len = sizeof(s_local_name),
                .is_ptr = true,
            }, 
            {0}
        },
        .seek_rsp_struct_set = (xf_sle_adv_struct_t [])
        {
            {0}
        },
    };

    xf_err_t ret = xf_sle_set_announce_data(SAMPLE_ADV_ID, &announce_data);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "ADV set data error:%#X", ret);
}

static void sample_sle_set_adv_param(void)
{
    xf_sle_announce_param_t announce_param = {0};
    {
        announce_param.announce_type = XF_SLE_ANNOUNCE_TYPE_CONNECTABLE_SCANABLE;
        announce_param.announce_handle = SAMPLE_ADV_HANDLE;
        announce_param.announce_gt_role = XF_SLE_ANNOUNCE_ROLE_T_CAN_NEGO;
        announce_param.announce_level = XF_SLE_ANNOUNCE_LEVEL_NORMAL;
        announce_param.announce_channel_map = XF_SLE_ADV_CHANNEL_MAP_DEFAULT;

        announce_param.announce_interval_min = SAMPLE_SLE_ADV_INTERVAL_MIN;
        announce_param.announce_interval_max = SAMPLE_SLE_ADV_INTERVAL_MAX;

        announce_param.conn_interval_min = SAMPLE_SLE_CONN_INTV_MIN;
        announce_param.conn_interval_max = SAMPLE_SLE_CONN_INTV_MAX;
        announce_param.conn_max_latency = SAMPLE_SLE_CONN_MAX_LATENCY;
        announce_param.conn_supervision_timeout = SAMPLE_SLE_CONN_SUPERVISION_TIMEOUT;
    };
    xf_memcpy(announce_param.own_addr.addr, s_sle_mac_addr, XF_SLE_ADDR_LEN);

    xf_err_t ret = xf_sle_set_announce_param(SAMPLE_ADV_ID, &announce_param);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "ADV set param error:%#X", ret);
}
