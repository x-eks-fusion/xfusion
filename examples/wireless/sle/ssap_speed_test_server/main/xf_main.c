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
#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define TAG "sample_ssaps"

#define PROP_INDEX_S2M      0
#define PROP_INDEX_M2S      1

#define TASK_PRIORITY                   1
#define SPEED_TEST_PREINIT_DELAY_MS     1000

#define SAMPLE_ADV_ID       1
#define SAMPLE_ADV_HANDLE   1

#define SLE_TEST_CFG_WS63_MAX_SPEED         0
#define SLE_TEST_CFG_WS63_MAX_DISTANCE      1
#define SLE_TEST_CFG_WS63_MAX_DISTANCE_MAX_SENSITIVITY 2
#define SLE_TEST_CFG_BS2X_MAX_SPEED         3
#define SLE_TEST_CFG_BS2X_MAX_DISTANCE      4

#define SLE_TEST_CFG_WS63E_MAX_SPEED        4

#define SLE_TEST_CFG_TYPE   SLE_TEST_CFG_WS63_MAX_DISTANCE_MAX_SENSITIVITY

/* 连接调度间隔12.5ms，单位125us */
#define SAMPLE_SLE_CONN_INTV_MIN                0x64    //0x14 //0x14   // 0x64
/* 连接调度间隔12.5ms，单位125us */
#define SAMPLE_SLE_CONN_INTV_MAX                0x64    // 0x14 // 0x14   // 0x64
/* 连接调度间隔25ms，单位125us */
#define SAMPLE_SLE_ADV_INTERVAL_MIN             0x20    // 0xC8 // 0x14   // 0xC8
/* 连接调度间隔25ms，单位125us */
#define SAMPLE_SLE_ADV_INTERVAL_MAX             0x20    // 0xC8 // 0x14   // 0xC8
/* 超时时间5000ms，单位10ms */
#define SAMPLE_SLE_CONN_SUPERVISION_TIMEOUT     0x1F4
/* 超时时间4990ms，单位10ms */
#define SAMPLE_SLE_CONN_MAX_LATENCY             200

#define DEFAULT_SLE_ADV_PARAM_SCAN_INTERVAL     400
#define DEFAULT_SLE_ADV_PARAM_SCAN_WINDOW       60

#if (SLE_TEST_CFG_TYPE == SLE_TEST_CFG_WS63_MAX_SPEED)

#define DEFAULT_SLE_CONN_INTV_MIN   0x14    // 0x20    // 0x14
#define DEFAULT_SLE_CONN_INTV_MAX   0x14    // 0x20    // 0x14
#define DEFAULT_SLE_CONN_LATENCY    0
#define DEFAULT_SLE_CONN_TIMEOUT    SAMPLE_SLE_CONN_SUPERVISION_TIMEOUT

#define DEFAULT_SLE_TX_RADIO_FRAME      XF_SLE_RADIO_FRAME_2
#define DEFAULT_SLE_RX_RADIO_FRAME      XF_SLE_RADIO_FRAME_2
#define DEFAULT_SLE_TX_PHY              (XF_SLE_PHY_4M)
#define DEFAULT_SLE_RX_PHY              (XF_SLE_PHY_4M)
#define DEFAULT_SLE_TX_PILOT_DENSITY    XF_SLE_PHY_PILOT_DENSITY_16_TO_1
#define DEFAULT_SLE_RX_PILOT_DENSITY    XF_SLE_PHY_PILOT_DENSITY_16_TO_1
#define DEFAULT_SLE_MCS                 (10)
#define DEFAULT_SLE_MTU                 1500    // 512     //(1500)
#define DEFAULT_SLE_SPEED_DATA_LEN      1500    // 512     //(1500)
#define PKT_DATA_LEN                    1200    // 236     //(1200)

#define DEFAULT_SLE_TX_PWR_LEVEL        20
#define SAMPLE_SLE_ADV_TX_POWER         20
#define DEFAULT_SLE_CUSTOMIZE_PWR       20

#elif (SLE_TEST_CFG_TYPE == SLE_TEST_CFG_WS63E_MAX_SPEED)

#define DEFAULT_SLE_CONN_INTV_MIN   0x14    // 0x20    // 0x14
#define DEFAULT_SLE_CONN_INTV_MAX   0x14    // 0x20    // 0x14
#define DEFAULT_SLE_CONN_LATENCY    0
#define DEFAULT_SLE_CONN_TIMEOUT    SAMPLE_SLE_CONN_SUPERVISION_TIMEOUT

