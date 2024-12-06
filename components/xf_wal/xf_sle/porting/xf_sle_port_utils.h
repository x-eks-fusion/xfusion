/**
 * @file xf_sle_port_utils.h
 * @author dotc (dotchan@qq.com)
 * @brief 主要为 SLE 对接时辅助的一些方法，可简化对接处理。
 * @version 0.1
 * @date 2024-12-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/**
 * @cond (XFAPI_USER || XFAPI_PORT || XFAPI_INTERNAL)
 * @ingroup group_xf_wal_sle
 * @defgroup group_xf_wal_sle_port utils
 * @brief 主要为 SLE 对接时辅助的一些方法，可简化对接处理。
 * @endcond
 */

#ifndef __XF_SLE_PORT_UTILS_H__
#define __XF_SLE_PORT_UTILS_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#include "xf_sle_types.h"
#include "xf_sle_device_discovery_types.h"

#if XF_SLE_IS_ENABLE || defined(__DOXYGEN__)

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_wal_sle_port
 * @endcond
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

uint8_t xf_sle_adv_data_packed_size_get(xf_sle_adv_struct_t *adv_struct_set);
xf_err_t xf_sle_adv_data_packed_by_adv_struct_set(
    uint8_t *adv_data_buf, xf_sle_adv_struct_t *adv_struct_set);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_wal_sle_port
 * @}
 */

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_PORT_UTILS_H__ */
