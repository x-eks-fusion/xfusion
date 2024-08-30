/**
 * @file port_tim.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-14
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_port.h"

#if XF_HAL_TIM_IS_ENABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "port_common.h"

#include "driver/timer.h"
#include "esp_log.h"
#include "soc/soc.h"

#include "xf_utils.h"
#include "xf_task.h"
#include "xf_init.h"
#include "xf_heap.h"

/* ==================== [Defines] =========================================== */

#define TAG "port_tim"

#define XF_HAL_TIM_DEFAULT_ACTIVE               XF_HAL_TIM_ACTIVE_STOP
#define XF_HAL_TIM_DEFAULT_AUTO_RELOAD          true
#define XF_HAL_TIM_DEFAULT_COUNT_DIR            XF_HAL_TIM_COUNT_DIR_UP
#define XF_HAL_TIM_DEFAULT_FREQ_HZ              1000*1000
#define XF_HAL_TIM_DEFAULT_TARGET_TICKS         1000
#define XF_HAL_TIM_DEFAULT_CB_CALLBACK          NULL
#define XF_HAL_TIM_DEFAULT_CB_USER_DATA         NULL
#define XF_HAL_TIM_DEFAULT_ISR_CALLBACK         NULL
#define XF_HAL_TIM_DEFAULT_ISR_USER_DATA        NULL

/* ==================== [Typedefs] ========================================== */

typedef struct _port_arg_t {
    uint32_t tim_num;
    uint32_t ticks;
} port_arg_t;

typedef struct _port_tim_t {
    port_arg_t arg;
    int group;
    int timer;
    bool is_enable;
    xf_hal_tim_callback_t *isr;
    xf_hal_tim_callback_t *cb;
} port_tim_t;

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static xf_err_t port_tim_open(xf_hal_dev_t *dev);
static xf_err_t port_tim_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_tim_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_tim_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static xf_err_t port_tim_close(xf_hal_dev_t *dev);

static bool timer_on_alarm_cb(void *user_data);
static void xf_isr_task_sub(const void *const data, void *user_data);
/* ==================== [Static Variables] ================================== */

static const char *tag = "port_tim";

/* ==================== [Macros] ============================================ */

#define XF_HAL_TIM_UNUSED(x)   (void)(x)

#define XF_HAL_TIM_ERR(x) \
    if (x != 0) {\
        ESP_LOGD(tag, "error code:%d", x);\
        return x;\
    }

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static xf_err_t port_tim_open(xf_hal_dev_t *dev)
{
    port_tim_t *tim = (port_tim_t *)xf_malloc(sizeof(port_tim_t));
    if (tim == NULL) {
        return XF_ERR_NO_MEM;
    }
    memset(tim, 0, sizeof(port_tim_t));

    if (dev->id >= SOC_TIMER_GROUP_TOTAL_TIMERS) {
        return XF_ERR_INVALID_ARG;
    }
    tim->arg.tim_num = dev->id;
    tim->group = dev->id / TIMER_GROUP_MAX;
    tim->timer = dev->id % TIMER_GROUP_MAX;
    tim->is_enable = false;

    dev->platform_data = tim;

    return XF_OK;
}