#define DEFAULT_SLE_TX_RADIO_FRAME      XF_SLE_RADIO_FRAME_2
#define DEFAULT_SLE_RX_RADIO_FRAME      XF_SLE_RADIO_FRAME_2
#define DEFAULT_SLE_TX_PHY              (XF_SLE_PHY_4M)
#define DEFAULT_SLE_RX_PHY              (XF_SLE_PHY_4M)
#define DEFAULT_SLE_TX_PILOT_DENSITY    XF_SLE_PHY_PILOT_DENSITY_16_TO_1
#define DEFAULT_SLE_RX_PILOT_DENSITY    XF_SLE_PHY_PILOT_DENSITY_16_TO_1
#define DEFAULT_SLE_MCS                 (10)
#define DEFAULT_SLE_MTU                 1500    // 512     //(1500)
#define DEFAULT_SLE_SPEED_DATA_LEN      1500    // 512     //(1500)
#define PKT_DATA_LEN                    1200    // 236     //(1200)

#define DEFAULT_SLE_TX_PWR_LEVEL        20
#define SAMPLE_SLE_ADV_TX_POWER         20
#define DEFAULT_SLE_CUSTOMIZE_PWR       20

#elif (SLE_TEST_CFG_TYPE == SLE_TEST_CFG_WS63_MAX_DISTANCE)

#define DEFAULT_SLE_CONN_INTV_MIN       0x14    // 0x20    // 0x14
#define DEFAULT_SLE_CONN_INTV_MAX       0x14    // 0x20    // 0x14
#define DEFAULT_SLE_CONN_LATENCY        0
#define DEFAULT_SLE_CONN_TIMEOUT        SAMPLE_SLE_CONN_SUPERVISION_TIMEOUT

#define DEFAULT_SLE_TX_RADIO_FRAME      XF_SLE_RADIO_FRAME_1
#define DEFAULT_SLE_RX_RADIO_FRAME      XF_SLE_RADIO_FRAME_1
#define DEFAULT_SLE_TX_PHY              (XF_SLE_PHY_1M)
#define DEFAULT_SLE_RX_PHY              (XF_SLE_PHY_1M)
#define DEFAULT_SLE_TX_PILOT_DENSITY    XF_SLE_PHY_PILOT_DENSITY_NO  // (XF_SLE_PHY_PILOT_DENSITY_16_TO_1)
#define DEFAULT_SLE_RX_PILOT_DENSITY    XF_SLE_PHY_PILOT_DENSITY_NO  // (XF_SLE_PHY_PILOT_DENSITY_16_TO_1)
#define DEFAULT_SLE_MCS                 (0)
#define DEFAULT_SLE_MTU                 512    // 512     //(1500)
#define DEFAULT_SLE_SPEED_DATA_LEN      512    // 512     //(1500)
#define PKT_DATA_LEN                    236    // 236     //(1200)

#define DEFAULT_SLE_TX_PWR_LEVEL        20
#define SAMPLE_SLE_ADV_TX_POWER         20
#define DEFAULT_SLE_CUSTOMIZE_PWR       20

#elif (SLE_TEST_CFG_TYPE == SLE_TEST_CFG_WS63_MAX_DISTANCE_MAX_SENSITIVITY)

#define DEFAULT_SLE_CONN_INTV_MIN       0x14    // 0x20    // 0x14
#define DEFAULT_SLE_CONN_INTV_MAX       0x14    // 0x20    // 0x14
#define DEFAULT_SLE_CONN_LATENCY        0
#define DEFAULT_SLE_CONN_TIMEOUT        10000

#define DEFAULT_SLE_TX_RADIO_FRAME      XF_SLE_RADIO_FRAME_1
#define DEFAULT_SLE_RX_RADIO_FRAME      XF_SLE_RADIO_FRAME_1
#define DEFAULT_SLE_TX_PHY              (XF_SLE_PHY_1M)
#define DEFAULT_SLE_RX_PHY              (XF_SLE_PHY_1M)
#define DEFAULT_SLE_TX_PILOT_DENSITY    XF_SLE_PHY_PILOT_DENSITY_NO  // (XF_SLE_PHY_PILOT_DENSITY_16_TO_1)
#define DEFAULT_SLE_RX_PILOT_DENSITY    XF_SLE_PHY_PILOT_DENSITY_NO  // (XF_SLE_PHY_PILOT_DENSITY_16_TO_1)
#define DEFAULT_SLE_MCS                 (0)
#define DEFAULT_SLE_MTU                 23    // 512     //(1500)
#define DEFAULT_SLE_SPEED_DATA_LEN      2    // 512     //(1500)
#define PKT_DATA_LEN                    2    // 236     //(1200)

