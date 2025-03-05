/**
 * @file xf_osal_mutex.c
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-19
 *
 * @copyright Copyright (c) 2024
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_osal_internal.h"

#if XF_OSAL_MUTEX_IS_ENABLE

#include "xf_sys.h"
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

/* ==================== [Defines] =========================================== */

#define TAG "oasl_mutex"

#define CHECK_INTERVAL_MS   2

/* ==================== [Typedefs] ========================================== */

typedef struct {
    pthread_mutex_t mutex;
    pthread_t owner_pid;
} osal_mutex_internal_t;

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_osal_mutex_t xf_osal_mutex_create(const xf_osal_mutex_attr_t *attr)
{
    osal_mutex_internal_t *osal_mutex = (osal_mutex_internal_t *)malloc(sizeof(osal_mutex_internal_t));
    if (osal_mutex == NULL) {
        return NULL;
    }
    int ret = 0;
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);

    // 设置互斥锁属性
    if (attr->attr_bits & XF_OSAL_MUTEX_RECURSIVE) {
        ret = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);

    }
    if (attr->attr_bits & XF_OSAL_MUTEX_PRIO_INHERIT) {
        ret = pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
        XF_CHECK(ret != 0, NULL, TAG, "pthread_mutexattr_setprotocol err:%d", errno);
    }
    if (attr->attr_bits & XF_OSAL_MUTEX_ROBUST) {
        ret = pthread_mutexattr_setrobust(&mutex_attr, PTHREAD_MUTEX_ROBUST);
        XF_CHECK(ret != 0, NULL, TAG, "pthread_mutexattr_setrobust err:%d", errno);
    }

    // 初始化互斥锁
    ret = pthread_mutex_init(&osal_mutex->mutex, &mutex_attr);
    XF_CHECK(ret != 0, NULL, TAG, "pthread_mutex_init err:%d", errno);

    return osal_mutex;
}

xf_err_t xf_osal_mutex_acquire(xf_osal_mutex_t mutex, uint32_t timeout)
{
    PORT_OSAL_ASSERT(mutex != NULL, XF_ERR_INVALID_ARG, TAG, "mutex != NULL");
    osal_mutex_internal_t *osal_mutex = (osal_mutex_internal_t *)mutex;

    int ret = 0;
    xf_ms_t end_ms = xf_sys_time_get_ms() + timeout;
    do {
        ret = pthread_mutex_trylock(&osal_mutex->mutex);
        if (ret == 0) {
            osal_mutex->owner_pid = pthread_self();
            return XF_OK;
        }
        usleep(CHECK_INTERVAL_MS * 1000);
    } while (end_ms > xf_sys_time_get_ms());
    return XF_ERR_TIMEOUT;
}

xf_err_t xf_osal_mutex_release(xf_osal_mutex_t mutex)
{
    PORT_OSAL_ASSERT(mutex != NULL, XF_ERR_INVALID_ARG, TAG, "mutex != NULL");
    osal_mutex_internal_t *osal_mutex = (osal_mutex_internal_t *)mutex;
    int ret = pthread_mutex_unlock(&osal_mutex->mutex);
    XF_CHECK(ret != 0, XF_ERR_INVALID_STATE, TAG, "pthread_mutex_unlock err:%d", errno);

    osal_mutex->owner_pid = 0;  // NO owner
    return 0;
}

xf_osal_thread_t xf_osal_mutex_get_owner(xf_osal_mutex_t mutex)
{
    PORT_OSAL_ASSERT(mutex != NULL, NULL, TAG, "mutex != NULL");
    osal_mutex_internal_t *osal_mutex = (osal_mutex_internal_t *)mutex;

    xf_osal_thread_t osal_thread = port_osal_thread_get_by_pid(osal_mutex->owner_pid);
    return osal_thread;
}

xf_err_t xf_osal_mutex_delete(xf_osal_mutex_t mutex)
{
    PORT_OSAL_ASSERT(mutex != NULL, XF_ERR_INVALID_ARG, TAG, "mutex != NULL");
    osal_mutex_internal_t *osal_mutex = (osal_mutex_internal_t *)mutex;
    int ret = pthread_mutex_destroy(&osal_mutex->mutex);
    XF_CHECK(ret != 0, XF_ERR_INVALID_ARG, TAG, "pthread_mutex_destroy err:%d", errno);

    free(mutex);
    return 0;
}

/* ==================== [Static Functions] ================================== */

#endif
