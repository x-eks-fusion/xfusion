/**
 * @file xf_main.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief 
 * @version 1.0
 * @date 2025-01-02
 * 
 * Copyright (c) 2025, CorAL. All rights reserved.
 * 
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_osal.h"
#include "xf_hal.h"
#include "xf_sys.h"
#include "xf_ymodem.h"

#include "ex_init.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void ex_ymodem_uart_setup(void);

static int32_t port_xf_ymodem_write(const void *src, uint32_t size, uint32_t timeout_ms);
static int32_t port_xf_ymodem_read(void *dst, uint32_t size, uint32_t timeout_ms);
static void port_xf_ymodem_flush(void);
static void port_xf_ymodem_delay_ms(uint32_t ms);

/* ==================== [Static Variables] ================================== */

static xf_ymodem_t s_ymodem = {0};
static xf_ymodem_t *sp_ym = &s_ymodem;
/* 不需要预留尾随 '\0' */
static uint8_t s_ym_buf[XF_YMODEM_STX_PACKET_SIZE] = {0};

static const xf_ymodem_ops_t port_xf_ymodem_ops = {
    .read           = port_xf_ymodem_read,
    .write          = port_xf_ymodem_write,
    .flush          = port_xf_ymodem_flush,
    .delay_ms       = port_xf_ymodem_delay_ms,
    .user_parse     = NULL,         /*!< 可以没有用户自定义解析函数 */
    .user_file_info = NULL,         /*!< 可以没有用户自定义文件信息填充函数 */
};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void ex_init_xf_ymodem(void)
{
    xf_err_t xf_ret = XF_OK;

    ex_ymodem_uart_setup();

    sp_ym->p_buf        = s_ym_buf;
    sp_ym->buf_size     = ARRAY_SIZE(s_ym_buf);
    sp_ym->retry_num    = 10;
    sp_ym->timeout_ms   = 50;
    sp_ym->user_data    = NULL;
    sp_ym->ops          = &port_xf_ymodem_ops;

    xf_ret = xf_ymodem_check(sp_ym);
    XF_ERROR_CHECK(xf_ret);
}

xf_ymodem_t *ex_get_xf_ymodem(void)
{
    return sp_ym;
}

/* ==================== [Static Functions] ================================== */

static void ex_ymodem_uart_setup(void)
{
    xf_hal_uart_init(UART_NUM, UART_BAUDRATE);
    xf_hal_uart_set_gpio(UART_NUM, UART_TX_PIN, UART_RX_PIN);
    xf_hal_uart_enable(UART_NUM);
}

static int32_t port_xf_ymodem_write(const void *src, uint32_t size, uint32_t timeout_ms)
{
    UNUSED(timeout_ms);
    int32_t wlen;
    wlen = xf_hal_uart_write(UART_NUM, src, size);
    return wlen;
}

static int32_t port_xf_ymodem_read(void *dst, uint32_t size, uint32_t timeout_ms)
{
    int32_t rlen_real = 0;
    int32_t rlen = 0;
    uint32_t time_ms = 0;
    uint32_t time_start_ms = (uint32_t)xf_sys_time_get_ms();
    uint32_t time_end_ms = time_start_ms + timeout_ms;

    do {
        rlen = xf_hal_uart_read(
                   UART_NUM, (uint8_t *)dst + rlen_real, size - rlen_real);
        if (rlen > 0) {
            rlen_real += rlen;
        }
        if (rlen_real >= size) {
            break;
        }
        /*
            esp32 测试数据
            1.  `xf_osal_delay_ms(1);`
                太慢(921600, 正常约 52 KBps, 此方式只有 40 KBps)。
            2. `xf_sys_watchdog_kick(); xf_osal_thread_yield();`
                没用
            3. 每 20 ms 睡眠一次，速度约 47.6 KBps(20MB 数据量)，看门狗不会叫。
            4. 关闭看门狗。
         */
        time_ms = xf_sys_time_get_ms();
        if ((time_ms - time_start_ms + 1) % 20 == 0) {
            xf_osal_delay_ms(1);
        }
    } while (time_ms < time_end_ms);

    return rlen_real;
}

static void port_xf_ymodem_flush(void)
{
    int32_t rlen = 0;
    uint8_t dummy_buf[32]   = {0};
    while (1) {
        rlen = xf_hal_uart_read(UART_NUM, dummy_buf, ARRAY_SIZE(dummy_buf));
        if (rlen <= 0) {
            break;
        }
    }
}

static void port_xf_ymodem_delay_ms(uint32_t ms)
{
    xf_osal_delay_ms(ms);
}
