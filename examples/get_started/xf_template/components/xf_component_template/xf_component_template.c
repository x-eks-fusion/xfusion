/**
 * @file xf_component_template.c
 * @author 你的名字 (你的邮箱@domain.com)
 * @brief 文件模板。
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_component_template.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

static const char *const TAG = "components_test";

/* ==================== [Global Variables] ================================== */

extern int g_xf_templ_global_data2_no_in_head;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_component_func(void)
{
    XF_LOGI(TAG, "hello");
    XF_LOGI(TAG, "CONFIG_COMPONENTS_TEST_VAL: %d", CONFIG_COMPONENTS_TEST_VAL);
    g_xf_templ_global_data2_no_in_head = 9;
    return;
}

/* ==================== [Static Functions] ================================== */
