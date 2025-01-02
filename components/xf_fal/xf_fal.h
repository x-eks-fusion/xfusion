/**
 * @file xf_fal.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xfusion flash 抽象层。
 *        xfusion 用户实际包含的 xf_fal.h.
 *        使用 xf_utils_config.h 覆盖 xf_fal 内部的 xf_fal.h 的一些配置。
 * @version 0.1
 * @date 2024-10-18
 * @note 此文件不需要防止重复包含。
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#if defined(__DOXYGEN__) && defined(__XFUSION__) && defined(__DOXYGEN_ADD_COMPONENTS__)
/* 用于将组件内部 doxygen 组添加到 xfusion 文档的 `components_internal` 组中。 */
/**
 * @cond (XFAPI_USER || XFAPI_PORT || XFAPI_INTERNAL)
 * @ingroup group_components_internal
 * @addtogroup group_xf_fal
 * @endcond
 */
#endif

/* ==================== [Includes] ========================================== */

#include "xf_utils_config.h" /*!< 调用者使用 utils 的 XF_LOG_LEVEL */
#include "xf_fal/src/xf_fal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif
