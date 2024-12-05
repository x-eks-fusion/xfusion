/**
 * @example examples/wireless/ble/sample_gatt_client/main/xf_main.c
 * xf_wal ble gatt 客户端 示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"
#include "xf_ble_gap.h"
#include "xf_ble_gatt_client.h"
#include "string.h"
#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define TAG "sample_gattc"

/* ==================== [Typedefs] ========================================== */

/* Ble scan interval */
#define DEFAULT_BLE_GAP_SCAN_INTERVAL   0x48
#define DEFAULT_BLE_GAP_SCAN_WINDOW     0x48

#define TASK_PRIORITY   5
#define TASK_DELAY_MS   500

#define DEFAULT_SERVICE_INDEX       (1)
#define DEFAULT_CHARA_READ_INDEX    (0)
#define DEFAULT_CHARA_WRITE_INDEX   (1)

/* ==================== [Static Prototypes] ================================= */

static xf_err_t sample_ble_set_scan_param(void);
static xf_err_t sample_ble_gattc_event_cb(
    xf_ble_gattc_event_t event,
    xf_ble_gattc_evt_cb_param_t param);
static xf_err_t gattc_event_scan_result_cb(xf_ble_gattc_evt_cb_param_t *param);

/* ==================== [Static Variables] ================================== */

/* 目标 ble device name，用于过滤扫描到的广播
    最后仅会连接该 device name 设备
*/
static uint8_t target_gatts_device_name[] = "XF_GATTS";
static uint8_t write_req_data[] = "I M GATTC";
static xf_ble_uuid_info_t app_uuid = {
    .len_type = XF_BLE_UUID_TYPE_16,
    .uuid16 = 0x0000
};
static uint8_t s_app_id = 0;
static uint8_t s_conn_id = 0;
static bool is_need_discovery = false;
static bool is_discovery_cmpl = false;
static bool is_read_cmpl = false;
static xf_ble_gattc_service_found_set_t *service_found_set = NULL;
static xf_ble_addr_t peer_addr = {0};

static void discovery_task(xf_task_t task);
static void read_write_task(xf_task_t task);

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    XF_LOGI(TAG, "xf ble gatt client sample");
    xf_err_t ret = XF_OK;
    // 使能 ble
    xf_ble_enable();

    // 注册 gattc 事件回调
    ret = xf_ble_gattc_event_cb_register(sample_ble_gattc_event_cb, XF_BLE_COMMON_EVT_ALL);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "REGISTER event cb failed:%#X", ret);

    // 注册 gattc app_profile
    ret = xf_ble_gattc_app_register(&app_uuid, &s_app_id);
    XF_CHECK(ret != XF_OK || s_app_id == 0, XF_RETURN_VOID, TAG,
             "REGISTER app profile failed:%#X app_id:%d", ret, s_app_id);

    // 设置扫描参数
    ret = sample_ble_set_scan_param();
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "SET scan param failed:%#X", ret);
    XF_LOGI(TAG, "SET scan param CMPL");

    // 开启扫描
    ret = xf_ble_gap_start_scan();
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "STAR scan failed:%#X", ret);
    XF_LOGI(TAG, "STAR scan CMPL");

    // 创建发现任务
    xf_ntask_create(discovery_task, NULL, TASK_PRIORITY, TASK_DELAY_MS,
                    XF_NTASK_INFINITE_LOOP);
    // 创建读写任务
    xf_ntask_create(read_write_task, NULL, TASK_PRIORITY, TASK_DELAY_MS,
                    XF_NTASK_INFINITE_LOOP);
}

/* ==================== [Static Functions] ================================== */

