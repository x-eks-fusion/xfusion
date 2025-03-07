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

#define TAG "gattc"

/* ==================== [Typedefs] ========================================== */

/* Ble scan interval */
#define DEFAULT_BLE_GAP_SCAN_INTERVAL   0x20
#define DEFAULT_BLE_GAP_SCAN_WINDOW     0x60

#define TASK_PRIORITY   5
#define TASK_DELAY_MS   1000

/**
 * @brief 发现用于读写测试的服务结构时限定的属性句柄范围
 * @note 此范围值能有效避免其他服务发现异常或其他意外原因。
 * @note 范围值需根据实际服务端的整个profile的情况进行定义，
 *  默认按 GAP 服务 + GATT 服务 + 读写测试服务的 profile 进行定义
 */
#define DEFAULT_SVC_DISC_HDL_MIN     0x09
#define DEFAULT_SVC_DISC_HDL_MAX     XF_BLE_ATTR_HANDLE_MAX

#define DEFAULT_SERVICE_INDEX       (2)
#define DEFAULT_CHARA_READ_INDEX    (0)
#define DEFAULT_CHARA_WRITE_INDEX   (1)

/* ==================== [Static Prototypes] ================================= */

static xf_ble_evt_res_t sample_ble_gap_event_cb(
    xf_ble_gap_evt_t event,
    xf_ble_gap_evt_cb_param_t *param);
static xf_ble_evt_res_t sample_ble_gattc_event_cb(
    xf_ble_gattc_evt_t event,
    xf_ble_gattc_evt_cb_param_t *param);
static xf_ble_evt_res_t gap_event_scan_result_cb(xf_ble_gap_evt_cb_param_t *param);

/* ==================== [Static Variables] ================================== */

/* 目标 ble device name，用于过滤扫描到的广播
    最后仅会连接广播中有 local_name 广播单元类型 且名字是 device name 设备
*/
static uint8_t s_target_dev_name[] = "XF_GATTS";
static uint8_t s_write_req_data[] = "I M GATTC";
static xf_ble_uuid_info_t s_app_uuid = XF_BLE_UUID16_INIT(0x0000);
static uint8_t s_app_id = XF_BLE_APP_ID_INVALID;
static uint8_t s_conn_id = 0;
static bool is_need_discovery = false;
static bool is_discovery_cmpl = false;
static bool is_read_cmpl = false;
static xf_ble_gattc_service_found_set_t service_found_set = {0};
static xf_ble_addr_t peer_addr = {0};

static void discovery_task(xf_task_t task);
static void read_write_task(xf_task_t task);

static xf_ble_gap_scan_param_t scan_param = {
    .interval = DEFAULT_BLE_GAP_SCAN_INTERVAL,
    .window = DEFAULT_BLE_GAP_SCAN_WINDOW,
    .type = XF_BLE_GAP_SCAN_TYPE_ACTIVE,
    .phy = XF_BLE_GAP_SCAN_PHY_1M,
    .filter_policy = XF_BLE_GAP_SCAN_FILTER_POLICY_ALL,
};

xf_ble_addr_t s_local_addr = {
    .type = XF_BLE_ADDR_TYPE_PUBLIC_DEV,
    .addr = {0xAA, 0x32, 0x20, 0x20, 0x32, 0xEE}
};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    XF_LOGI(TAG, "xf ble gatt client sample");
    xf_err_t ret = XF_OK;
    // 使能 ble
    xf_ble_enable();

    xf_ble_gap_set_local_addr(&s_local_addr);

    // 注册 GAP 事件回调
    ret = xf_ble_gap_event_cb_register(sample_ble_gap_event_cb, XF_BLE_EVT_ALL);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "REGISTER common event cb failed:%#X", ret);

    // 注册 gattc 事件回调
    ret = xf_ble_gattc_event_cb_register(sample_ble_gattc_event_cb, XF_BLE_EVT_ALL);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "REGISTER event cb failed:%#X", ret);

    // 注册 gattc app_profile
    ret = xf_ble_gattc_app_register(&s_app_uuid, &s_app_id);
    XF_CHECK(ret != XF_OK || s_app_id == 0, XF_RETURN_VOID, TAG,
             "REGISTER app profile failed:%#X app_id:%d", ret, s_app_id);

    // 开启扫描
    ret = xf_ble_gap_start_scan(&scan_param);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "STAR SCAN failed:%#X", ret);
    XF_LOGI(TAG, "STAR SCAN CMPL");

    xf_sys_watchdog_disable();

    // 创建发现任务
    xf_ttask_create_loop(discovery_task, NULL, TASK_PRIORITY, TASK_DELAY_MS);
    // 创建读写任务
    xf_ttask_create_loop(read_write_task, NULL, TASK_PRIORITY, TASK_DELAY_MS);
}

/* ==================== [Static Functions] ================================== */

