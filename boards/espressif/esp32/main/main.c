/**
 * @file main.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-07-09
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xfusion.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void _preinit(void);
static void _predeinit(void);

extern void port_log_init(void);

/* ==================== [Static Variables] ================================== */

static const xf_init_preinit_ops_t preinit_ops = {
    .preinit        = _preinit,
    .predeinit      = _predeinit,
};


/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void app_main(void)
{
    xfusion_run(&preinit_ops);
}

/* ==================== [Static Functions] ================================== */

static void _preinit(void)
{
    port_log_init();
}

static void _predeinit(void)
{

}
