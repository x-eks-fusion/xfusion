/**
 * @file xf_osal_timer.c
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-20
 *
 * @copyright Copyright (c) 2024
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_osal_internal.h"

#if XF_OSAL_TIMER_IS_ENABLE

#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

/* ==================== [Defines] =========================================== */

#define TAG "osal_timer"

/* ==================== [Typedefs] ========================================== */

typedef struct {
    timer_t timer_id;
    const uint8_t *name;
    xf_osal_timer_func_t cb;
    void *cb_arg;
    xf_osal_timer_type_t type;
} osal_timer_internal_t;

/* ==================== [Static Prototypes] ================================= */

static void timer_handler(union sigval sv);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_osal_timer_t xf_osal_timer_create(xf_osal_timer_func_t func, xf_osal_timer_type_t type, void *argument,
                                     xf_osal_timer_attr_t *attr)
{
    PORT_OSAL_ASSERT(func != NULL, NULL, TAG, "func != NULL");
    osal_timer_internal_t *osal_timer = (osal_timer_internal_t *)malloc(sizeof(osal_timer_internal_t));
    if (osal_timer == NULL) {
        return NULL;
    }

    struct sigevent sev = {0};
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_value.sival_ptr = osal_timer;
    sev.sigev_notify_function = timer_handler;

    int ret = timer_create(CLOCK_REALTIME, &sev, &osal_timer->timer_id);
    if (ret != 0) {
        free(osal_timer);
        XF_LOGE(TAG, "timer_create err:%d", errno);
        return NULL;
    }

    osal_timer->cb = func;
    osal_timer->cb_arg = argument;
    osal_timer->type = type;
    osal_timer->name = attr->name;
    return (xf_osal_timer_t)osal_timer;
}

const char *xf_osal_timer_get_name(xf_osal_timer_t timer)
{
    PORT_OSAL_ASSERT(timer != NULL, NULL, TAG, "timer != NULL");
    osal_timer_internal_t *osal_timer = (osal_timer_internal_t *)timer;
    // 由于POSIX定时器没有直接支持名称，这里返回NULL或自定义名称
    return osal_timer->name;
}

xf_err_t xf_osal_timer_start(xf_osal_timer_t timer, uint32_t ticks)
{
    PORT_OSAL_ASSERT(timer != NULL, XF_ERR_INVALID_ARG, TAG, "timer != NULL");
    osal_timer_internal_t *osal_timer = (osal_timer_internal_t *)timer;
    struct itimerspec its;
    its.it_value.tv_sec = ticks / 1000;
    its.it_value.tv_nsec = (ticks % 1000) * 1000000;
    if (osal_timer->type == XF_OSAL_TIMER_PERIODIC) {
        its.it_interval.tv_sec = its.it_value.tv_sec;
        its.it_interval.tv_nsec = its.it_value.tv_nsec;
    } else {
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;
    }
    int ret = timer_settime(osal_timer->timer_id, 0, &its, NULL);
    XF_CHECK(ret != 0, XF_ERR_INVALID_STATE, TAG, "timer_settime err:%d", errno);
    return XF_OK;
}

xf_err_t xf_osal_timer_stop(xf_osal_timer_t timer)
{
    PORT_OSAL_ASSERT(timer != NULL, XF_ERR_INVALID_ARG, TAG, "timer != NULL");
    osal_timer_internal_t *osal_timer = (osal_timer_internal_t *)timer;
    struct itimerspec its = {0};
    int ret = timer_settime(osal_timer->timer_id, 0, &its, NULL);
    XF_CHECK(ret != 0, XF_ERR_INVALID_STATE, TAG, "timer_settime err:%d", errno);
    return XF_OK;
}

uint32_t xf_osal_timer_is_running(xf_osal_timer_t timer)
{
    PORT_OSAL_ASSERT(timer != NULL, XF_ERR_INVALID_ARG, TAG, "timer != NULL");
    osal_timer_internal_t *osal_timer = (osal_timer_internal_t *)timer;

    struct itimerspec its;
    int ret = timer_gettime(osal_timer->timer_id, &its);
    XF_CHECK(ret != 0, 0, TAG, "timer_settime err:%d", errno);

    return (its.it_value.tv_sec != 0 || its.it_value.tv_nsec != 0);
}

xf_err_t xf_osal_timer_delete(xf_osal_timer_t timer)
{
    PORT_OSAL_ASSERT(timer != NULL, XF_ERR_INVALID_ARG, TAG, "timer != NULL");
    osal_timer_internal_t *osal_timer = (osal_timer_internal_t *)timer;
    int ret = timer_delete(osal_timer->timer_id);
    XF_CHECK(ret != 0, XF_ERR_INVALID_STATE, TAG, "timer_delete err:%d", errno);

    free(osal_timer);
    return XF_OK;
}

/* ==================== [Static Functions] ================================== */

static void timer_handler(union sigval sv)
{
    osal_timer_internal_t *osal_timer = (osal_timer_internal_t *)sv.sival_ptr;
    if (osal_timer && osal_timer->cb) {
        osal_timer->cb(osal_timer->cb_arg);
    }
}

#endif
