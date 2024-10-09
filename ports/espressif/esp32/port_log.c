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
#include "xf_log_port.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static size_t log_backend(
    char *p_buf, size_t buf_size, xf_log_backend_args_t *p_args);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void port_log_init(void)
{
    xf_log_set_backend(log_backend);
    xf_printf_set_putchar(putchar);
}

/* ==================== [Static Functions] ================================== */

static size_t log_backend(char *p_buf, size_t buf_size, xf_log_backend_args_t *p_args)
{
    (void)(p_args);
    if ((NULL == p_buf) || (0 == buf_size)) {
        return 0;
    }
    return printf("%.*s", (int)buf_size, p_buf);
}
