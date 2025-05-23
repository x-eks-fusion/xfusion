/**
 * @example examples/wireless/sle/sample_ssap_client/main/xf_main.c
 * xf_wal sle ssap 客户端 示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"
#include "xf_sle_connection_manager.h"
#include "xf_sle_device_discovery.h"
#include "xf_sle_ssap_client.h"
#include "string.h"
#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define TAG "sample_ssapc"

#define SAMPLE_SLE_SEEK_INTERVAL    100
#define SAMPLE_SLE_SEEK_WINDOW      100

#define TASK_PRIORITY   5
#define TASK_DELAY_MS   500

#define PROP_INDEX_S2M      0
#define PROP_INDEX_M2S      1

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void sle_client_task(xf_task_t task);
static xf_err_t sample_ssapc_event_cb(
    xf_sle_ssapc_event_t event,
    xf_sle_ssapc_evt_cb_param_t *param);
static void sample_sle_set_seek_param(void);
static xf_err_t ssapc_event_seek_result_cb(
    xf_sle_common_evt_param_seek_result_t *result);

/* ==================== [Static Variables] ================================== */

/* 目标 device name，用于过滤扫描到的广播
    最后仅会连接该 device name 设备 */
static uint8_t target_device_name[] = "XF_SSAPS";

static xf_sle_uuid_info_t s_app_uuid = {0};
static uint8_t s_app_id = 0;
static uint8_t s_conn_id = 0;
static xf_sle_ssapc_find_struct_param_t service_struct = {
    .type = XF_SLE_SSAP_FIND_TYPE_PRIMARY_SERVICE,
    .start_hdl = 1,
    .end_hdl = 0XFFFF,
};

static xf_sle_ssapc_find_struct_param_t prop_struct = {
    .type = XF_SLE_SSAP_FIND_TYPE_PROPERTY,
    .start_hdl = 1,
    .end_hdl = 0XFFFF,
};

static bool is_need_discovery = false;
static bool is_discovery_cmpl = false;
static bool is_write_cmpl = false;

static uint16_t s_prop_hdl[2] = {0};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    XF_LOGI(TAG, "XF SLE SSAP CLient");

    xf_err_t ret = XF_OK;
    ret = xf_sle_enable();
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_enable error!:%#X", ret);

    xf_sle_ssapc_event_cb_register(sample_ssapc_event_cb, XF_SLE_EVT_ALL);

    // 注册 ssaps 客户端 app
    ret = xf_sle_ssapc_app_register(&s_app_uuid, &s_app_id);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_ssapc_app_register error:%#X", ret);
    XF_LOGI(TAG, ">> register app:aap_id:%d", s_app_id);

    /* 设置扫描参数 */
    sample_sle_set_seek_param();

    // 启动扫描
    ret = xf_sle_start_seek();
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_start_seek error:%#X", ret);
    XF_LOGI(TAG, ">> STAR seek CMPL");

    xf_ttask_create_loop(sle_client_task, NULL, TASK_PRIORITY, TASK_DELAY_MS);

}

/* ==================== [Static Functions] ================================== */

static void sle_client_task(xf_task_t task)
{
    xf_err_t ret;
    if (is_need_discovery == true) {
        is_need_discovery = false;
        ret = xf_sle_ssapc_discover_service(s_app_id, s_conn_id, &service_struct);
        if (ret != XF_OK) {
            XF_LOGE(TAG, ">> FIND service error: %d", ret);
            return;
        }
        XF_LOGI(TAG, "service:uuid:%#X,hdl[%d,%d]", service_struct.uuid.uuid16,
                service_struct.start_hdl+1, service_struct.end_hdl);

        s_prop_hdl[PROP_INDEX_S2M] = service_struct.start_hdl;
        s_prop_hdl[PROP_INDEX_M2S] = service_struct.start_hdl+2;
        is_discovery_cmpl = true;
    }
    if (is_discovery_cmpl == true) {
        is_discovery_cmpl = false;
        /* 向对端服务端发送 写请求 */
        is_write_cmpl = false;
        XF_LOGI(TAG, ">> request write data,app_id:%d,conn_id:%d,hdl:%u",
                s_app_id, s_conn_id, s_prop_hdl[PROP_INDEX_M2S]);
        uint8_t data_write[] = "I M SSAPC WRITE REQ!";
        ret = xf_sle_ssapc_request_write_cmd(s_app_id, s_conn_id,
                                              s_prop_hdl[PROP_INDEX_M2S], XF_SLE_SSAP_PROPERTY_TYPE_VALUE,
                                              data_write, sizeof(data_write));
        if (ret != XF_OK) {
            XF_LOGE(TAG, ">> request write cmd error: %#X", ret);
            return;
        }
    } else if (is_write_cmpl == true) {
        /* 向对端服务端发送 读请求 */
        XF_LOGI(TAG, ">> request read,app_id:%d,conn_id:%d,hdl:%u",
                s_app_id, s_conn_id, s_prop_hdl[PROP_INDEX_S2M]);
        ret = xf_sle_ssapc_request_read_by_handle(s_app_id, s_conn_id,
              XF_SLE_SSAP_DESCRIPTOR_CLIENT_CONFIGURATION, s_prop_hdl[PROP_INDEX_S2M]);
        if (ret != XF_OK) {
            XF_LOGE(TAG, ">> request write cmd error: %#X", ret);
            return;
        }
        is_write_cmpl = false;
    }
}

