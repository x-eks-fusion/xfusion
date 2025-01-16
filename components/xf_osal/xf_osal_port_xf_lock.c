/**
 * @file xf_osal_port_xf_lock.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2025-01-13
 *
 * @copyright Copyright (c) 2025
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_utils_port.h"
#include "xf_init.h"

#include "xf_osal.h"

#if XF_OSAL_MUTEX_IS_ENABLE

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static int port_xf_lock(void);

static xf_err_t _port_xf_lock_init(xf_lock_t *p_lock);
static xf_err_t _port_xf_lock_destroy(xf_lock_t lock);
static int _port_xf_lock_trylock(xf_lock_t lock);
static int _port_xf_lock_lock(xf_lock_t lock);
static int _port_xf_lock_timedlock(xf_lock_t lock, uint32_t timeout_ms);
static int _port_xf_lock_unlock(xf_lock_t lock);

/* ==================== [Static Variables] ================================== */

static const xf_lock_ops_t lock_ops = {
    .init       = _port_xf_lock_init,
    .destroy    = _port_xf_lock_destroy,
    .trylock    = _port_xf_lock_trylock,
    .lock       = _port_xf_lock_lock,
    .timedlock  = _port_xf_lock_timedlock,
    .unlock     = _port_xf_lock_unlock,
};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static int port_xf_lock(void)
{
    return xf_lock_register(&lock_ops);
}
XF_INIT_EXPORT_PREV(port_xf_lock);

static xf_err_t _port_xf_lock_init(xf_lock_t *p_lock)
{
    xf_osal_mutex_t mutex = NULL;
    xf_osal_mutex_attr_t mutex_attr = {
        .name = "mutex",
    };
    mutex = xf_osal_mutex_create(&mutex_attr);
    if (mutex == NULL) {
        return XF_FAIL;
    }
    *(uintptr_t *)p_lock = (uintptr_t)mutex;
    return XF_OK;
}

static xf_err_t _port_xf_lock_destroy(xf_lock_t lock)
{
    xf_osal_mutex_t mutex = (xf_osal_mutex_t)lock;
    return xf_osal_mutex_delete(mutex);
}

static int _port_xf_lock_trylock(xf_lock_t lock)
{
    xf_osal_mutex_t mutex = (xf_osal_mutex_t)lock;
    xf_err_t xf_ret = xf_osal_mutex_acquire(mutex, 0);
    return (xf_ret == XF_OK) ? XF_LOCK_SUCC : XF_LOCK_FAIL;
}

static int _port_xf_lock_lock(xf_lock_t lock)
{
    xf_osal_mutex_t mutex = (xf_osal_mutex_t)lock;
    xf_err_t xf_ret = xf_osal_mutex_acquire(mutex, XF_OSAL_WAIT_FOREVER);
    return (xf_ret == XF_OK) ? XF_LOCK_SUCC : XF_LOCK_FAIL;
}

static int _port_xf_lock_timedlock(xf_lock_t lock, uint32_t timeout_ms)
{
    xf_osal_mutex_t mutex = (xf_osal_mutex_t)lock;
    xf_err_t xf_ret = xf_osal_mutex_acquire(mutex, xf_osal_kernel_ms_to_ticks(timeout_ms));
    return (xf_ret == XF_OK) ? XF_LOCK_SUCC : XF_LOCK_FAIL;
}

static int _port_xf_lock_unlock(xf_lock_t lock)
{
    xf_osal_mutex_t mutex = (xf_osal_mutex_t)lock;
    xf_err_t xf_ret = xf_osal_mutex_release(mutex);
    return (xf_ret == XF_OK) ? XF_LOCK_SUCC : XF_LOCK_FAIL;
}

#endif /* XF_OSAL_MUTEX_IS_ENABLE */
