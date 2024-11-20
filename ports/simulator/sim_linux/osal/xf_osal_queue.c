/**
 * @file xf_osal_queue.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-09-11
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_osal_internal.h"

#if XF_OSAL_QUEUE_IS_ENABLE

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

/* ==================== [Defines] =========================================== */

#define TAG "oasl_queue"

/* ==================== [Typedefs] ========================================== */

typedef struct {
    mqd_t mq;
    struct mq_attr mq_attr;
    uint8_t path_size;
    uint8_t path[0];
} osal_queue_internal_t;

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_osal_queue_t xf_osal_queue_create(uint32_t msg_count, uint32_t msg_size, const xf_osal_queue_attr_t *attr)
{
    uint16_t path_size = strlen(attr->name) + 2;
    osal_queue_internal_t *osal_queue = malloc(sizeof(osal_queue_internal_t) + path_size);
    XF_CHECK(osal_queue == NULL, NULL, TAG, "malloc osal_queue failed!");

    osal_queue->mq_attr.mq_flags = 0;
    osal_queue->mq_attr.mq_maxmsg = msg_count;
    osal_queue->mq_attr.mq_msgsize = msg_size;
    osal_queue->mq_attr.mq_curmsgs = 0;

    osal_queue->path_size = path_size;
    snprintf(osal_queue->path, osal_queue->path_size, "/%s", attr->name);

    osal_queue->mq = mq_open(osal_queue->path, O_CREAT | O_RDWR, 0644, &osal_queue->mq_attr);
    if (osal_queue->mq == ((mqd_t) -1)) {
        free(osal_queue);
        XF_LOGE(TAG, "mq_open err:%d", errno);
        return NULL;
    }

    return (xf_osal_queue_t)osal_queue;
}

xf_err_t xf_osal_queue_put(xf_osal_queue_t queue, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout)
{
    PORT_OSAL_ASSERT(queue != NULL, XF_ERR_INVALID_ARG, TAG, "queue != NULL");
    osal_queue_internal_t *osal_queue = (osal_queue_internal_t *)queue;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += timeout;

    int ret = mq_timedsend(osal_queue->mq, msg_ptr,
                           osal_queue->mq_attr.mq_msgsize, msg_prio, &ts);
    XF_CHECK(ret == -1, XF_ERR_INVALID_STATE, TAG, "mq_timedsend err:%d", errno);

    return XF_OK;
}

xf_err_t xf_osal_queue_get(xf_osal_queue_t queue, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout)
{
    PORT_OSAL_ASSERT(queue != NULL, XF_ERR_INVALID_ARG, TAG, "queue != NULL");
    osal_queue_internal_t *osal_queue = (osal_queue_internal_t *)queue;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += timeout;

    int ret = mq_timedreceive(osal_queue->mq, msg_ptr,
                              osal_queue->mq_attr.mq_msgsize, (unsigned int *)msg_prio, &ts);
    XF_CHECK(ret == -1, XF_ERR_INVALID_STATE, TAG, "mq_timedreceive err:%d", errno);

    return XF_OK;
}

uint32_t xf_osal_queue_get_count(xf_osal_queue_t queue)
{
    PORT_OSAL_ASSERT(queue != NULL, XF_ERR_INVALID_ARG, TAG, "queue != NULL");
    osal_queue_internal_t *osal_queue = (osal_queue_internal_t *)queue;

    struct mq_attr mq_attr;
    int ret = mq_getattr(osal_queue->mq, &mq_attr);
    XF_CHECK(ret == -1, 0, TAG, "mq_timedreceive err:%d", errno);

    return mq_attr.mq_curmsgs;
}

xf_err_t xf_osal_queue_reset(xf_osal_queue_t queue)
{
    return XF_ERR_NOT_SUPPORTED;
}

xf_err_t xf_osal_queue_delete(xf_osal_queue_t queue)
{
    PORT_OSAL_ASSERT(queue != NULL, XF_ERR_INVALID_ARG, TAG, "queue != NULL");
    osal_queue_internal_t *osal_queue = (osal_queue_internal_t *)queue;

    int ret = 0;
    ret = mq_close(osal_queue->mq);
    XF_CHECK(ret == -1, XF_ERR_INVALID_STATE, TAG, "mq_close err:%d", errno);

    ret = mq_unlink(osal_queue->path);
    XF_CHECK(ret == -1, XF_ERR_INVALID_STATE, TAG, "mq_unlink err:%d", errno);

    free(osal_queue);
    return XF_OK;
}

/* ==================== [Static Functions] ================================== */

#endif
