/**
 * @file xf_osal_semaphore.c
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

#if XF_OSAL_SEMAPHORE_IS_ENABLE

#include <semaphore.h>
#include <time.h>
#include <errno.h>

/* ==================== [Defines] =========================================== */

#define TAG "osal_sem"

/* ==================== [Typedefs] ========================================== */

typedef sem_t osal_semaphore_internal_t;

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_osal_semaphore_t xf_osal_semaphore_create(uint32_t max_count, uint32_t initial_count,
        const xf_osal_semaphore_attr_t *attr)
{
    osal_semaphore_internal_t *osal_sem = (osal_semaphore_internal_t *)malloc(sizeof(osal_semaphore_internal_t));
    if (osal_sem == NULL) {
        return NULL;
    }
    int ret = sem_init(osal_sem, 0, initial_count);
    XF_CHECK(ret != 0, NULL, TAG, "sem_init err:%d", errno);
    return (xf_osal_semaphore_t)osal_sem;
}

xf_err_t xf_osal_semaphore_acquire(xf_osal_semaphore_t semaphore, uint32_t timeout)
{
    PORT_OSAL_ASSERT(semaphore != NULL, XF_ERR_INVALID_ARG, TAG, "semaphore != NULL");
    osal_semaphore_internal_t *osal_sem = (osal_semaphore_internal_t *)semaphore;
    int ret;
    if (timeout == 0) {
        ret = sem_wait(osal_sem);
        XF_CHECK(ret != 0, XF_ERR_INVALID_STATE, TAG, "sem_wait err:%d", errno);
    } else {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += timeout / 1000;
        ts.tv_nsec += (timeout % 1000) * 1000000;
        ret = sem_timedwait(osal_sem, &ts);
        XF_CHECK(ret != 0, XF_ERR_INVALID_STATE, TAG, "sem_timedwait err:%d", errno);
    }
    return XF_OK;
}
xf_err_t xf_osal_semaphore_release(xf_osal_semaphore_t semaphore)
{
    PORT_OSAL_ASSERT(semaphore != NULL, XF_ERR_INVALID_ARG, TAG, "semaphore != NULL");
    osal_semaphore_internal_t *osal_sem = (osal_semaphore_internal_t *)semaphore;
    int ret = sem_post(osal_sem);
    XF_CHECK(ret != 0, XF_ERR_INVALID_STATE, TAG, "sem_getvalue err:%d", errno);
    return XF_OK;
}

uint32_t xf_osal_semaphore_get_count(xf_osal_semaphore_t semaphore)
{
    PORT_OSAL_ASSERT(semaphore != NULL, XF_ERR_INVALID_ARG, TAG, "semaphore != NULL");
    osal_semaphore_internal_t *osal_sem = (osal_semaphore_internal_t *)semaphore;
    uint32_t cnt = 0;
    int ret = sem_getvalue(osal_sem, &cnt);
    XF_CHECK(ret != 0, 0, TAG, "sem_getvalue err:%d", errno);
    return cnt;
}

xf_err_t xf_osal_semaphore_delete(xf_osal_semaphore_t semaphore)
{
    PORT_OSAL_ASSERT(semaphore != NULL, XF_ERR_INVALID_ARG, TAG, "semaphore != NULL");
    osal_semaphore_internal_t *osal_sem = (osal_semaphore_internal_t *)semaphore;
    int ret = sem_destroy(osal_sem);
    XF_CHECK(ret != 0, NULL, TAG, "sem_destroy err:%d", errno);
    free(osal_sem);
    return ret;
}

/* ==================== [Static Functions] ================================== */

#endif