static xf_ble_evt_res_t sample_ble_gap_event_cb(
    xf_ble_gap_evt_t event,
    xf_ble_gap_evt_cb_param_t *param)
{
    UNUSED(param);
    xf_err_t ret = XF_OK;
    switch (event) {
    case XF_BLE_GAP_EVT_SCAN_RESULT: {
        ret = gap_event_scan_result_cb(param);
    } break;
    case XF_BLE_GAP_EVT_CONNECT: {
        s_conn_id = param->connect.conn_id;
        peer_addr = *param->connect.addr;

        XF_LOGI(TAG, "EV:connect:s_conn_id:%d,addr_type:%u,addr:"
                XF_BLE_ADDR_PRINT_FMT,
                s_conn_id, peer_addr.type,
                XF_BLE_ADDR_EXPAND_TO_ARG(peer_addr.addr));

        is_need_discovery = true;
        
    } break;
    case XF_BLE_GAP_EVT_DISCONNECT: {
        XF_LOGI(TAG, "EV:peer disconnect:s_app_id:%d,conn_id:%d,reason:%#X,"
                "addr_type:%d,addr:"XF_BLE_ADDR_PRINT_FMT,
                s_app_id, param->disconnect.conn_id,
                param->disconnect.reason,
                param->disconnect.addr->type,
                XF_BLE_ADDR_EXPAND_TO_ARG(param->disconnect.addr->addr));
    } break;
    case XF_BLE_GAP_EVT_CONN_PARAM_UPDATE: {
        XF_LOGW(TAG, "EV:upd conn param:conn_id:%d,:intv:%d,latency:%d,tiemout:%d", 
            param->conn_param_upd.conn_id,
            param->conn_param_upd.interval,
            param->conn_param_upd.latency,
            param->conn_param_upd.timeout);
            is_need_discovery = true;
    };
    default:
        XF_LOGW(TAG, "EV:GAP OTHERS:%d", event);
        return XF_BLE_EVT_RES_NOT_HANDLED;
    }

    if(ret != XF_OK)
    {
        return XF_BLE_EVT_RES_ERR;
    }
    return XF_BLE_EVT_RES_HANDLED;
}

static xf_ble_evt_res_t sample_ble_gattc_event_cb(
    xf_ble_gattc_evt_t event,
    xf_ble_gattc_evt_cb_param_t *param)
{
    UNUSED(param);
    switch (event) {
    case XF_BLE_GATTC_EVT_READ_CFM: {
        if (param->read_cfm.value_len != 0) {
            XF_LOGI(TAG, "EV:read CMPL:handle:%d", param->read_cfm.handle);
            XF_LOG_BUFFER_HEXDUMP(param->read_cfm.value,
                                  param->read_cfm.value_len);
            is_read_cmpl = true;
        }
    } break;
    case XF_BLE_GATTC_EVT_WRITE_CFM: {
        XF_LOGI(TAG, "EV:write CMPL:handle:%d,it will disconnect",
                param->write_cfm.handle);
    } break;
    case XF_BLE_GATTC_EVT_NOTIFICATION: {
        if (param->ntf.value_len != 0) {
            XF_LOGI(TAG, "EV:read ntf:chara_handle:%d", param->ntf.handle);
            XF_LOG_BUFFER_HEXDUMP(param->ntf.value,
                                  param->ntf.value_len);
        }
    } break;
    default:
         XF_LOGW(TAG, "EV:GATT OTHERS:%d", event);
        return XF_BLE_EVT_RES_NOT_HANDLED;
    }

    return XF_BLE_EVT_RES_HANDLED;
}

static xf_ble_evt_res_t gap_event_scan_result_cb(xf_ble_gap_evt_cb_param_t *param)
{
    xf_err_t ret = XF_OK;
    XF_LOGD(TAG, "EV:scan_result:ADV:addr_type:%d,addr:" XF_BLE_ADDR_PRINT_FMT ",RSSI:%d",
            param->scan_result.addr->type,
            XF_BLE_ADDR_EXPAND_TO_ARG(param->scan_result.addr->addr),
            param->scan_result.rssi
    );
    uint8_t *adv_data_all = param->scan_result.adv_data;
    uint16_t adv_size_all = param->scan_result.adv_data_len;

    if((adv_data_all == NULL) || (adv_size_all == 0))
    {
        return XF_BLE_EVT_RES_HANDLED;
    }
    uint8_t *ptr_current = adv_data_all;
    uint8_t *ptr_end = adv_data_all + adv_size_all;

    // XF_LOGI(TAG, ">>>>>:start:%p,end:%p,size:%d", ptr_current, ptr_end, adv_size_all);
    /* 解析广播数据，当包含指定设备名的广播时，进行连接 */
    while (ptr_current <= ptr_end) 
    {
        uint8_t ad_data_len = ptr_current[0]- XF_BLE_GAP_ADV_STRUCT_AD_TYPE_FIELD_SIZE;
        xf_ble_gap_adv_struct_type_t ad_type = ptr_current[1];

        switch (ad_type) {
        case XF_BLE_ADV_STRUCT_TYPE_LOCAL_NAME_ALL: {
            uint8_t *local_name = &ptr_current[2];
            if (strncmp((char *)s_target_dev_name, (char *)local_name, ad_data_len) == 0) {
                XF_LOGI(TAG, "Found the target dev:%s,addr:" XF_BLE_ADDR_PRINT_FMT ",RSSI:%d",
                    local_name, XF_BLE_ADDR_EXPAND_TO_ARG(param->scan_result.addr->addr), param->scan_result.rssi);
                ret = xf_ble_gap_stop_scan();
                XF_CHECK(ret != XF_OK, XF_BLE_EVT_RES_ERR, TAG, "gap stop scan failed:%#X", ret);
                XF_LOGI(TAG, "STOP SCAN");
                ret = xf_ble_gap_connect(param->scan_result.addr);
                XF_CHECK(ret != XF_OK, XF_BLE_EVT_RES_ERR, TAG, "gap connect failed:%#X", ret);
            }
        } break;
        default: {
            break;
        }
        }
        ptr_current += (ad_data_len + XF_BLE_GAP_ADV_STRUCT_AD_TYPE_FIELD_SIZE + XF_BLE_GAP_ADV_STRUCT_LEN_FIELD_SIZE);
    }
    return XF_BLE_EVT_RES_HANDLED;
}

