/**
 * @file xf_task.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xfusion 多任务。
 *        xfusion 用户实际包含的 xf_task.h.
 *        使用 xf_utils_config.h 覆盖 xf_hal 内部的 xf_hal.h 的一些配置。
 * @version 1.0
 * @date 2024-08-08
 * @note 此文件不需要防止重复包含。
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#if defined(__DOXYGEN__) && defined(__XFUSION__)
/* 用于将组件内部 doxygen 组添加到 xfusion 文档的 `components_internal` 组中。 */
/**
 * @ingroup group_components_internal
 * @addtogroup group_xf_task
 */
#endif

/* ==================== [Includes] ========================================== */

#include "xf_utils_config.h" /*!< 调用者使用 utils 的 XF_LOG_LEVEL */
#include "xf_task/src/xf_task.h"

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
