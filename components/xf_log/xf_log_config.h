/**
 * @file xf_log_config.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief 使用 xfusion 菜单配置 xf_log 内部配置。
 * @version 1.0
 * @date 2024-08-08
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_LOG_CONFIG_H__
#define __XF_LOG_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/**
 * @brief 新行符。
 */
#if defined(CONFIG_XF_LOG_NEWLINE_LF)
#   define XF_LOG_NEWLINE                   "\n"
#elif defined(CONFIG_XF_LOG_NEWLINE_CRLF)   
#   define XF_LOG_NEWLINE                   "\r\n"
#elif defined(CONFIG_XF_LOG_NEWLINE_NONE)   
#   define XF_LOG_NEWLINE                   ""
#endif

/**
 * @brief 日志动态栈数组缓存大小。
 */
#if defined(CONFIG_XF_LOG_BUF_SIZE)
#   define XF_LOG_BUF_SIZE                  CONFIG_XF_LOG_BUF_SIZE
#endif

/**
 * @brief 是否使用彩色 log。
 */
#if defined(CONFIG_XF_LOG_COLORS_DISABLE)
#   define XF_LOG_COLORS_ENABLE             (0)
#else
#   define XF_LOG_COLORS_ENABLE             (1)
#endif

/**
 * @brief 是否使用 xf_printf() 替代 printf()
 */
#if defined(CONFIG_XF_PRINTF_OVERRIDE_STD_ENABLE)
#   define XF_PRINTF_OVERRIDE_STD_ENABLE    (1)
#else
#   define XF_PRINTF_OVERRIDE_STD_ENABLE    (0)
#endif

/**
 * @brief 是否使用 xf_printf.
 */
#define XF_PRINTF_CUSTOM_XF_PRINTF_ENABLE   (1)
//                                          (0)

/**
 * @brief 'ntoa' 转换缓冲区大小，必须足够大才能容纳一个转换后的缓冲区
 * 包含填充零的数字（在堆栈上动态创建）
 * 默认值：32 字节
 */
#if defined(CONFIG_PRINTF_NTOA_BUFFER_SIZE)
#   define XF_PRINTF_NTOA_BUFFER_SIZE       CONFIG_PRINTF_NTOA_BUFFER_SIZE
#endif

/**
 * @brief 'ftoa' 转换缓冲区大小，必须足够大才能容纳转换后的数据
 * 包含填充零的浮点数（在堆栈上动态创建）
 * 默认值：32 字节
 */
#if defined(CONFIG_XF_PRINTF_FTOA_BUFFER_SIZE)
#   define XF_PRINTF_FTOA_BUFFER_SIZE       CONFIG_XF_PRINTF_FTOA_BUFFER_SIZE
#endif

/**
 * @brief 支持浮点类型（%f）
 */
#if defined(CONFIG_PRINTF_DISABLE_SUPPORT_FLOAT)
#   define XF_PRINTF_DISABLE_SUPPORT_FLOAT  1
#else
#   define XF_PRINTF_SUPPORT_FLOAT          1
#endif

/**
 * @brief 支持指数浮点表示法 (%e/%g)
 */
#if defined(CONFIG_PRINTF_DISABLE_SUPPORT_EXPONENTIAL)
#   define XF_PRINTF_DISABLE_SUPPORT_EXPONENTIAL    1
#else
#   define XF_PRINTF_SUPPORT_EXPONENTIAL            1
#endif

/**
 * @brief 定义默认浮点精度
 */
#define XF_PRINTF_DEFAULT_FLOAT_PRECISION       6U

/**
 * @brief 定义适合用 %f 打印的最大浮点数
 */
#define XF_PRINTF_MAX_FLOAT                     1e9

/**
 * @brief 支持 long long 类型（%llu 或 %p）
 */
#if defined(CONFIG_PRINTF_DISABLE_SUPPORT_LONG_LONG)
#   define XF_PRINTF_DISABLE_SUPPORT_LONG_LONG  1
#else
#   define XF_PRINTF_SUPPORT_LONG_LONG          1
#endif

/**
 * @brief 支持 ptrdiff_t 类型 (%t)
 * ptrdiff_t 通常在 <stddef.h> 中定义为 long 或 long long 类型
 */
#if defined(CONFIG_PRINTF_DISABLE_SUPPORT_PTRDIFF_T)
#   define XF_PRINTF_DISABLE_SUPPORT_PTRDIFF_T  1
#else
#   define XF_PRINTF_SUPPORT_PTRDIFF_T          1
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __XF_LOG_CONFIG_H__ */