static void sample_sle_set_seek_param(void)
{
    xf_sle_seek_param_t seek_param = {
        .filter_duplicates = true,
        .seek_filter_policy = XF_SLE_SEEK_FILTER_ALLOW_ALL,
        .seek_phy = XF_SLE_SEEK_PHY_1M,
        .own_addr_type = XF_SLE_ADDRESS_TYPE_PUBLIC,

        .phy_param_set[0] =
        {
            .seek_type = XF_SLE_SEEK_TYPE_PASSIVE,
            .seek_interval = SAMPLE_SLE_SEEK_INTERVAL,
            .seek_window = SAMPLE_SLE_SEEK_WINDOW
        }
    };
    xf_err_t ret = xf_sle_set_seek_param(&seek_param);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_set_seek_param error:%#X", ret);
}

static xf_err_t sample_ssapc_event_cb(
    xf_sle_ssapc_event_t event,
    xf_sle_ssapc_evt_cb_param_t *param)
{
    switch (event) {
    case XF_SLE_COMMON_EVT_CONNECT: {
        is_need_discovery = true;
        s_conn_id = param->connect.conn_id;
        XF_LOGI(TAG, "EV:connect:conn_id:%u," XF_SLE_ADDR_PRINT_FMT,
                s_conn_id, XF_SLE_ADDR_EXPAND_TO_ARG(param->connect.peer_addr.addr));
    } break;
    case XF_SLE_COMMON_EVT_SEEK_RESULT: {
        ssapc_event_seek_result_cb(&param->seek_result);
    } break;
    case XF_SLE_SSAPC_EVT_WRITE_CFM: {
        XF_LOGI(TAG, "EV:WRITE confirm:conn_id:%d,hdl:%d",
                param->write_cfm.conn_id, param->write_cfm.handle);
        is_write_cmpl = true;
    } break;
    case XF_SLE_SSAPC_EVT_READ_CFM: {
        XF_LOGI(TAG, "EV:READ confirm:conn_id:%d,hdl:%d",
                param->read_cfm.conn_id, param->read_cfm.handle);
        XF_LOG_BUFFER_HEXDUMP_ESCAPE(param->read_cfm.data, param->read_cfm.data_len);
    } break;
    case XF_SLE_SSAPC_EVT_NOTIFICATION: {
        XF_LOGI(TAG, "EV:RECV NTF or IND:conn_id:%d,hdl:%d",
                param->ntf.conn_id, param->ntf.handle);
        XF_LOG_BUFFER_HEXDUMP_ESCAPE(param->ntf.data, param->ntf.data_len);
    } break;
    default:
        XF_LOGD(TAG, "EV: evt_code:%u", event);
        break;
    }
    return XF_OK;
}

static xf_err_t ssapc_event_seek_result_cb(
    xf_sle_common_evt_param_seek_result_t *result)
{
    xf_err_t ret = XF_OK;

    uint8_t *adv_data_all = result->data;
    uint16_t adv_size_all = result->data_len;

    XF_LOGI(TAG, "EV:seek result:evt_type:%d,rssi:%d"
        XF_SLE_ADDR_PRINT_FMT " data status:%d",
        result->evt_type, result->rssi,
        XF_SLE_ADDR_EXPAND_TO_ARG(result->peer_addr.addr),
        result->data_status);

    uint8_t *adv_pos = adv_data_all;
    uint8_t *adv_end = adv_data_all + adv_size_all;
    while (adv_pos <= adv_end) {
        uint8_t ad_data_len = adv_pos[1];
        xf_sle_adv_struct_type_t ad_type = adv_pos[0];
        XF_LOGD(TAG, "EV:scan_result:ad_type:%#2X, ad_data_len:%d",
                ad_type, ad_data_len);
        switch (ad_type) {
        case XF_SLE_ADV_STRUCT_TYPE_COMPLETE_LOCAL_NAME: {
            uint8_t *local_name = &adv_pos[2];
            if (strncmp((char *)target_device_name, (char *)local_name, ad_data_len) == 0) {

                XF_LOGD(TAG, "EV:seek result:evt_type:%d,rssi:%d"
                        XF_SLE_ADDR_PRINT_FMT " data status:%d",
                        result->evt_type, result->rssi,
                        XF_SLE_ADDR_EXPAND_TO_ARG(result->peer_addr.addr),
                        result->data_status);
                XF_LOGI(TAG, "> target name:%s", local_name);
                ret = xf_sle_stop_seek();
                XF_CHECK(ret != XF_OK, ret, TAG, "xf_sle_stop_seek failed:%#X", ret);
                ret = xf_sle_connect(&result->peer_addr);
                XF_CHECK(ret != XF_OK, ret, TAG, "xf_sle_connect failed:%#X", ret);
            }
        } break;
        default: {
            XF_LOGD(TAG, "EV:scan_result:uncaring ad_type:%#2X", ad_type);
        } break;
        }
        adv_pos += (ad_data_len + XF_SLE_ADV_STRUCT_TYPE_FILED_SIZE + XF_SLE_ADV_STRUCT_LEN_FILED_SIZE);
    }
    return XF_OK;
}
