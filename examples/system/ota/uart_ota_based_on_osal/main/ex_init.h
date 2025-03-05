/**
 * @file ex_init.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief 
 * @version 1.0
 * @date 2025-01-02
 * 
 * Copyright (c) 2025, CorAL. All rights reserved.
 * 
 */

#ifndef __EX_INIT_H__
#define __EX_INIT_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_osal.h"
#include "xf_hal.h"
#include "xf_sys.h"
#include "xf_ymodem.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define MD5_MAX_LEN     16
#define UART_NUM        CONFIG_XF_YMODEM_EXAMPLE_UART_NUM
#define UART_BAUDRATE   CONFIG_XF_YMODEM_EXAMPLE_UART_BAUDRATE
#define UART_TX_PIN     CONFIG_XF_YMODEM_EXAMPLE_UART_TX_PIN
#define UART_RX_PIN     CONFIG_XF_YMODEM_EXAMPLE_UART_RX_PIN

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

void ex_init_xf_ymodem(void);
xf_ymodem_t *ex_get_xf_ymodem(void);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __EX_INIT_H__ */
