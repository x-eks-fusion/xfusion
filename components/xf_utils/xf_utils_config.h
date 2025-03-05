/**
 * @file xf_utils_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief 使用 xfusion 菜单配置 xf_utils 内部配置。
 * @version 0.1
 * @date 2024-07-01
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_UTILS_CONFIG_H__
#define __XF_UTILS_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "xfconfig.h"
#include "xf_heap.h"
#include "xf_log.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/**
 * @brief 是否include stddef.h
 *
 */
#define XF_STDDEF_ENABLE                CONFIG_XF_STDDEF_ENABLE

/**
 * @brief 是否include stdint.h
 *
 */
#define XF_STDINT_ENABLE                CONFIG_XF_STDINT_ENABLE

/**
 * @brief 是否include stdbool.h
 *
 */
#define XF_STDBOOL_ENABLE               CONFIG_XF_STDBOOL_ENABLE

/**
 * @brief 是否include stdlib.h
 *
 */
#define XF_STDLIB_ENABLE                CONFIG_XF_STDLIB_ENABLE

/**
 * @brief 是否include string.h
 *
 */
#define XF_STRING_ENABLE                CONFIG_XF_STRING_ENABLE

/**
 * @brief 是否include stdio.h
 *
 */
#define XF_STDIO_ENABLE                 CONFIG_XF_STDIO_ENABLE

/**
 * @brief XF_ERROR_CHECK宏错误处理的内容
 *
 */
#define XF_CHECK_ERROR_HANDLER          while(1);

/**
 * @brief 是否使能错误码值到字符串的查找表(`XF_OK`->"XF_OK").
 */
#define XF_COMMON_ERR_TO_NAME_LOOKUP_DISABLE    CONFIG_XF_COMMON_ERR_TO_NAME_LOOKUP_DISABLE

/**
 * @brief 是否使能__attribute__功能
 *
 */
#define XF_ATTRIBUTE_ENABLE                     CONFIG_XF_ATTRIBUTE_ENABLE

/**
 * @brief 是否开启XF_CHECK宏
 *
 */
#define XF_CHECK_ENABLE                         CONFIG_XF_CHECK_ENABLE

/**
 * @brief 是否开启XF_ASSERT宏
 *
 */
#define XF_ASSERT_ENABLE                        CONFIG_XF_ASSERT_ENABLE

/**
 * @brief 是否开启XF_ERROR_CHECK宏
 *
 */
#define XF_ERROR_CHECK_ENABLE                   CONFIG_XF_ERROR_CHECK_ENABLE

/**
 * @brief 对接xf_log的等级
 *
 */

#ifndef XF_LOG_LEVEL
#define XF_LOG_LEVEL                            CONFIG_XF_LOG_LEVEL
#endif

/**
 * @brief 是否需要打印数组
 *
 */
#define XF_LOG_DUMP_ENABLE                      CONFIG_XF_LOG_DUMP_ENABLE

/**
 * @brief 主要版本号 (X.x.x).
 */
#define XF_VERSION_MAJOR   1

/**
 * @brief 次要版本号 (x.X.x).
 */
#define XF_VERSION_MINOR   3

/**
 * @brief 补丁版本号 (x.x.X).
 */
#define XF_VERSION_PATCH   0

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#define xf_log_printf(format, ...)              xf_log_printf(format, ##__VA_ARGS__)

#define xf_log_dump_printf(format, ...)         xf_log_printf(format, ##__VA_ARGS__)

/**
 * @brief 使用xf_heap替代malloc
 *
 */
#define xf_user_malloc(x)               xf_malloc(x)

/**
 * @brief 使用xf_heap替代free
 *
 */
#define xf_user_free(x)                 xf_free(x)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_UTILS_CONFIG_H__