#define DEFAULT_SLE_TX_PWR_LEVEL        20
#define SAMPLE_SLE_ADV_TX_POWER         20
#define DEFAULT_SLE_CUSTOMIZE_PWR       20

#elif (SLE_TEST_CFG_TYPE == SLE_TEST_CFG_BS2X_MAX_SPEED)

#define DEFAULT_SLE_CONN_INTV_MIN       0x0F    // 0x20    // 0x14
#define DEFAULT_SLE_CONN_INTV_MAX       0x0F    // 0x20    // 0x14
#define DEFAULT_SLE_CONN_LATENCY        2000
#define DEFAULT_SLE_CONN_TIMEOUT        SAMPLE_SLE_CONN_SUPERVISION_TIMEOUT

#define DEFAULT_SLE_TX_RADIO_FRAME      XF_SLE_RADIO_FRAME_1 // XF_SLE_RADIO_FRAME_1 //(XF_SLE_RADIO_FRAME_2)
#define DEFAULT_SLE_RX_RADIO_FRAME      XF_SLE_RADIO_FRAME_1 // XF_SLE_RADIO_FRAME_1 //(XF_SLE_RADIO_FRAME_2)
#define DEFAULT_SLE_TX_PHY              (XF_SLE_PHY_4M)
#define DEFAULT_SLE_RX_PHY              (XF_SLE_PHY_4M)
#define DEFAULT_SLE_TX_PILOT_DENSITY    (XF_SLE_PHY_PILOT_DENSITY_16_TO_1)
#define DEFAULT_SLE_RX_PILOT_DENSITY    (XF_SLE_PHY_PILOT_DENSITY_16_TO_1)
#define DEFAULT_SLE_MCS                 (0)
#define DEFAULT_SLE_MTU                 512    // 512     //(1500)
#define DEFAULT_SLE_SPEED_DATA_LEN      512    // 512     //(1500)
#define PKT_DATA_LEN                    236    // 236     //(1200)

#define DEFAULT_SLE_TX_PWR_LEVEL        6
#define SAMPLE_SLE_ADV_TX_POWER         6
#define DEFAULT_SLE_CUSTOMIZE_PWR       6

#elif (SLE_TEST_CFG_TYPE == SLE_TEST_CFG_BS2X_MAX_DISTANCE)

#define DEFAULT_SLE_CONN_INTV_MIN       400 // 0x0F    // 0x20    // 0x14
#define DEFAULT_SLE_CONN_INTV_MAX       400 // 0x0F    // 0x20    // 0x14
#define DEFAULT_SLE_CONN_LATENCY        0
#define DEFAULT_SLE_CONN_TIMEOUT        SAMPLE_SLE_CONN_SUPERVISION_TIMEOUT

#define DEFAULT_SLE_TX_RADIO_FRAME      XF_SLE_RADIO_FRAME_1 // XF_SLE_RADIO_FRAME_1 //(XF_SLE_RADIO_FRAME_2)
#define DEFAULT_SLE_RX_RADIO_FRAME      XF_SLE_RADIO_FRAME_1 // XF_SLE_RADIO_FRAME_1 //(XF_SLE_RADIO_FRAME_2)
#define DEFAULT_SLE_TX_PHY              (XF_SLE_PHY_1M)
#define DEFAULT_SLE_RX_PHY              (XF_SLE_PHY_1M)
#define DEFAULT_SLE_TX_PILOT_DENSITY    (XF_SLE_PHY_PILOT_DENSITY_NO)
#define DEFAULT_SLE_RX_PILOT_DENSITY    (XF_SLE_PHY_PILOT_DENSITY_NO)
#define DEFAULT_SLE_MCS                 (0)
#define DEFAULT_SLE_MTU                 512    // 512     //(1500)
#define DEFAULT_SLE_SPEED_DATA_LEN      512    // 512     //(1500)
#define PKT_DATA_LEN                    236    // 236     //(1200)

