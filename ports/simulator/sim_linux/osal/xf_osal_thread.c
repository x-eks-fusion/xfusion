/**
 * @file xf_osal_thread.c
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-18
 *
 * @copyright Copyright (c) 2024
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_osal_internal.h"

#if XF_OSAL_THREAD_IS_ENABLE

#include <pthread.h>
#include <unistd.h>
#include <errno.h>

/* ==================== [Defines] =========================================== */

#define TAG "osal_thread"

/* ==================== [Typedefs] ========================================== */

typedef struct {
    pthread_t pid;
    xf_osal_thread_func_t func;
    void *arg;
    uint8_t *name;
    xf_list_t link;
} osal_thread_internal_t;

/* ==================== [Static Prototypes] ================================= */

static int _pthread_get_policy(pthread_attr_t *attr);

/* ==================== [Static Variables] ================================== */

pthread_mutex_t s_mutex_list = PTHREAD_MUTEX_INITIALIZER;
static xf_list_t s_list_thread = XF_LIST_HEAD_INIT(s_list_thread);

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_osal_thread_t xf_osal_thread_create(xf_osal_thread_func_t func, void *argument, const xf_osal_thread_attr_t *attr)
{
    PORT_OSAL_ASSERT(func != NULL, NULL, TAG, "func != NULL");
    PORT_OSAL_ASSERT(attr != NULL, NULL, TAG, "attr != NULL");
    PORT_OSAL_ASSERT(attr->stack_size != 0, NULL, TAG, "attr->stack_size != 0");
    PORT_OSAL_ASSERT(attr->priority != XF_OSAL_PRIORITY_NONE, NULL,
                     TAG, "attr->priority != XF_OSAL_PRIORITY_NONE");

    osal_thread_internal_t *osal_thread = (osal_thread_internal_t *)malloc(sizeof(osal_thread_internal_t));
    if (osal_thread == NULL) {
        return NULL;
    }

    osal_thread->func = func;
    osal_thread->arg = argument;
    osal_thread->name = (uint8_t *)attr->name;

    pthread_attr_t thread_attr;
    // 初始化线程属性
    pthread_attr_init(&thread_attr);

    // 设置任务栈
    pthread_attr_setstacksize(&thread_attr, attr->stack_size);

    /* FIXME 暂时不支持优先级：不知道是否是权限问题，无法设置其他调度策略，因此无法自定义优先级 */
    // /* 设置优先级 */
    // struct sched_param sched_param;
    // if (attr->priority != XF_OSAL_PRIORITY_NONE) {
    //     pthread_attr_setschedpolicy(&thread_attr, SCHED_FIFO);
    //     sched_param.sched_priority = attr->priority;
    //     pthread_attr_setschedparam(&thread_attr, &sched_param);
    // }

    int ret = pthread_create(&osal_thread->pid, &thread_attr, (void *(*)(void *))func, argument);

    if (ret != 0) {
        XF_LOGE(TAG, "pthread_create err:%d", errno);
        free(osal_thread);
        return NULL;
    }

    // 销毁线程属性对象
    pthread_attr_destroy(&thread_attr);

    // 加到 list 中，用于后续
    pthread_mutex_lock(&s_mutex_list);
    xf_list_add_tail(&osal_thread->link, &s_list_thread);
    pthread_mutex_unlock(&s_mutex_list);
    return (xf_osal_thread_t)osal_thread;
}

void xf_osal_thread_exit(void)
{
    pthread_exit(NULL);
}

xf_osal_thread_t xf_osal_thread_get_current(void)
{
    pthread_t pid = pthread_self();
    return port_osal_thread_get_by_pid(pid);
}

xf_err_t xf_osal_thread_set_priority(xf_osal_thread_t thread, xf_osal_priority_t priority)
{
    return XF_ERR_NOT_SUPPORTED;
    /* FIXME 暂时不支持优先级：不知道是否是权限问题，无法设置其他调度策略，因此无法自定义优先级 */
    // osal_thread_internal_t *osal_thread = (osal_thread_internal_t *)thread;

    // struct sched_param param;
    // int policy = SCHED_FIFO;    // 设置线程的调度策略为 FIFO 默认的 SCHED_NORMAL 不分优先级

    // // 设置线程优先级
    // param.sched_priority = priority;
    // int ret = pthread_setschedparam(osal_thread->pid, policy, &param);
    // XF_CHECK(ret !=0, XF_ERR_INVALID_STATE, TAG, "pthread_setschedparam err:%d", ret);

    // return XF_OK;
}

