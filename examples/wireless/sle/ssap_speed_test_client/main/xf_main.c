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
#include "xf_sys.h"

/* ==================== [Defines] =========================================== */

#define TAG "sample_ssapc"

#define SAMPLE_SLE_SEEK_INTERVAL    0x30
#define SAMPLE_SLE_SEEK_WINDOW      0x30

#define TASK_PRIORITY   5
#define TASK_DELAY_MS   500

#define PROP_INDEX_S2M      0
#define PROP_INDEX_M2S      1

#define DEC_TO_INT_KEEP_PLACES_NUM      2
#define DEC_TO_INT_KEEP_PLACES_BASE     100
#define DEC_TO_INT_PRINT_FMT            "%u.%02u"

#define SLE_SPEED_HUNDRED   100        /* 100  */

#define SLE_TEST_CFG_WS63_MAX_SPEED         0
#define SLE_TEST_CFG_WS63_MAX_DISTANCE      1
#define SLE_TEST_CFG_WS63_MAX_DISTANCE_MAX_SENSITIVITY      2
#define SLE_TEST_CFG_BS2X_MAX_SPEED         3
#define SLE_TEST_CFG_BS2X_MAX_DISTANCE      4

#define SLE_TEST_CFG_TYPE   SLE_TEST_CFG_WS63_MAX_DISTANCE_MAX_SENSITIVITY

#if (SLE_TEST_CFG_TYPE == SLE_TEST_CFG_WS63_MAX_SPEED)

#define DEFAULT_SLE_TX_PWR_LEVEL        20
#define DEFAULT_SLE_CUSTOMIZE_PWR       20

#define DEFAULT_SLE_TEST_SUB_PKT_SIZE       1200    //(236)
#define DEFAULT_SLE_TEST_SUB_PKT_NUMS       (1000)
#define DEFAULT_SLE_TEST_SUB_REPEAT_TIME    (1)
#define DEFAULT_SLE_TEST_RECV_PKT_CNT       1000

#elif (SLE_TEST_CFG_TYPE == SLE_TEST_CFG_WS63_MAX_DISTANCE)

#define DEFAULT_SLE_TX_PWR_LEVEL        20
#define DEFAULT_SLE_CUSTOMIZE_PWR       20

#define DEFAULT_SLE_TEST_SUB_PKT_SIZE       236    //(236)
#define DEFAULT_SLE_TEST_SUB_PKT_NUMS       (10)
#define DEFAULT_SLE_TEST_SUB_REPEAT_TIME    (1)
#define DEFAULT_SLE_TEST_RECV_PKT_CNT       10

#elif (SLE_TEST_CFG_TYPE == SLE_TEST_CFG_WS63_MAX_DISTANCE_MAX_SENSITIVITY)

#define DEFAULT_SLE_TX_PWR_LEVEL        20
#define DEFAULT_SLE_CUSTOMIZE_PWR       20

#define DEFAULT_SLE_TEST_SUB_PKT_SIZE       2    //(236)
#define DEFAULT_SLE_TEST_SUB_PKT_NUMS       (1)
#define DEFAULT_SLE_TEST_SUB_REPEAT_TIME    (1)
#define DEFAULT_SLE_TEST_RECV_PKT_CNT       1


#elif (SLE_TEST_CFG_TYPE == SLE_TEST_CFG_BS2X_MAX_SPEED)

#define DEFAULT_SLE_TX_PWR_LEVEL        8
#define DEFAULT_SLE_CUSTOMIZE_PWR       8

#define DEFAULT_SLE_TEST_SUB_PKT_SIZE       1200    //(236)
#define DEFAULT_SLE_TEST_SUB_PKT_NUMS       (1000)
#define DEFAULT_SLE_TEST_SUB_REPEAT_TIME    (1)
#define DEFAULT_SLE_TEST_RECV_PKT_CNT       100

#elif (SLE_TEST_CFG_TYPE == SLE_TEST_CFG_BS2X_MAX_DISTANCE)

#define DEFAULT_SLE_TX_PWR_LEVEL        8
#define DEFAULT_SLE_CUSTOMIZE_PWR       8

#define DEFAULT_SLE_TEST_SUB_PKT_SIZE       1200    //(236)
#define DEFAULT_SLE_TEST_SUB_PKT_NUMS       (1000)
#define DEFAULT_SLE_TEST_SUB_REPEAT_TIME    (1)
#define DEFAULT_SLE_TEST_RECV_PKT_CNT       100

#endif

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
static uint8_t target_device_addr[XF_SLE_ADDR_LEN] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

static xf_sle_uuid_info_t s_app_uuid = {0};
static uint8_t s_app_id = 0;
static uint8_t s_conn_id = 0;

static uint64_t s_test_start_us;
static uint64_t s_test_end_us;

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

typedef enum _sle_test_sync_type_t
{
    SLE_TEST_SYNC_TYPE_NONE,
    SLE_TEST_SYNC_TYPE_ALL_CFG_RECORD,
    SLE_TEST_SYNC_TYPE_SUB_INFO,
    SLE_TEST_SYNC_TYPE_TEST_CFG_START,
    SLE_TEST_SYNC_TYPE_TEST_CFG_END
} sle_test_sync_type_t;

static uint16_t s_prop_hdl[2] = {0};

static bool is_need_discovery = false;
static bool is_discovery_cmpl = false;
static bool is_write_cmpl = false;

static bool is_sync_cmpl = false;
static bool is_upd_cmpl = false;
static bool is_test_cmpl = false;
static bool is_read_cfm = false;

static uint64_t s_temp_time_us_start = 0;
static uint64_t s_temp_time_us_end = 0;
static uint64_t s_test_time_diff_us = 0;
static uint32_t s_recv_pkt_nums = 0;
static volatile uint32_t s_recv_pkt_times = 0;

static uint32_t s_count_recv_byte = 0;

static int g_rssi_sum = 0;
static int g_rssi_number = 0;


typedef struct _sle_test_sub_info_t
{
    union 
    {
        uint16_t _arg_1;
        uint16_t delay_ms;  // master
        uint16_t test_cnt;  // slave
    };
    
    uint16_t pkt_size;
    uint16_t pkt_nums;
    uint16_t repeat;
} sle_test_sub_info_t;

static sle_test_sub_info_t s_sle_test_sub_info =
{
    ._arg_1 = 0, // master: delay_ms 0; slave: forever 
    .pkt_size = DEFAULT_SLE_TEST_SUB_PKT_SIZE,
    .pkt_nums = DEFAULT_SLE_TEST_SUB_PKT_NUMS,
    .repeat = DEFAULT_SLE_TEST_SUB_REPEAT_TIME,
};


