/**
 * @file port_log.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-09
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include <stdio.h>

#include "esp_log.h"

#include "xf_init.h"

#include "xf_log.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static void xf_log_out(const char *str, size_t len, void *arg)
{
    if ((NULL == str) || (0 == len)) {
        return;
    }
    printf("%.*s", (int)len, str);
}

int port_log_init(void)
{
    xf_log_register_obj(xf_log_out, NULL);
    return 0;
}
XF_INIT_EXPORT_SETUP(port_log_init);