xf_osal_priority_t xf_osal_thread_get_priority(xf_osal_thread_t thread)
{
    return XF_ERR_NOT_SUPPORTED;
    /* FIXME 暂时不支持优先级：不知道是否是权限问题，无法设置其他调度策略，因此无法自定义优先级 */
    // osal_thread_internal_t *osal_thread = (osal_thread_internal_t *)thread;

    // struct sched_param param;
    // int policy;

    // // 获取当前线程的调度策略和优先级
    // if (pthread_getschedparam(osal_thread->pid, &policy, &param) != 0) {
    //     return XF_OSAL_PRIORITY_ERROR;
    // }

    // // 返回获取到的优先级
    // return (xf_osal_priority_t)param.sched_priority;
}

xf_err_t xf_osal_thread_delete(xf_osal_thread_t thread)
{
    osal_thread_internal_t *osal_thread = (osal_thread_internal_t *)thread;
    /* NULL 时获取当前线程 */
    if (osal_thread == NULL) {
        osal_thread = xf_osal_thread_get_current();
    }
    pthread_mutex_lock(&s_mutex_list);
    xf_list_del(&osal_thread->link);
    pthread_mutex_unlock(&s_mutex_list);
    int ret = pthread_cancel(osal_thread->pid);
    XF_CHECK(ret != 0, NULL, TAG, "pthread_cancel err:%d", errno);
    
    free(osal_thread);
    return XF_OK;
}

xf_err_t xf_osal_delay_ms(uint32_t ms)
{
    return usleep(ms * 1000) == 0 ? XF_OK : XF_FAIL;
}

xf_err_t xf_osal_delay(uint32_t ticks)
{
    return usleep(ticks * 1000) == 0 ? XF_OK : XF_FAIL;
}

xf_err_t xf_osal_delay_until(uint32_t ticks)
{
    // 自定义实现，使用定时器
    return XF_OK;
}

const char *xf_osal_thread_get_name(xf_osal_thread_t thread)
{
    osal_thread_internal_t *osal_thread = (osal_thread_internal_t *)thread;
    /* NULL 时获取当前线程 */
    if (osal_thread == NULL) {
        osal_thread = xf_osal_thread_get_current();
    }
    return osal_thread->name;
}

xf_osal_state_t xf_osal_thread_get_state(xf_osal_thread_t thread)
{
    return XF_ERR_NOT_SUPPORTED;
}

uint32_t xf_osal_thread_get_stack_space(xf_osal_thread_t thread)
{
    return 0;
}

xf_err_t xf_osal_thread_yield(void)
{
    return sched_yield() == 0 ? XF_OK : XF_FAIL;
}

xf_err_t xf_osal_thread_suspend(xf_osal_thread_t thread)
{
    return XF_ERR_NOT_SUPPORTED;
}

xf_err_t xf_osal_thread_resume(xf_osal_thread_t thread)
{
    return XF_ERR_NOT_SUPPORTED;
}

uint32_t xf_osal_thread_get_count(void)
{
    return 0;
}

uint32_t xf_osal_thread_get_active_count(xf_osal_thread_t *thread_array, uint32_t array_items)
{
    return 0;
}

xf_err_t xf_osal_thread_notify_set(xf_osal_thread_t thread, uint32_t notify)
{
    return XF_ERR_NOT_SUPPORTED;
}

xf_err_t xf_osal_thread_notify_clear(uint32_t notify)
{
    return XF_ERR_NOT_SUPPORTED;
}

uint32_t xf_osal_thread_notify_get(void)
{
    return XF_ERR_NOT_SUPPORTED;
}

xf_err_t xf_osal_thread_notify_wait(uint32_t notify, uint32_t options, uint32_t timeout)
{
    return XF_ERR_NOT_SUPPORTED;
}

/* ==================== [Static Functions] ================================== */

static int _pthread_get_policy(pthread_attr_t *attr)
{
    int policy;
    int ret = pthread_attr_getschedpolicy(attr, &policy);
    XF_ASSERT(ret == 0, 0, TAG, "pthread_attr_getschedpolicy == 0");
    switch (policy) {
    case SCHED_FIFO:
        XF_LOGI(TAG, "policy = SCHED_FIFO");
        break;

    case SCHED_RR:
        XF_LOGI(TAG, "policy = SCHED_RR");
        break;

    case SCHED_OTHER:
        XF_LOGI(TAG, "policy = SCHED_OTHER");
        break;

    default:
        XF_LOGI(TAG, "policy = UNKNOWN");
        break;
    }

    return policy;
}

xf_osal_thread_t port_osal_thread_get_by_pid(pthread_t pid)
{
    osal_thread_internal_t *thread, *temp;
    pthread_mutex_lock(&s_mutex_list);
    xf_list_for_each_entry_safe(thread, temp, &s_list_thread,
                                osal_thread_internal_t, link) {
        if (thread->pid == pid) {
            pthread_mutex_unlock(&s_mutex_list);
            return (xf_osal_thread_t)thread;
        }
    }
    pthread_mutex_unlock(&s_mutex_list);
    return NULL;
}

#endif