/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    XF_LOGI(TAG, "XF SLE SPEED TEST CLIENT:%d", SLE_TEST_CFG_TYPE);

    xf_sys_watchdog_disable();

    // [speed]: 给定预期功率值设置最大功率档位
    xf_sle_set_max_pwr_level_by_pwr(DEFAULT_SLE_TX_PWR_LEVEL);
    
    xf_sle_set_max_pwr(DEFAULT_SLE_CUSTOMIZE_PWR, DEFAULT_SLE_CUSTOMIZE_PWR);

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
                service_struct.start_hdl, service_struct.end_hdl);
        
        s_prop_hdl[PROP_INDEX_S2M] = service_struct.start_hdl;
        s_prop_hdl[PROP_INDEX_M2S] = service_struct.start_hdl+2;

        is_discovery_cmpl = true;
    }

    if (is_discovery_cmpl == true) {
        /* 向对端服务端发送 读请求 */
        XF_LOGI(TAG, ">> request read,app_id:%d,conn_id:%d,hdl:%u",
                s_app_id, s_conn_id, s_prop_hdl[PROP_INDEX_S2M]);
        ret = xf_sle_ssapc_request_read_by_handle(s_app_id, s_conn_id,
              XF_SLE_SSAP_DESCRIPTOR_CLIENT_CONFIGURATION, s_prop_hdl[PROP_INDEX_S2M]);
        if (ret != XF_OK) {
            XF_LOGE(TAG, ">> request write cmd error: %#X", ret);
            return;
        }
        is_discovery_cmpl = false;
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

static uint32_t get_float_int(float in)
{
    return (uint32_t)(((uint64_t)(in * SLE_SPEED_HUNDRED)) / SLE_SPEED_HUNDRED);
}

static uint32_t get_float_dec(float in)
{
    return (uint32_t)(((uint64_t)(in * SLE_SPEED_HUNDRED)) % SLE_SPEED_HUNDRED);
}

static uint32_t GetFloatInt(float in)
{
    return (uint32_t)(in);
}

static uint32_t GetFloatDec(float in)
{
    uint32_t part_int = (uint32_t)(in);
    return (uint32_t)((in - part_int) * DEC_TO_INT_KEEP_PLACES_BASE);
}

static xf_err_t sample_ssapc_event_cb(
    xf_sle_ssapc_event_t event,
    xf_sle_ssapc_evt_cb_param_t *param)
{
    switch (event) {
    case XF_SLE_COMMON_EVT_CONN_PARAMS_UPDATE:{
        XF_LOGI(TAG, "EV:UPD CONN PARAM:intv:%d,latency:%d",
            param->conn_param_update.interval, param->conn_param_update.latency);
    }break;
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
    } break;
    case XF_SLE_SSAPC_EVT_READ_CFM: {
    } break;
    case XF_SLE_SSAPC_EVT_NOTIFICATION:
    {
        if (s_recv_pkt_nums == 0) {
            s_test_start_us = xf_sys_time_get_us();
            s_count_recv_byte = 0;
        } 
        ++s_recv_pkt_nums;
        s_count_recv_byte += param->ntf.data_len;
        if (s_recv_pkt_nums == DEFAULT_SLE_TEST_RECV_PKT_CNT) {
            s_test_end_us = xf_sys_time_get_us();
           
            xf_us_t s_test_time_diff_us = s_test_end_us - s_test_start_us;
            float diff_s = (float)(s_test_time_diff_us) / 1000000.0;  /* 1s = 1000000.0us */

            // uint32_t size_byte = DEFAULT_SLE_TEST_SUB_PKT_SIZE*DEFAULT_SLE_TEST_RECV_PKT_CNT;
            uint32_t size_byte = s_count_recv_byte;
            uint32_t size_bit = size_byte * 8;

            float speed_byte_per_s = size_byte / diff_s;
            float speed_bit_per_s = size_bit / diff_s;  /* 1B = 8bits */

            printf(
                    "SPEED TEST ONCE:recv_byte:%d,times:%d,pkt:%d,nums:%d Time:%llu us, [%llu, %llu] us\r\n"
                    "\t" DEC_TO_INT_PRINT_FMT " bps\t" DEC_TO_INT_PRINT_FMT " Kibps\t" DEC_TO_INT_PRINT_FMT " Mibps\r\n"
                    "\t" DEC_TO_INT_PRINT_FMT " Bps\t" DEC_TO_INT_PRINT_FMT " KiBps\t" DEC_TO_INT_PRINT_FMT " MiBps \r\n",
                    // s_sle_test_sub_info.repeat, s_sle_test_sub_info.pkt_size, s_sle_test_sub_info.pkt_nums, s_test_time_diff_us,
                    s_count_recv_byte, 1, DEFAULT_SLE_TEST_SUB_PKT_SIZE, DEFAULT_SLE_TEST_RECV_PKT_CNT, s_test_time_diff_us, s_test_start_us, s_test_end_us,
                    GetFloatInt(speed_bit_per_s), GetFloatDec(speed_bit_per_s),
                    GetFloatInt(speed_bit_per_s / 1024), GetFloatDec(speed_bit_per_s / 1024),
                    GetFloatInt(speed_bit_per_s / (1024 * 1024)), GetFloatDec(speed_bit_per_s / (1024 * 1024)),

                    GetFloatInt(speed_byte_per_s), GetFloatDec(speed_byte_per_s),
                    GetFloatInt(speed_byte_per_s / 1024), GetFloatDec(speed_byte_per_s / 1024),
                    GetFloatInt(speed_byte_per_s / (1024 * 1024)), GetFloatDec(speed_byte_per_s / (1024 * 1024))
                    );

            s_recv_pkt_nums = 0;
            s_test_start_us = s_test_end_us;
        }
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

    if(xf_memcmp(result->peer_addr.addr, target_device_addr, XF_SLE_ADDR_LEN) == 0)
    {
        XF_LOGD(TAG, "EV:seek result:evt_type:%d,rssi:%d"
            XF_SLE_ADDR_PRINT_FMT " data status:%d",
            result->evt_type, result->rssi,
            XF_SLE_ADDR_EXPAND_TO_ARG(result->peer_addr.addr),
            result->data_status);
        
        ret = xf_sle_stop_seek();
        XF_CHECK(ret != XF_OK, ret, TAG, "xf_sle_stop_seek failed:%#X", ret);
        ret = xf_sle_connect(&result->peer_addr);
        XF_CHECK(ret != XF_OK, ret, TAG, "xf_sle_connect failed:%#X", ret);
        return XF_OK;
    }

    uint8_t *adv_data_all = result->data;
    uint16_t adv_size_all = result->data_len;

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