#define DEFAULT_SLE_TX_PWR_LEVEL        8
#define SAMPLE_SLE_ADV_TX_POWER         8
#define DEFAULT_SLE_CUSTOMIZE_PWR       8

#endif

static unsigned char data[PKT_DATA_LEN];

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static xf_err_t sample_ssaps_event_cb(
    xf_sle_ssaps_event_t event,
    xf_sle_ssaps_evt_cb_param_t *param);
static void sample_sle_set_adv_data(void);
static void sample_sle_set_adv_param(void);
static void task_speed_test(xf_task_t task);

/* ==================== [Static Variables] ================================== */

static uint8_t s_local_name[] = "XF_SSAPS";
static xf_sle_uuid_info_t s_app_uuid = {0};
static uint8_t s_app_id = 0;
static uint16_t s_conn_id = 0;
static uint8_t s_rw_prop_value[8] = "PROP R";
static uint8_t s_rw_desc_value[] = {0x01, 0x00};

static uint8_t s_ntf_prop_value[8] = "I M PROP";
static uint8_t s_ntf_desc_value[] = {0x01, 0x00};

static uint8_t s_rsp_value[] = "I M SSAPS RSP";
static uint8_t s_ntf0_value[] = "I M SSAPS NTF 0";
static uint8_t s_ntf1_value[] = "I M SSAPS NTF 1";

static uint8_t s_prop_ntf_value[8] = {0xBB};
static uint8_t s_desc_ntf_value[] = {0x01, 0x0};

static uint8_t s_sle_mac_addr[XF_SLE_ADDR_LEN] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
static bool is_connected = false;

static xf_sle_ssaps_service_t service_struct = {
    .service_uuid = XF_SLE_DECLARE_UUID16(0x2222),
    .service_type = XF_SLE_SSAP_SERVICE_TYPE_PRIMARY,
    .prop_set = (xf_sle_ssaps_property_t [])
    {
        {
            .prop_uuid = XF_SLE_DECLARE_UUID16(0x2324),
            .permissions = (XF_SLE_SSAP_PERMISSION_READ | XF_SLE_SSAP_PERMISSION_WRITE),
            .operate_indication = (
                    XF_SLE_SSAP_OPERATE_INDICATION_BIT_READ 
                |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_NOTIFY
                |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_INDICATE),
            .value = s_ntf_prop_value,
            .value_len = sizeof(s_ntf_prop_value),
            .desc_set = (xf_sle_ssaps_desc_t []) {
                {
                    .desc_uuid = XF_SLE_DECLARE_UUID16(0x23DD),
                    .permissions = (XF_SLE_SSAP_PERMISSION_READ | XF_SLE_SSAP_PERMISSION_WRITE),
                    .desc_type = XF_SLE_SSAP_DESC_TYPE_CLIENT_CONFIGURATION,
                    .operate_indication = (
                            XF_SLE_SSAP_OPERATE_INDICATION_BIT_READ 
                        |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE 
                    ),
                    .value = s_ntf_desc_value,
                    .value_len = sizeof(s_ntf_desc_value),
                },
                {0}
            },
        },
        {
            .prop_uuid = XF_SLE_DECLARE_UUID16(0x2323),
            .permissions = (XF_SLE_SSAP_PERMISSION_READ | XF_SLE_SSAP_PERMISSION_WRITE),
            .operate_indication = (
                    XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE 
                |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_WRITE_NO_RSP
                |   XF_SLE_SSAP_OPERATE_INDICATION_BIT_READ
            ),
            .value = s_rw_prop_value,
            .value_len = sizeof(s_rw_prop_value),
        },
        {0}
    }
};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    XF_LOGI(TAG, "XF SLE SPEED TEST SERVER:%d", SLE_TEST_CFG_TYPE);

    xf_sys_watchdog_disable();

    // [speed]: 给定预期功率值设置最大功率档位
    xf_sle_set_max_pwr_level_by_pwr(DEFAULT_SLE_TX_PWR_LEVEL);
    
    xf_sle_set_max_pwr(DEFAULT_SLE_CUSTOMIZE_PWR, DEFAULT_SLE_CUSTOMIZE_PWR);

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
    ret = xf_sle_ssaps_app_register(&s_app_uuid, &s_app_id);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_set_local_name error:%#X", ret);

    // 添加 服务（结构）到 服务端 app
    ret = xf_sle_ssaps_add_service_to_app(s_app_id, &service_struct);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_ssaps_add_service_to_app error:%#X", ret);
    XF_LOGI(TAG, "property handle:[0]:%d,uuid:%#X",
            service_struct.prop_set[0].prop_handle,
            service_struct.prop_set[0].prop_uuid->uuid16);

    // 启动服务
    ret = xf_sle_ssaps_start_service(s_app_id, service_struct.service_handle);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_ssaps_start_service error:%#X", ret);

    /* 设置广播数据及参数 */
    sample_sle_set_adv_param();
    sample_sle_set_adv_data();

    /* [speed]: mtu 协商 (注意需要在连接后) */
    xf_sle_ssap_exchange_info_t info = {
        .mtu_size = DEFAULT_SLE_MTU,
        .version = 1,
    };
    ret = xf_sle_ssaps_set_info(s_app_id, &info);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
            "xf_sle_ssaps_set_info error:%#X", ret);

    // 启动广播
    ret = xf_sle_start_announce(SAMPLE_ADV_ID);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_start_announce error:%#X", ret);
    XF_LOGI(TAG, "ADV STAR CMPL");

    xf_sle_conn_param_def_t _set_conn_param_def =
    {
        .en_filter          = false,
        .en_gt_negotiate    = false,
        .scan_interval      = DEFAULT_SLE_ADV_PARAM_SCAN_INTERVAL,
        .scan_phy           = XF_SLE_SEEK_PHY_1M,
        .scan_window        = DEFAULT_SLE_ADV_PARAM_SCAN_WINDOW,
        .interval_max       = DEFAULT_SLE_CONN_INTV_MAX,
        .interval_min       = DEFAULT_SLE_CONN_INTV_MIN,
        .timeout            = DEFAULT_SLE_CONN_TIMEOUT,
    };

    ret = xf_sle_set_default_conn_params(&_set_conn_param_def);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID,
                TAG, "xf_sle_set_default_conn_params err:%d", ret);
    // [speed]: 关闭看门狗避免测速任务时触发看门狗
    xf_sys_watchdog_disable();

    // [speed]: 创建测速任务
    xf_ttask_create_loop(task_speed_test, NULL, TASK_PRIORITY, 1000);
}

