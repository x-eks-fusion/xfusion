/**
 * @file xf_sle_connection_manager.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-08-06
 * 
 * Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef __XF_SLE_XF_SLE_CONNECTION_MANAGER_H__
#define __XF_SLE_XF_SLE_CONNECTION_MANAGER_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_sle_types.h"
#include "xf_sle_connection_manager_types.h"

#if XF_SLE_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

// 更新连接参数
xf_err_t xf_sle_update_conn_params(
    xf_sle_conn_param_update_t *params);
// 与指定地址设备进行连接
xf_err_t xf_sle_connect(const xf_sle_addr_t *addr);
// 与指定地址设备断开连接
xf_err_t xf_sle_disconnect(const xf_sle_addr_t *addr);
// 与所有设备断开连接
xf_err_t xf_sle_disconnect_all(void);
// 与指定地址设备进行配对
xf_err_t xf_sle_add_pair(const xf_sle_addr_t *addr);
// 与指定地址设备取消配对
xf_err_t xf_sle_del_pair(const xf_sle_addr_t *addr);
// 与所有设备取消配对
xf_err_t xf_sle_del_pair_all(void);
// 获取已配对的设备
xf_err_t xf_sle_get_pair_list(
    uint16_t *max_num, xf_sle_addr_t *dev_list);
// 获取已绑定的设备
xf_err_t xf_sle_get_bond_list(
    int *max_num, xf_sle_addr_t *dev_list);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_SLE_IS_ENABLE */

#endif /* __XF_SLE_XF_SLE_CONNECTION_MANAGER_H__ */
