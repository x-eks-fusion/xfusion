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
#else /* 如果前两个没定义则不启用新行，不管 CONFIG_XF_LOG_NEWLINE_NONE 是否定义 */
#   define XF_LOG_NEWLINE                   ""
#endif

// 打开彩色打印，xf_log_config.h 中如果定义 XF_LOG_COLORS_ENABLE 为 0 则关闭
#define XF_LOG_COLORS_ENABLE CONFIG_XF_LOG_COLORS_ENABLE

// 打开过滤器，xf_log_config.h 中如果定义 XF_LOG_FILTER_ENABLE 为 0 则关闭
#define XF_LOG_FILTER_ENABLE CONFIG_XF_LOG_FILTER_ENABLE

// ctype.h头文件的支持，如果关闭则启用内部宏实现 isdigit 函数
#define XF_LOG_CTYPE_ENABLE CONFIG_XF_LOG_CTYPE_ENABLE

// stddef.h头文件的支持，如果关闭则启用内部实现 size_t 类型
#define XF_LOG_STDDEF_ENABLE CONFIG_XF_LOG_STDDEF_ENABLE

// stdint.h头文件的支持，如果关闭则启用内部实现 uint32_t uint8_t 类型
#define XF_LOG_STDINT_ENABLE CONFIG_XF_LOG_STDINT_ENABLE

// string.h头文件的支持，如果关闭则需要手动实现 xf_log_strlen(s) 函数
#define XF_LOG_STRLEN_ENABLE CONFIG_XF_LOG_STRLEN_ENABLE

// stdio.h头文件的支持，如果关闭则需要手动实现 xf_log_vsprintf(buffer, maxlen, fmt, args) 函数
#define XF_LOG_VSNPRINTF_ENABLE CONFIG_XF_LOG_VSNPRINTF_ENABLE

// 后端对接的输出对象数目，默认为一个对象
#define XF_LOG_OBJ_NUM CONFIG_XF_LOG_OBJ_NUM

// 格式化标志长度
#define XF_FORMAT_FLAG_SIZE CONFIG_XF_FORMAT_FLAG_SIZE

// 格式化结果缓冲区
#define XF_FORMAT_BUFFER_SIZE CONFIG_XF_FORMAT_BUFFER_SIZE

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __XF_LOG_CONFIG_H__ */
