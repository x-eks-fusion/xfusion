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

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

/* ==================== [Defines] =========================================== */

#define TAG "example_log"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void test_log_hello(void);
static void test_log_array(void *array, size_t size);
static void test_log_level(void);

/* ==================== [Static Variables] ================================== */

static uint8_t array[] = {11, 22, 33, 44, 55, 66};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    test_log_hello();
    test_log_array(array, sizeof(array));
    test_log_level();
}

/* ==================== [Static Functions] ================================== */

static void test_log_hello(void)
{
    XF_LOGU(TAG, "hello: 测试格式化输出: int: %d, str: %s, float: %f", 123, "abc", -123.456f);
    XF_LOGE(TAG, "hello: 测试格式化输出: int: %d, str: %s, float: %f", 123, "abc", -123.456f);
    XF_LOGW(TAG, "hello: 测试格式化输出: int: %d, str: %s, float: %f", 123, "abc", -123.456f);
    XF_LOGI(TAG, "hello: 测试格式化输出: int: %d, str: %s, float: %f", 123, "abc", -123.456f);
    XF_LOGD(TAG, "hello: 测试格式化输出: int: %d, str: %s, float: %f", 123, "abc", -123.456f);
    XF_LOGV(TAG, "hello: 测试格式化输出: int: %d, str: %s, float: %f", 123, "abc", -123.456f);
}

static void test_log_array(void *array, size_t size)
{
    XF_LOG_BUFFER_HEX(array, size);
    XF_LOG_BUFFER_HEXDUMP(array, size);
    XF_LOG_BUFFER_HEXDUMP_ESCAPE(array, size);
}

static void test_log_level(void)
{
    xf_log_printf("测试输出等级" XF_LOG_NEWLINE);
    int lvl_backup = xf_log_get_global_level();
    for (int lvl = XF_LOG_VERBOSE; lvl > XF_LOG_NONE; --lvl) {
        xf_log_set_global_level(lvl);
        xf_log_printf("--- 输出开始 --- lvl: %u ---" XF_LOG_NEWLINE, lvl);
        XF_LOGU(TAG, "hello");
        XF_LOGE(TAG, "hello");
        XF_LOGW(TAG, "hello");
        XF_LOGI(TAG, "hello");
        XF_LOGD(TAG, "hello");
        XF_LOGV(TAG, "hello");
        xf_log_printf("--- 输出结束 ---------------" XF_LOG_NEWLINE XF_LOG_NEWLINE);
    }
    xf_log_set_global_level(lvl_backup);
}