static void discovery_task(xf_task_t task)
{
    /* 检测是否需要执行发现操作（服务、特征、特征描述符（暂时未支持）） -> 尝试进行连接 */
    if (is_need_discovery == true) {
        is_need_discovery = false;

        xf_err_t ret = XF_OK;

        /* 尝试发现服务 */
        ret = xf_ble_gattc_discover_service(s_app_id, s_conn_id,
                                            DEFAULT_SVC_DISC_HDL_MIN,
                                            DEFAULT_SVC_DISC_HDL_MAX,
                                            NULL,
                                            &service_found_set);
        XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG, "discover service failed:%#X app_id:%d",
                 ret, s_app_id);
        XF_LOGI(TAG, "discover chara:service_found_set cnt:%u,set:%p", 
            service_found_set.cnt, service_found_set.set);
        uint16_t cnt_service = service_found_set.cnt;
        while (cnt_service-- > 0) {
            /* 尝试发现服务下的特征 */
            xf_ble_gattc_service_found_t *service = &service_found_set.set[cnt_service];
            
            ret = xf_ble_gattc_discover_chara(s_app_id, s_conn_id,
                                              service->start_hdl, service->end_hdl, NULL, &service->chara_set_info);
            XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
                     "discover chara failed:%#X app_id:%d", ret, s_app_id);

            if(ret != XF_OK)
            {
                XF_LOGE(TAG, "discover chara failed:%#X app_id:%d", ret, s_app_id);
            }
        }
        if(service_found_set.cnt != 0)
        {
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
            &service_found_set.set[DEFAULT_SERVICE_INDEX];

        XF_LOGI(TAG, "REQ READ:"
                "s_app_id:%d,s_conn_id:%d,"
                "service_uuid(16):%#X,chara_uuid:%#X,hdl:%d",
                s_app_id, s_conn_id,
                service->uuid.uuid16,
                service->chara_set_info.set[DEFAULT_CHARA_READ_INDEX].uuid.uuid16,
                service->chara_set_info.set[DEFAULT_CHARA_READ_INDEX].value_handle
        );
        // 向对端服务端发送 读请求
        ret = xf_ble_gattc_request_read_by_handle(s_app_id, s_conn_id,
              service->chara_set_info.set[DEFAULT_CHARA_READ_INDEX].value_handle);
        if (ret != XF_OK) {
            XF_LOGE(TAG, "REQ READ:failed:%#X", ret);
        }
    }

    if (is_read_cmpl == true) 
    {
        xf_ble_gattc_service_found_t *service =
            &service_found_set.set[DEFAULT_SERVICE_INDEX];
        XF_LOGI(TAG, "REQ WRITE:"
                "s_app_id:%d,s_conn_id:%d,"
                "service_uuid(16):%#02X,chara_uuid:%#02X,hdl:%d",
                s_app_id, s_conn_id,
                service->uuid.uuid16,
                service->chara_set_info.set[DEFAULT_CHARA_WRITE_INDEX].uuid.uuid16,
                service->chara_set_info.set[DEFAULT_CHARA_WRITE_INDEX].value_handle
        );
       // 向对端服务端发送 写请求
        xf_err_t ret = xf_ble_gattc_request_write(s_app_id, s_conn_id,
                       service->chara_set_info.set[DEFAULT_CHARA_WRITE_INDEX].value_handle,
                       s_write_req_data, sizeof(s_write_req_data),
                       XF_BLE_GATT_WRITE_TYPE_WITH_RSP);
        if (ret != XF_OK) {
            XF_LOGE(TAG, "REQ WRITE:failed:%#X", ret);
        }
        
        is_read_cmpl = false;
    }
}
