/**
 * @file xf_main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-23
 *
 * Copyright (c) 2023, CorAL. All rights reserved.
 *
 */

/**
 * @example{lineno} examples/storage/fal/fal_flashdb/main/xf_main.c
 * 基于 xf_fal 的 flashdb 示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_osal.h"
#include "xf_fal.h"
#include "flashdb.h"

/* ==================== [Defines] =========================================== */

#define TAG "flashdb_example"

#define KVDB_NAME CONFIG_PORT_FDB_XF_FAL_KVDB_PARTITION_NAME
#define TSDB_NAME CONFIG_PORT_FDB_XF_FAL_TSDB_PARTITION_NAME

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static fdb_time_t get_time(void);
static void lock(fdb_db_t db);
static void unlock(fdb_db_t db);

/* 这些示例位于 `examples/storage/fal/fal_flashdb/components/FlashDB/samples` */
extern void kvdb_basic_sample(fdb_kvdb_t kvdb);
extern void kvdb_type_string_sample(fdb_kvdb_t kvdb);
extern void kvdb_type_blob_sample(fdb_kvdb_t kvdb);
extern void tsdb_sample(fdb_tsdb_t tsdb);

static int flashdb_demo(void);

/* ==================== [Static Variables] ================================== */

/**
 * @brief 使用默认分区的空间注册一个提供给 FlashDB 使用的分区表。
 * @note 注意不要与其他应用注册的分区表交叠。
 */
static const xf_fal_partition_t sc_default_flash_fdb_partition_table[] = {
    {
        CONFIG_PORT_FDB_XF_FAL_KVDB_PARTITION_NAME,
        CONFIG_PORT_FDB_XF_FAL_FLASH_NAME,
        CONFIG_PORT_FDB_XF_FAL_KVDB_PARTITION_OFFSET,
        CONFIG_PORT_FDB_XF_FAL_KVDB_PARTITION_LENGTH,
    },
    {
        CONFIG_PORT_FDB_XF_FAL_TSDB_PARTITION_NAME,
        CONFIG_PORT_FDB_XF_FAL_FLASH_NAME,
        CONFIG_PORT_FDB_XF_FAL_TSDB_PARTITION_OFFSET,
        CONFIG_PORT_FDB_XF_FAL_TSDB_PARTITION_LENGTH,
    },
};

static uint32_t boot_count = 0;
static time_t boot_time[10] = {0, 1, 2, 3};
/* default KV nodes */
static struct fdb_default_kv_node default_kv_table[] = {
    {"username", "armink", 0},                       /* string KV */
    {"password", "123456", 0},                       /* string KV */
    {"boot_count", &boot_count, sizeof(boot_count)}, /* int type KV */
    {"boot_time", &boot_time, sizeof(boot_time)},    /* int array type KV */
};
/* KVDB object */
static struct fdb_kvdb kvdb = {0};
/* TSDB object */
struct fdb_tsdb tsdb = {0};
/* counts for simulated timestamp */
static int counts = 0;

static xf_osal_mutex_t s_mutex = NULL;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_err_t        xf_ret;
    int             flashdb_demo_ret;

    /* 已初始化状态下无法注册 flash 设备和分区表 */
    xf_fal_deinit();

    /*
        如果 sc_default_flash_fdb_partition_table 内的 flash 设备不是已注册的设备，
        需要先调用 xf_fal_register_flash_device() 注册 flash 设备。
        本例中使用的是默认已注册的 flash 设备，无需再次注册。
     */

    /* 注册提供给 flashdb 使用的分区表 */
    xf_ret = xf_fal_register_partition_table(
                 sc_default_flash_fdb_partition_table,
                 ARRAY_SIZE(sc_default_flash_fdb_partition_table));
    XF_CHECK(xf_ret != XF_OK, XF_RETURN_VOID, TAG, "An error occurred.");

    xf_ret = xf_fal_init();
    XF_CHECK(xf_ret != XF_OK, XF_RETURN_VOID, TAG, "An error occurred.");

    xf_fal_show_part_table();

    flashdb_demo_ret = flashdb_demo();
    XF_CHECK(flashdb_demo_ret != XF_OK, XF_RETURN_VOID, TAG, "An error occurred.");

    XF_LOGI(TAG, "flashdb_demo done!");
}

/* ==================== [Static Functions] ================================== */

static fdb_time_t get_time(void)
{
    /* Using the counts instead of timestamp.
     * Please change this function to return RTC time.
     */
    return ++counts;
}

static int flashdb_demo(void)
{
    fdb_err_t       fdb_ret;

#ifdef FDB_USING_KVDB
    { /* KVDB Sample */
        struct fdb_default_kv default_kv;

        default_kv.kvs = default_kv_table;
        default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
        /* set the lock and unlock function if you want */
        fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_LOCK, lock);
        fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_UNLOCK, unlock);
        /* Key-Value database initialization
         *
         *       &kvdb: database object
         *       "env": database name
         * "fdb_kvdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
         *              Please change to YOUR partition name.
         * &default_kv: The default KV nodes. It will auto add to KVDB when first initialize successfully.
         *        NULL: The user data if you need, now is empty.
         */
        fdb_ret = fdb_kvdb_init(&kvdb, "env", KVDB_NAME, &default_kv, NULL);

        if (fdb_ret != FDB_NO_ERR) {
            return -1;
        }

        /* run basic KV samples */
        kvdb_basic_sample(&kvdb);
        /* run string KV samples */
        kvdb_type_string_sample(&kvdb);
        /* run blob KV samples */
        kvdb_type_blob_sample(&kvdb);
    }
#endif /* FDB_USING_KVDB */

#ifdef FDB_USING_TSDB
    { /* TSDB Sample */
        /* set the lock and unlock function if you want */
        fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_LOCK, lock);
        fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_UNLOCK, unlock);
        /* Time series database initialization
         *
         *       &tsdb: database object
         *       "log": database name
         * "fdb_tsdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
         *              Please change to YOUR partition name.
         *    get_time: The get current timestamp function.
         *         128: maximum length of each log
         *        NULL: The user data if you need, now is empty.
         */
        fdb_ret = fdb_tsdb_init(&tsdb, "log", TSDB_NAME, get_time, 128, NULL);
        /* read last saved time for simulated timestamp */
        fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_GET_LAST_TIME, &counts);

        if (fdb_ret != FDB_NO_ERR) {
            return -1;
        }

        /* run TSDB sample */
        tsdb_sample(&tsdb);
    }
#endif /* FDB_USING_TSDB */

    return 0;
}

static void lock(fdb_db_t db)
{
    UNUSED(db);
    if (s_mutex == NULL) {
        xf_osal_mutex_attr_t mutex_attr = {
            .name = "mutex",
        };
        s_mutex = xf_osal_mutex_create(&mutex_attr);
        if (s_mutex == NULL) {
            XF_LOGE(TAG, "xf mutex create error");
            return;
        }
    }
    xf_osal_mutex_acquire(s_mutex, XF_OSAL_WAIT_FOREVER);
    return;
}

static void unlock(fdb_db_t db)
{
    UNUSED(db);
    if (s_mutex != NULL) {
        xf_osal_mutex_release(s_mutex);
    }
    return;
}
