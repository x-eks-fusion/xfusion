/**
 * @file xf_main.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2025-01-15
 *
 * @copyright Copyright (c) 2025
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_osal.h"
#include "xf_vfs.h"
#include "xf_vfs_littlefs.h"
#include "test_xf_vfs_littlefs.h"

/* ==================== [Defines] =========================================== */

#define TAG "xf_main"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    test_base();
    test_benchmark();
}

/* ==================== [Static Functions] ================================== */
