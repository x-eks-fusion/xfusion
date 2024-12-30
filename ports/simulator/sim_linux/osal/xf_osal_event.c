/**
 * @file xf_osal_event.c
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

#if XF_OSAL_EVENT_IS_ENABLE

#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "xf_sys.h"

/* ==================== [Defines] =========================================== */

#define TAG "oasl_event"

#define CHECK_INTERVAL_MS   2

/* ==================== [Typedefs] ========================================== */

typedef struct {
    uint32_t flags;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} osal_event_internal_t;

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_osal_event_t xf_osal_event_create(const xf_osal_event_attr_t *attr)
{
    osal_event_internal_t *osal_event = malloc(sizeof(osal_event_internal_t));
    if (osal_event) {
        osal_event->flags = 0;
        pthread_mutex_init(&osal_event->mutex, NULL);
        pthread_cond_init(&osal_event->cond, NULL);
    }
    return (xf_osal_event_t)osal_event;
}

xf_err_t xf_osal_event_set(xf_osal_event_t event, uint32_t flags)
{
    PORT_OSAL_ASSERT(event != NULL, XF_ERR_INVALID_ARG, TAG, "event != NULL");
    osal_event_internal_t *osal_event = (osal_event_internal_t *)event;
    pthread_mutex_lock(&osal_event->mutex);
    osal_event->flags |= flags;
    pthread_cond_broadcast(&osal_event->cond);
    pthread_mutex_unlock(&osal_event->mutex);
    return XF_OK;
}

xf_err_t xf_osal_event_clear(xf_osal_event_t event, uint32_t flags)
{
    PORT_OSAL_ASSERT(event != NULL, XF_ERR_INVALID_ARG, TAG, "event != NULL");
    osal_event_internal_t *osal_event = (osal_event_internal_t *)event;
    pthread_mutex_lock(&osal_event->mutex);
    osal_event->flags &= ~flags;
    pthread_mutex_unlock(&osal_event->mutex);
    return 0;
}

uint32_t xf_osal_event_get(xf_osal_event_t event)
{
    PORT_OSAL_ASSERT(event != NULL, XF_ERR_INVALID_ARG, TAG, "event != NULL");
    osal_event_internal_t *osal_event = (osal_event_internal_t *)event;
    pthread_mutex_lock(&osal_event->mutex);
    uint32_t flags = osal_event->flags;
    pthread_mutex_unlock(&osal_event->mutex);
    return flags;
}

xf_err_t xf_osal_event_wait(xf_osal_event_t event, uint32_t flags, uint32_t options, uint32_t timeout)
{
    PORT_OSAL_ASSERT(event != NULL, XF_ERR_INVALID_ARG, TAG, "event != NULL");
    osal_event_internal_t *osal_event = (osal_event_internal_t *)event;

    int ret;
    xf_ms_t now_ms = xf_sys_time_get_ms();
    xf_ms_t end_ms = now_ms + timeout;
    while (end_ms > now_ms) {
        ret = pthread_mutex_trylock(&osal_event->mutex);
        if (ret == 0) {
            // 成功获取锁
            break;
        } else if (ret == EBUSY) {
            now_ms = xf_sys_time_get_ms();
            usleep(CHECK_INTERVAL_MS * 1000);
        } else {
            // 其他错误
            XF_LOGE(TAG, "pthread_mutex_trylock err:%d", errno);
            return XF_FAIL;
        }
    }
    if (end_ms <= now_ms) {
        return XF_ERR_TIMEOUT;
    }

    /*  A. 等待所有指定事件（ XF_OSAL_WAIT_ALL ）： 指定的所有事件都触发时才返回（无完全触发时继续等待）
        B. 等待任何指定事件（ XF_OSAL_WAIT_ANY ）： 指定的事件任何一个被触发时都返回（无任何触发时继续等待）
    */
    while (
        (((options & 0x01) == XF_OSAL_WAIT_ALL) && (osal_event->flags & flags) != flags)
        || (((options & 0x01) == XF_OSAL_WAIT_ANY) && (osal_event->flags & flags) == 0)
    ) {
        if (timeout == XF_OSAL_WAIT_FOREVER) {
            pthread_cond_wait(&osal_event->cond, &osal_event->mutex);
        } else {
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            ts.tv_sec += end_ms / 1000;
            ts.tv_nsec += (end_ms % 1000) * 1000000;
            if (pthread_cond_timedwait(&osal_event->cond, &osal_event->mutex, &ts) == ETIMEDOUT) {
                pthread_mutex_unlock(&osal_event->mutex);
                return XF_ERR_TIMEOUT; // 超时
            }
        }
    }

    if (!(options & XF_OSAL_NO_CLEAR)) {
        osal_event->flags &= ~flags;
    }
    pthread_mutex_unlock(&osal_event->mutex);
    return 0;
}

xf_err_t xf_osal_event_delete(xf_osal_event_t event)
{
    PORT_OSAL_ASSERT(event != NULL, XF_ERR_INVALID_ARG, TAG, "event != NULL");
    osal_event_internal_t *osal_event = (osal_event_internal_t *)event;
    pthread_mutex_destroy(&osal_event->mutex);
    pthread_cond_destroy(&osal_event->cond);
    free(osal_event);
    return 0;
}

/* ==================== [Static Functions] ================================== */

#endif
