/**
 * @file xf_main.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-08
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/**
 * @example examples/system/sys/main/xf_main.c
 * 系统级 API 示例。
 */

/* ==================== [Includes] ========================================== */

#include "xf_sys.h"

/* ==================== [Defines] =========================================== */

#define TAG "main"
#define EXAMPLE_DELAY_LOOP_COUNT        20  /*!< 有些看门狗超时时间很长 */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_ms_t ms_before = xf_sys_time_get_ms();
    xf_delay_ms(1000);
    xf_ms_t ms_after = xf_sys_time_get_ms();

    xf_us_t us_before = xf_sys_time_get_us();
    xf_delay_us(10);
    xf_us_t us_after = xf_sys_time_get_us();

    XF_LOGI(TAG, "tick:ms_before:%llu\tms_after:%llu\tms_diff:%llu",
            ms_before, ms_after, ms_after - ms_before);

    XF_LOGI(TAG, "tick:us_before:%llu\tus_after:%llu\tus_diff:%llu",
            us_before, us_after, us_after - us_before);

    /* 确保开启了 watchdog */
    xf_sys_watchdog_enable();
    xf_sys_watchdog_kick();
    XF_LOGI(TAG, "Will reboot in %d seconds.", EXAMPLE_DELAY_LOOP_COUNT);
    for (int i = EXAMPLE_DELAY_LOOP_COUNT; i > 0; --i) {
        XF_LOGI(TAG, "count: %d", i);
        /*
            测试 kick 看门狗是否有作用，
            如果有作用，应当在 EXAMPLE_DELAY_LOOP_COUNT s 后
            输出 `reboot!` 消息并重启。
         */
        xf_sys_watchdog_kick();
        xf_delay_ms(1000);
    }
    XF_LOGI(TAG, "reboot!");
    xf_sys_reboot();
}

/* ==================== [Static Functions] ================================== */