static xf_err_t sample_ble_gattc_event_cb(
    xf_ble_gattc_event_t event,
    xf_ble_gattc_evt_cb_param_t param)
{
    UNUSED(param);
    xf_err_t ret = XF_OK;
    switch (event) {
    case XF_BLE_COMMON_EVT_SCAN_RESULT: {
        ret = gattc_event_scan_result_cb(&param);
    } break;
    case XF_BLE_COMMON_EVT_CONNECT: {
        s_conn_id = param.connect.conn_id;
        peer_addr = param.connect.peer_addr;

        XF_LOGI(TAG, "EV:connect:s_conn_id:%d,addr_type:%u,addr:"
                XF_BLE_ADDR_PRINT_FMT,
                s_conn_id, peer_addr.type,
                XF_BLE_ADDR_EXPAND_TO_ARG(peer_addr.addr));
        is_need_discovery = true;
    } break;
    case XF_BLE_GATTC_EVT_READ_CFM: {
        if (param.read_cfm.value_len != 0) {
            XF_LOGI(TAG, "EV:read CMPL:handle:%d", param.read_cfm.handle);
            XF_LOG_BUFFER_HEXDUMP(param.read_cfm.value,
                                  param.read_cfm.value_len);
            is_read_cmpl = true;
        }
    } break;
    case XF_BLE_GATTC_EVT_WRITE_CFM: {
        XF_LOGI(TAG, "EV:write CMPL:handle:%d,it will disconnect",
                param.write_cfm.handle);
        xf_ble_gap_disconnect(&peer_addr);
    } break;
    case XF_BLE_GATTC_EVT_NOTIFICATION: {
        if (param.ntf.value_len != 0) {
            XF_LOGI(TAG, "EV:read ntf/ind:chara_handle:%d", param.ntf.handle);
            XF_LOG_BUFFER_HEXDUMP(param.ntf.value,
                                  param.ntf.value_len);
        }
    } break;
    default:
        break;
    }

    return ret;
}

static xf_err_t sample_ble_set_scan_param(void)
{
    xf_ble_gap_scan_param_t scan_param = {
        .scan_interval = DEFAULT_BLE_GAP_SCAN_INTERVAL,
        .scan_window = DEFAULT_BLE_GAP_SCAN_WINDOW,
        .scan_type = XF_BLE_GAP_SCAN_TYPE_PASSIVE,
        .scan_phy = XF_BLE_GAP_PHY_1M,
        .scan_filter_policy = XF_BLE_GAP_SCAN_FILTER_ACCEPT_ALL,
    };
    return xf_ble_gap_set_scan_param(&scan_param);
}

static xf_err_t gattc_event_scan_result_cb(xf_ble_gattc_evt_cb_param_t *param)
{
    xf_err_t ret = XF_OK;
    XF_LOGD(TAG, "EV:scan_result:ADV:addr_type:%d,addr:" XF_BLE_ADDR_PRINT_FMT
            ",RSSI:%d,adv_data_len:%u",
            param->scan_result.addr_scanned.type,
            XF_BLE_ADDR_EXPAND_TO_ARG(param->scan_result.addr_scanned.addr)
           );
    uint8_t *adv_data_all = param->scan_result.adv_data;
    uint16_t adv_size_all = param->scan_result.adv_data_len;
    uint8_t *ptr_current = adv_data_all;
    uint8_t *ptr_end = adv_data_all + adv_size_all;

    /* 解析广播数据，当包含指定设备名的广播时，进行连接 */
    while (ptr_current <= ptr_end) {
        uint8_t struct_data_len = ptr_current[0];
        xf_ble_gap_adv_struct_type_t ad_type = ptr_current[1];
        XF_LOGD(TAG, "EV:scan_result:struct_data_len:%d ad_type:%#2X",
                struct_data_len, ad_type);
        switch (ad_type) {
        case XF_BLE_ADV_STRUCT_TYPE_LOCAL_NAME_ALL: {
            uint8_t *local_name = &ptr_current[2];
            uint8_t local_namez_size = struct_data_len - XF_BLE_GAP_ADV_STRUCT_AD_TYPE_FIELD_SIZE;
            if (strncmp((char *)target_gatts_device_name, (char *)local_name, local_namez_size) == 0) {
                XF_LOGI(TAG, "EV:scan_result:target local name:%s", local_name);
                ret = xf_ble_gap_stop_scan();
                XF_CHECK(ret != XF_OK, ret, TAG, "gap stop scan failed:%#X", ret);
                ret = xf_ble_gap_connect(&param->scan_result.addr_scanned);
                XF_CHECK(ret != XF_OK, ret, TAG, "gap connect failed:%#X", ret);
            }
        } break;
        default: {
            XF_LOGD(TAG, "EV:scan_result:uncaring ad_type:%#2X", ad_type);
        } break;
        }
        ptr_current += (struct_data_len + XF_BLE_GAP_ADV_STRUCT_LEN_FIELD_SIZE);
    }
    return XF_OK;
}

