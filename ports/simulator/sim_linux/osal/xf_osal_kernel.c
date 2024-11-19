/**
 * @file xf_osal_kernel.c
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

#if XF_OSAL_KERNEL_IS_ENABLE

#include <pthread.h>
#include <time.h>

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_err_t xf_osal_kernel_get_info(xf_osal_version_t *version, char *id_buf, uint32_t id_size)
{
    return XF_ERR_NOT_SUPPORTED;
}

xf_osal_state_t xf_osal_kernel_get_state(void)
{
    return 0;
}

static pthread_mutex_t kernel_mutex = PTHREAD_MUTEX_INITIALIZER;

xf_err_t xf_osal_kernel_lock(void) {
    return XF_ERR_NOT_SUPPORTED;
}

xf_err_t xf_osal_kernel_unlock(void) {
    return XF_ERR_NOT_SUPPORTED;
}

uint32_t xf_osal_kernel_get_tick_count(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

uint32_t xf_osal_kernel_get_tick_freq(void) {
    return 1000; // 1000 ticks per second
}

uint32_t xf_osal_kernel_ticks_to_ms(uint32_t ticks) {
    return ticks; // Assuming 1 tick = 1 ms
}

uint32_t xf_osal_kernel_ms_to_ticks(uint32_t ms) {
    return ms; // Assuming 1 tick = 1 ms
}

/* ==================== [Static Functions] ================================== */


#endif
