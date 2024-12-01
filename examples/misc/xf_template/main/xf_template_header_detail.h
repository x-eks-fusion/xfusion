/**
 * @file xf_template_header_detail.h
 * @author 你的名字 (你的邮箱@domain.com)
 * @brief 文件模板。
 * @version 0.1
 * @date 2023-08-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __XF_TEMPLATE_HEADER_DETAIL_H__  /*!< 格式为大写，且以双下划线开头结尾 */
#define __XF_TEMPLATE_HEADER_DETAIL_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

/**
 * @brief extern "C" 的位置通常在 include 后面。
 */
#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/**
 * @brief 模板版本号。
*/
#define XF_TEMPLATE_VERSION_MAJOR (1)
#define XF_TEMPLATE_VERSION_MINOR (0)
#define XF_TEMPLATE_VERSION_PATCH (0)

/* ==================== [Typedefs] ========================================== */

/**
 * @brief 整形错误类型。
 */
typedef xf_err_t xf_templ_err_t;

/**
 * @brief 模板中的错误类型。
 *
 */
typedef enum _xf_templ_err_code_t {
    XF_TEMPL_FAIL = XF_FAIL,                /*!< 通用错误（执行失败） */
    XF_TEMPL_OK = XF_OK,                    /*!< 通用成功（执行成功） */

    XF_TEMPL_ERR_MAX,                       /*!< 错误类型最大值 */
} xf_templ_err_code_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 一个初始化函数。
 *
 * @return xf_templ_err_t
 *      - XF_TEMPL_OK         成功
 */
xf_templ_err_t xf_template_init(void);

/**
 * @brief 一个函数，函数定义在 xf_template_another.c。
 *
 * @return xf_templ_err_t
 *      - XF_TEMPL_OK         成功
 */
xf_templ_err_t xf_template_another_func(void);

/* ==================== [Global Variables] ================================== */

/**
 * @warning 一般**不允许**有全局变量，此处是为了演示必须有全局变量时，
 * 内容标识 [Global Variables] 所在位置。
 */
extern int g_xf_templ_global_data1_in_head;

/* ==================== [Macros] ============================================ */

/**
 * @brief 将版本号转换为整数。与 XF_VERSION_VAL 不同。
 */
#define XF_TEMPLATE_VERSION_CHECK(major, minor, patch) \
    ((major * 10000) + (minor * 100) + patch)

/**
 * @brief 当前模板版本号整数。
 */
#define XF_TEMPLATE_VERSION \
    XF_TEMPLATE_VERSION_CHECK(XF_TEMPLATE_VERSION_MAJOR, \
                              XF_TEMPLATE_VERSION_MINOR, \
                              XF_TEMPLATE_VERSION_PATCH)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_TEMPLATE_HEADER_DETAIL_H__ */