static void discovery_task(xf_task_t task)
{
    /* 检测是否需要执行发现操作（服务、特征、特征描述符（暂时未支持）） -> 尝试进行连接 */
    if (is_need_discovery == true) {
        is_need_discovery = false;
        xf_err_t ret = XF_OK;
        /* 尝试发现服务 */
        XF_LOGI(TAG, "discover service:%p %p", &service_found_set, service_found_set);
        ret = xf_ble_gattc_discover_service(s_app_id, s_conn_id,
                                            NULL, &service_found_set);
        XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG, "discover service failed:%#X app_id:%d",
                 ret, s_app_id);

        XF_LOGI(TAG, "discover chara:service_found_set cnt:%u", service_found_set->cnt);
        uint16_t cnt_service = service_found_set->cnt;
        while (cnt_service-- > 0) {
            /* 尝试发现服务下的特征 */
            xf_ble_gattc_service_found_t *service = &service_found_set->set[cnt_service];
            // XF_LOGI(TAG, ">> service[%d]", cnt_service);
            service->chara_set_info = NULL;
            ret = xf_ble_gattc_discover_chara(s_app_id, s_conn_id,
                                              service->start_hdl, service->end_hdl, NULL, &service->chara_set_info);
            XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
                     "discover chara failed:%#X app_id:%d", ret, s_app_id);

            uint16_t cnt_chara = service->chara_set_info->cnt;

            while (cnt_chara-- > 0) {
                /* 打印服务及其下特征信息 */
                xf_ble_gattc_chara_found_t *chara = &service->chara_set_info->set[cnt_chara];
                XF_LOGI(TAG, "service[%u]:uuid(16):[%#X],handle[%u,%u],chara[%u]:%#X",
                        cnt_service, service->uuid.uuid16,
                        service->start_hdl, service->end_hdl,
                        cnt_chara, chara->uuid.uuid16);
                // /* 尝试发现服务下的特征下的特征描述符 */
                // chara->desc_set_info = NULL;
                // ret = xf_ble_gattc_discover_desc_by_chara_handle(s_app_id, s_conn_id,
                //     service->start_hdl, service->end_hdl, NULL, service->chara_set_info);
                // XF_CHECK(ret != XF_OK, ret, TAG, "xf_ble_gattc_discover_chara failed:%#X app_id:%d",
                //     ret, s_app_id);
            }
            is_discovery_cmpl = true;
        }
    }
}

static void read_write_task(xf_task_t task)
{
    /* 发现（服务、特征、特征能描述符）完成 -> 进行读写测试 */
    if (is_discovery_cmpl == true) {
        is_discovery_cmpl = false;

        xf_err_t ret = XF_OK;

        xf_ble_gattc_service_found_t *service =
            &service_found_set->set[DEFAULT_SERVICE_INDEX];

        XF_LOGI(TAG, "REQ READ(uuid) and WRITE(handle):"
                "s_app_id:%d,s_conn_id:%d"
                "service_uuid(16):%#X,chara_uuid(16):%#X,",
                service->uuid.uuid16,
                s_app_id, s_conn_id,
                service->chara_set_info->set[DEFAULT_CHARA_READ_INDEX].uuid.uuid16
               );
        /* 向对端服务端发送 读请求 第1次 */
        is_read_cmpl = false;
        ret = xf_ble_gattc_request_read_by_handle(s_app_id, s_conn_id,
              service->chara_set_info->set[DEFAULT_CHARA_READ_INDEX].value_handle);
        if (ret != XF_OK) {
            XF_LOGE(TAG, "REQ READ:failed:%#X", ret);
        }
        /* 向对端服务端发送 读请求 第2次 */
        is_read_cmpl = false;
        ret = xf_ble_gattc_request_read_by_handle(s_app_id, s_conn_id,
              service->chara_set_info->set[DEFAULT_CHARA_READ_INDEX].value_handle);
        if (ret != XF_OK) {
            XF_LOGE(TAG, "REQ READ:failed:%#X", ret);
        }
    }
    if (is_read_cmpl == true) {
        xf_ble_gattc_service_found_t *service =
            &service_found_set->set[DEFAULT_SERVICE_INDEX];

        /* 向对端服务端发送 写请求 */
        xf_err_t ret = xf_ble_gattc_request_write(s_app_id, s_conn_id,
                       service->chara_set_info->set[DEFAULT_CHARA_WRITE_INDEX].value_handle,
                       write_req_data, sizeof(write_req_data),
                       XF_BLE_GATT_WRITE_TYPE_WITH_RSP);
        if (ret != XF_OK) {
            XF_LOGE(TAG, "REQ WRITE:failed:%#X", ret);
        }
        is_read_cmpl = false;
    }
}