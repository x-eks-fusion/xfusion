/**
 * @file xf_ble_port_utils.h
 * @author dotc (dotchan@qq.com)
 * @brief 主要为 BLE 对接时辅助的一些方法，可简化对接处理。
 * @version 0.1
 * @date 2024-12-06
 *
 * @copyright Copyright (c) 2024
 *
 */

/**
 * @cond (XFAPI_USER || XFAPI_PORT || XFAPI_INTERNAL)
 * @ingroup group_xf_wal_ble
 * @defgroup group_xf_wal_ble_port utils
 * @brief 主要为 BLE 对接时辅助的一些方法，可简化对接处理。
 * @endcond
 */

#ifndef __XF_BLE_PORT_UTILS_H__
#define __XF_BLE_PORT_UTILS_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

#include "xf_ble_types.h"
#include "xf_ble_gap_types.h"

#if XF_BLE_IS_ENABLE || defined(__DOXYGEN__)

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_wal_ble_port
 * @endcond
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief XF BLE GAP 广播 (或扫描响应数据) 数据包的大小获取 (根据广播数据单元集合的信息)
 *
 * @note 本方法通常用于广播数据对接时调用
 * @param adv_struct_set 广播 (或扫描响应数据) 数据单元集合 (扫描响应数据也使用同一类型的单元结构)，
 *  见 @ref xf_ble_gap_adv_struct_t
 * @return uint8_t 获取广播数据包到的大小
 */
uint8_t xf_ble_gap_adv_data_packed_size_get(xf_ble_gap_adv_struct_t *adv_struct_set);

/**
 * @brief XF BLE GAP 根据数据单元集合进行解析、打包至广播 (或扫描响应数据) 数据包中
 *
 * @param adv_data_buf 广播 (或扫描响应数据) 数据包的地址，将打包数据至此
 * @param adv_struct_set 广播 (或扫描响应数据) 数据单元集合 (扫描响应数据也使用同一类型的单元结构)，
 *  见 @ref xf_ble_gap_adv_struct_t
 * @return xf_err_t
 */
xf_err_t xf_ble_gap_adv_data_packed_by_adv_struct_set(
    uint8_t *adv_data_buf, xf_ble_gap_adv_struct_t *adv_struct_set);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_wal_ble_port
 * @}
 */

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_PORT_UTILS_H__ */