/* ==================== [Static Functions] ================================== */

/**
 * @brief [speed]: 测速预初始化:
 *  1. 设置 data len ( payload )
 *  2. 设置 PHY 参数
 *  3. 设置 调制与编码策略 (mcs)
 *  4. 标记已连接 (告知测速任务就绪)
 * 
 * @param task 
 */
static void task_speed_test_preinit(xf_task_t task)
{
    xf_err_t ret = xf_sle_set_data_len(s_conn_id, DEFAULT_SLE_SPEED_DATA_LEN);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
            "xf_sle_set_data_len error:%#X", ret);

    xf_sle_set_phy_t phy_parm = {
        .tx_format = DEFAULT_SLE_TX_RADIO_FRAME,
        .rx_format = DEFAULT_SLE_RX_RADIO_FRAME,
        .tx_phy = DEFAULT_SLE_TX_PHY,
        .rx_phy = DEFAULT_SLE_RX_PHY,
        .tx_pilot_density = DEFAULT_SLE_TX_PILOT_DENSITY,
        .rx_pilot_density = DEFAULT_SLE_RX_PILOT_DENSITY,
        .g_feedback = 0,
        .t_feedback = 0,
    };
    ret = xf_sle_set_phy_params(s_conn_id, &phy_parm);
    if(ret != XF_OK)
    {
        XF_LOGW(TAG, "xf_sle_set_phy_params error:%#X", ret);
    }
    ret = xf_sle_set_mcs(s_conn_id, DEFAULT_SLE_MCS);
    if(ret != XF_OK)
    {
        XF_LOGW(TAG, "xf_sle_set_mcs error:%#X", ret);
    }

    is_connected = true;
}

/**
 * @brief [speed]:测速任务
 * 
 * @param task 
 */
