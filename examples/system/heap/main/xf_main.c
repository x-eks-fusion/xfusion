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
 * @example{lineno} examples/system/heap/main/xf_main.c
 * 动态内存示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

/* ==================== [Defines] =========================================== */

#define TAG "example_heap"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    size_t size =  xf_heap_get_free_size();
    XF_LOGI(TAG, "free size:%d", size);

    int *value = (int *)xf_malloc(sizeof(int));
    *value = 11;
    XF_LOGI(TAG, "value:%d", *value);

    size =  xf_heap_get_free_size();
    XF_LOGI(TAG, "free size:%d", size);

    xf_free(value);

    size =  xf_heap_get_free_size();
    XF_LOGI(TAG, "free size:%d", size);
}

/* ==================== [Static Functions] ================================== */
