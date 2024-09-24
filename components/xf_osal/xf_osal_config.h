/**
 * @file xf_osal_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_OSAL_CONFIG_H__
#define __XF_OSAL_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_OSAL_KERNEL_ENABLE CONFIG_XF_OSAL_KERNEL_ENABLE

#define XF_OSAL_TIMER_ENABLE CONFIG_XF_OSAL_TIMER_ENABLE

#define XF_OSAL_EVENT_ENABLE CONFIG_XF_OSAL_EVENT_ENABLE

#define XF_OSAL_MUTEX_ENABLE CONFIG_XF_OSAL_MUTEX_ENABLE

#define XF_OSAL_SEMAPHORE_ENABLE CONFIG_XF_OSAL_SEMAPHORE_ENABLE

#define XF_OSAL_QUEUE_ENABLE CONFIG_XF_OSAL_QUEUE_ENABLE

#if CONFIG_XF_OSAL_CMSIS

#define XF_CMSIS_KERNEL_GET_INFO_ENABLE CONFIG_XF_CMSIS_KERNEL_GET_INFO_ENABLE

#define XF_CMSIS_KERNEL_GET_STATE_ENABLE CONFIG_XF_CMSIS_KERNEL_GET_STATE_ENABLE

#define XF_CMSIS_KERNEL_LOCK_ENABLE CONFIG_XF_CMSIS_KERNEL_LOCK_ENABLE

#define XF_CMSIS_KERNEL_UNLOCK_ENABLE CONFIG_XF_CMSIS_KERNEL_UNLOCK_ENABLE

#define XF_CMSIS_THREAD_GET_NAME_ENABLE CONFIG_XF_CMSIS_THREAD_GET_NAME_ENABLE

#define XF_CMSIS_THREAD_SET_PRIORITY_ENABLE CONFIG_XF_CMSIS_THREAD_SET_PRIORITY_ENABLE

#define XF_CMSIS_THREAD_YIELD_ENABLE CONFIG_XF_CMSIS_THREAD_YIELD_ENABLE

#define XF_CMSIS_THREAD_GET_COUNT_ENABLE CONFIG_XF_CMSIS_THREAD_GET_COUNT_ENABLE

#define XF_CMSIS_THREAD_GET_ACTIVE_COUNT_ENABLE CONFIG_XF_CMSIS_THREAD_GET_ACTIVE_COUNT_ENABLE

#define XF_CMSIS_THREAD_NOTIFY_ENABLE CONFIG_XF_CMSIS_THREAD_NOTIFY_ENABLE

#define XF_CMSIS_TIMER_GET_NAME_ENABLE CONFIG_XF_CMSIS_TIMER_GET_NAME_ENABLE

#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_OSAL_CONFIG_H__