static xf_err_t port_tim_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_tim_config_t *tim_config = (xf_hal_tim_config_t *)config;
    port_tim_t *tim = (port_tim_t *)dev->platform_data;

    if (cmd == XF_HAL_TIM_CMD_DEFAULT) {
        tim_config->active          = XF_HAL_TIM_DEFAULT_ACTIVE;
        tim_config->auto_reload     = XF_HAL_TIM_DEFAULT_AUTO_RELOAD;
        tim_config->count_dir       = XF_HAL_TIM_DEFAULT_COUNT_DIR;
        tim_config->tick_freq_hz    = XF_HAL_TIM_DEFAULT_FREQ_HZ;
        tim_config->target_ticks    = XF_HAL_TIM_DEFAULT_TARGET_TICKS;
        tim_config->cb.callback     = XF_HAL_TIM_DEFAULT_CB_CALLBACK;
        tim_config->cb.user_data    = XF_HAL_TIM_DEFAULT_CB_USER_DATA;
        tim_config->isr.callback    = XF_HAL_TIM_DEFAULT_ISR_CALLBACK;
        tim_config->isr.user_data   = XF_HAL_TIM_DEFAULT_ISR_USER_DATA;
        tim->isr                    = &tim_config->isr;
        tim->cb                     = &tim_config->cb;

        return XF_OK;
    }

    if (cmd == XF_HAL_TIM_CMD_ALL) {
        return XF_OK;
    }

    if (cmd & XF_HAL_TIM_CMD_COUNT_DIR || cmd & XF_HAL_TIM_CMD_TICK_FREQ_HZ
            || cmd & XF_HAL_TIM_CMD_AUTO_RELOAD) {
        tim_config->count_dir = (tim_config->count_dir == XF_HAL_TIM_COUNT_DIR_UP ?
                                 GPTIMER_COUNT_UP : GPTIMER_COUNT_DOWN);
        timer_config_t config = {
            .clk_src = TIMER_SRC_CLK_DEFAULT,
            .divider = APB_CLK_FREQ / tim_config->tick_freq_hz,
            .counter_dir = tim_config->count_dir,
            .counter_en = TIMER_PAUSE,
            .alarm_en = TIMER_ALARM_EN,
            .auto_reload = tim_config->auto_reload,
        };
        XF_HAL_TIM_ERR(timer_init(tim->group, tim->timer, &config));
    }

    if (cmd & XF_HAL_TIM_CMD_CB || cmd & XF_HAL_TIM_CMD_ISR) {
        XF_HAL_TIM_ERR(timer_isr_callback_add(tim->group, tim->timer, timer_on_alarm_cb,
                                              tim, 0));
        if (!tim->is_enable) {
            XF_HAL_TIM_ERR(timer_enable_intr(tim->group, tim->timer));
        }
    }

    if (cmd & XF_HAL_TIM_CMD_TARGET_TICKS) {
        XF_HAL_TIM_ERR(timer_set_alarm_value(tim->group, tim->timer,
                                             tim_config->target_ticks));
    }

    if (cmd & XF_HAL_TIM_CMD_ACTIVE) {
        if (tim_config->active) {
            xf_task_mbus_sub(PORT_TIM_TOPIC_ID, xf_isr_task_sub, &tim_config->cb);
            XF_HAL_TIM_ERR(timer_set_alarm(tim->group, tim->timer, true));
            XF_HAL_TIM_ERR(timer_start(tim->group, tim->timer));
        } else {
            xf_task_mbus_unsub(PORT_TIM_TOPIC_ID, xf_isr_task_sub);
            XF_HAL_TIM_ERR(timer_set_alarm(tim->group, tim->timer, false));
            XF_HAL_TIM_ERR(timer_pause(tim->group, tim->timer));
        }
    }

    return XF_OK;
}

static int port_tim_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    uint64_t value = 0;
    port_tim_t *tim = (port_tim_t *)dev->platform_data;
    esp_err_t ret = timer_get_counter_value(tim->group, tim->timer, &value);
    XF_CHECK(ret != XF_OK, -ret, TAG, "err:%d", ret);
    *(uint32_t *)buf = value;
    return XF_OK;
}

static int port_tim_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    port_tim_t *tim = (port_tim_t *)dev->platform_data;
    esp_err_t ret = timer_set_counter_value(tim->group, tim->timer, *(uint32_t *)buf);
    XF_CHECK(ret != XF_OK, -ret, TAG, "err:%d", ret);
    return XF_OK;
}

static xf_err_t port_tim_close(xf_hal_dev_t *dev)
{
    port_tim_t *tim = (port_tim_t *)dev->platform_data;
    if (!tim->is_enable) {
        XF_HAL_TIM_ERR(timer_disable_intr(tim->group, tim->timer));
        XF_HAL_TIM_ERR(timer_isr_callback_remove(tim->group, tim->timer));
    }
    XF_HAL_TIM_ERR(timer_deinit(tim->group, tim->timer));
    xf_free(tim);
    return XF_OK;
}

static bool IRAM_ATTR timer_on_alarm_cb(void *user_data)
{
    uint64_t value = 0;
    port_tim_t *tim = (port_tim_t *)user_data;
    value = timer_group_get_counter_value_in_isr(tim->group, tim->timer);
    tim->arg.ticks = value;
    if (tim->isr->callback) {
        tim->isr->callback(tim->arg.tim_num, value, tim->isr->user_data);
    }
    if (tim->cb->callback) {
        xf_task_mbus_pub_async(PORT_TIM_TOPIC_ID, (void *)&tim->arg);
    }
    return true;
}

static void xf_isr_task_sub(const void *const data, void *user_data)
{
    port_arg_t *arg = (port_arg_t *) data;
    xf_hal_tim_callback_t *cb = (xf_hal_tim_callback_t *)user_data;
    cb->callback(arg->tim_num, arg->ticks, cb->user_data);
}

static int xf_hal_tim_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_tim_open,
        .ioctl = port_tim_ioctl,
        .write = port_tim_write,
        .read = port_tim_read,
        .close = port_tim_close,
    };
    xf_hal_tim_register(&ops);
    xf_task_mbus_reg_topic(PORT_TIM_TOPIC_ID, sizeof(port_arg_t));
    return XF_OK;
}

XF_INIT_EXPORT_PREV(xf_hal_tim_reg);

#endif
