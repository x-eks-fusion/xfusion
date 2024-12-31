/**
 * @file xf_heap.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-09
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_heap/src/xf_heap.h"

#if defined(CONFIG_XF_HEAP_PORT_ENABLE)

#include "xf_init.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static char s_heap_arr[XF_HEAP_STATIC_SIZE] = {0};
static xf_heap_region_t s_heap_regions[] = {
    {(uint8_t *)s_heap_arr, XF_HEAP_STATIC_SIZE},
    {NULL, 0}
};

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static int port_xf_heap_init(void)
{
    xf_heap_init(s_heap_regions);
    return 0;
}

XF_INIT_EXPORT_BOARD(port_xf_heap_init);

#endif