static void task_speed_test(xf_task_t task)
{
    if(is_connected == false)
    {
        return;
    }

    int i = 0;
    xf_sle_ssaps_ntf_ind_t param_ntf = 
    {
        .handle = service_struct.prop_set[PROP_INDEX_S2M].prop_handle,
        .type = XF_SLE_SSAP_PROPERTY_TYPE_VALUE,
        .value = data,
        .value_len = PKT_DATA_LEN,
    };
    while (1)
    {
        i++;
        data[0] = (i >> 8) & 0xFF;  /* offset 8bits */
        data[1] = i & 0xFF;
        
        xf_err_t ret = xf_sle_ssaps_send_notify_indicate(
                        s_app_id, s_conn_id, &param_ntf);
        XF_CHECK(ret != XF_OK,XF_RETURN_VOID, TAG, "ntf failed!:%X", ret);
        if(is_connected == false)
        {
            return;
        }
        
#if (SLE_TEST_CFG_TYPE == SLE_TEST_CFG_WS63_MAX_DISTANCE_MAX_SENSITIVITY)
        xf_osal_delay_ms(500);
#endif
    }
}

static xf_err_t sample_ssaps_event_cb(
    xf_sle_ssaps_event_t event,
    xf_sle_ssaps_evt_cb_param_t *param)
{
    switch (event) {
    case XF_SLE_COMMON_EVT_CONN_PARAMS_UPDATE:{
        XF_LOGI(TAG, "EV:UPD CONN PARAM:intv:%d,latency:%d",
            param->conn_param_update.interval, param->conn_param_update.latency);

        // [speed]: 在几秒延时后执行一次 测速预初始化 (因为需要确保)
        xf_ttask_create(task_speed_test_preinit, NULL, TASK_PRIORITY, SPEED_TEST_PREINIT_DELAY_MS, 1);
    }break;
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
        s_conn_id = param->connect.conn_id;
        
        xf_err_t ret;
                
        /* [speed]: 更新连接参数 (注意需要在连接后) */
        xf_sle_conn_param_update_t _set_conn_param_upd =
        {
            .conn_id        = s_conn_id,
            .interval_max   = DEFAULT_SLE_CONN_INTV_MAX,
            .interval_min   = DEFAULT_SLE_CONN_INTV_MIN,
            .max_latency    = DEFAULT_SLE_CONN_LATENCY,
            .supervision_timeout = DEFAULT_SLE_CONN_TIMEOUT
        };
        ret = xf_sle_update_conn_params(&_set_conn_param_upd);
        XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "xf_sle_update_conn_params error:%#X", ret);
    } break;
    case XF_SLE_SSAPS_EVT_WRITE_REQ: {
        XF_LOGI(TAG, "EV:REQ WRITE:need_rsp:%d,hdl:%d,conn_id:%d,trans_id:%d",
                param->write_req.need_rsp, param->write_req.handle,
                param->write_req.conn_id, param->write_req.trans_id);
        XF_LOG_BUFFER_HEXDUMP_ESCAPE(param->write_req.value, param->write_req.value_len);
        XF_LOGI(TAG, ">>> [0].HDL:%d", service_struct.prop_set[0].prop_handle);

    } break;
    case XF_SLE_SSAPS_EVT_READ_REQ: {
        XF_LOGI(TAG, "EV:REQ READ:need_rsp:%d,hdl:%d,conn_id:%d,trans_id:%d",
                param->read_req.need_rsp, param->read_req.handle,
                param->read_req.conn_id, param->read_req.trans_id);

        XF_LOGI(TAG, ">>> [1].HDL:%d", service_struct.prop_set[1].prop_handle);
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
        announce_param.announce_channel_map =  XF_SLE_ADV_CHANNEL_MAP_77;    // XF_SLE_ADV_CHANNEL_MAP_DEFAULT;

        announce_param.announce_interval_min = SAMPLE_SLE_ADV_INTERVAL_MIN;
        announce_param.announce_interval_max = SAMPLE_SLE_ADV_INTERVAL_MAX;

        announce_param.conn_interval_min = SAMPLE_SLE_CONN_INTV_MIN;
        announce_param.conn_interval_max = SAMPLE_SLE_CONN_INTV_MAX;
        announce_param.conn_max_latency = SAMPLE_SLE_CONN_MAX_LATENCY;
        announce_param.conn_supervision_timeout = SAMPLE_SLE_CONN_SUPERVISION_TIMEOUT;
        announce_param.announce_tx_power = SAMPLE_SLE_ADV_TX_POWER;
    };
    xf_memcpy(announce_param.own_addr.addr, s_sle_mac_addr, XF_SLE_ADDR_LEN);

    xf_err_t ret = xf_sle_set_announce_param(SAMPLE_ADV_ID, &announce_param);
    XF_CHECK(ret != XF_OK, XF_RETURN_VOID, TAG,
             "ADV set param error:%#X", ret);
}
