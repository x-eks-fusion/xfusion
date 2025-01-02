/**
 * @file xf_osal_internal.h
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __XF_OSAL_INTERNAL_H__
#define __XF_OSAL_INTERNAL_H__

/* ==================== [Includes] ========================================== */

#include "xf_osal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define PORT_OSAL_ASSERT_ENABLE 1

#if PORT_OSAL_ASSERT_ENABLE
#define PORT_OSAL_ASSERT(condition, retval, tag, format, ...)  XF_ASSERT(condition, retval, tag, format, ##__VA_ARGS__)
#else
#define PORT_OSAL_ASSERT(condition, retval, tag, format, ...)
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

#if XF_OSAL_THREAD_IS_ENABLE

xf_osal_thread_t port_osal_thread_get_by_pid(pthread_t pid);

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_OSAL_INTERNAL_H__
