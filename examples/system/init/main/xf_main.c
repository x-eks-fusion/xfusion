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

#include "xf_init.h"
#include "xf_utils.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

static const char *const TAG = "xf_main";

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    /**
     * @brief 注意！ xf_init() 会在 xf_main() 前运行，此处不需要重复调用 xf_init()！
     *
     * 此示例仅用于演示自动初始化的使用方法。
     *
     */
}

/* ==================== [Static Functions] ================================== */

static int test_xf_init_component_board(void)
{
    XF_LOGI(TAG, "hello, I'm %s", __FUNCTION__);
    return __COUNTER__;
}
XF_INIT_EXPORT_BOARD(test_xf_init_component_board);

static int test_xf_init_component_prev(void)
{
    XF_LOGI(TAG, "hello, I'm %s", __FUNCTION__);
    return __COUNTER__;
}
XF_INIT_EXPORT_PREV(test_xf_init_component_prev);

static int test_xf_init_component_device(void)
{
    XF_LOGI(TAG, "hello, I'm %s", __FUNCTION__);
    return __COUNTER__;
}
XF_INIT_EXPORT_DEVICE(test_xf_init_component_device);

static int test_xf_init_component_component(void)
{
    XF_LOGI(TAG, "hello, I'm %s", __FUNCTION__);
    return __COUNTER__;
}
XF_INIT_EXPORT_COMPONENT(test_xf_init_component_component);

static int test_xf_init_component_env(void)
{
    XF_LOGI(TAG, "hello, I'm %s", __FUNCTION__);
    return __COUNTER__;
}
XF_INIT_EXPORT_ENV(test_xf_init_component_env);

static int test_xf_init_component_app(void)
{
    XF_LOGI(TAG, "hello, I'm %s", __FUNCTION__);
    return __COUNTER__;
}
XF_INIT_EXPORT_APP(test_xf_init_component_app);

