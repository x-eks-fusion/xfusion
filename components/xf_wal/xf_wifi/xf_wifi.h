/**
 * @file xf_wifi.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief xf_wifi 包含 STA、AP 接口。
 * @version 1.0
 * @date 2024-09-14
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/**
 * @cond (XFAPI_USER || XFAPI_PORT || XFAPI_INTERNAL)
 * @ingroup group_xf_wal
 * @defgroup group_xf_wal_wifi xf_wifi
 * @brief xf_wifi 包含 STA、AP 接口。
 * @endcond
 */

#ifndef __XF_WIFI_H__
#define __XF_WIFI_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_netif_types.h"
#include "xf_wifi_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_wal_wifi
 * @endcond
 * @{
 */

/**
 * @brief 启用 wifi 协议栈.
 *
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败或重复启用
 */
xf_err_t xf_wifi_enable(void);

/**
 * @brief 禁用 wifi 协议栈.
 *
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败或者未启用
 */
xf_err_t xf_wifi_disable(void);

/**
 * @brief wifi 设置 mac 地址。
 *
 * 该接口只有在 AP 或者 STA 启动前有效。
 *
 * @param ifx 接口，选择 STA 或者 AP。
 * @param mac 新的 mac 地址。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - XF_ERR_NOT_SUPPORTED  不支持
 */
xf_err_t xf_wifi_set_mac(
    xf_wifi_interface_t ifx, const uint8_t mac[XF_MAC_LEN_MAX]);

/**
 * @brief wifi 获取 mac 地址。
 *
 * @param ifx 接口，选择 STA 或者 AP。
 * @param[out] mac 读取到的 mac 地址。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_wifi_get_mac(
    xf_wifi_interface_t ifx, uint8_t mac[XF_MAC_LEN_MAX]);

/**
 * End of addtogroup group_xf_wal_wifi
 * @}
 */

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @ingroup group_xf_wal_wifi
 * @defgroup group_xf_wal_wifi_ap ap
 * @brief xf_wifi ap.
 * @endcond
 * @{
 */

/**
 * @brief 启用 wifi AP 模式.
 *
 * @note AP 事件只能通过 xf_wifi_ap_set_cb() 设置的回调获取.
 *
 * @param[in] p_cfg AP 配置.
 *
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 *      - XF_ERR_INITED     已初始化
 *
 * @attention 函数行为约定.
 * - xf_wifi_ap_init() 应在内部完成:
 *   - 判断是否已初始化。
 *     - 已初始化时返回 XF_ERR_INITED.
 *   - 判断参数合法性(含参数内部变量(如 xf_wifi_ap_cfg_t.ssid 长度是否为零))。
 *     - p_cfg 禁止为 NULL.
 *   - 检测内部 netif 组件是否初始化，未初始化则先初始化。
 *     - 不分离 netif 的原因是：
 *       某些系统会在 wifi 组件内部完成对 netif 的注册，不需要显式初始化，
 *       用户使用 netif 时只需 netif_find() 找到对应的网络接口即可。
 *     - 内部实现时需要保存对应 ap 的 netif，以供后续 xf_wifi_ap_get_netif() 使用。
 *   - 配置 wifi 组件为 ap 模式，根据 p_cfg 配置 wifi，后启动 ap 广播。
 *     - 对于不支持的配置，应当以 XF_LOGW() 输出。
 *   - 如果 p_cfg->p_static_ip == NULL 内部自动启用 dhcp 服务器；否则根据配置设置 ip。
 */
xf_err_t xf_wifi_ap_init(const xf_wifi_ap_cfg_t *p_cfg);

/**
 * @brief 反初始化 wifi AP 模式, 并且释放相关资源.
 *
 * @note 调用反初始化之后，从 xf_wifi_ap_init() 获得的 netif_hdl 将无法使用，
 *       需在调用 xf_wifi_ap_deinit() 后手动将 netif_hdl 置为 NULL.
 *
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 *      - XF_ERR_UNINIT     未初始化
 */
xf_err_t xf_wifi_ap_deinit(void);

/**
 * @brief 获取 AP 的 netif 句柄.
 *
 * @param[out] p_netif_hdl 传出对应的 netif 句柄, 通过该句柄可配置 IP 等信息.
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_wifi_ap_get_netif(xf_netif_t *p_netif_hdl);

/**
 * @brief AP 设置 wifi 事件回调函数.
 *
 * @param[in] cb_func 回调函数, 见 @ref xf_wifi_cb_t.
 * @param[in] user_args 调用回调函数时的用户参数.
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 *
 * @note 1. 建议在启用 AP 前设置回调.
 */
xf_err_t xf_wifi_ap_set_cb(xf_wifi_cb_t cb_func, void *user_args);

/**
 * @brief AP 设置 ip 事件回调函数.
 *
 * @param[in] cb_func 回调函数, 见 @ref xf_ip_cb_t.
 * @param[in] user_args 调用回调函数时的用户参数。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 *
 * @note 1. 建议在启用 AP 前设置回调.
 */
xf_err_t xf_wifi_ap_set_ip_cb(xf_ip_cb_t cb_func, void *user_args);

/**
 * @brief 获取 AP 已连接的 STA 列表.
 *
 * @param[out] sta_array STA 信息列表。
 *      sta_array 指向用于存放信息的内存。
 *      sta_array 可以为 NULL(或者 sta_array_size 为 0)，
 *      此时仅查询已连接的 STA 个数。
 * @param sta_array_size STA 信息列表最大容量（单位：个）。可以为 0.
 * @param[out] p_sta_num STA 个数. sta_array 前 num 个有效. 不能为 NULL.
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_wifi_ap_get_sta_list(
    xf_wifi_sta_info_t sta_array[], uint32_t sta_array_size,
    uint32_t *p_sta_num);

/**
 * @brief 断开已连接到 AP 的指定 STA.
 *
 * @param mac 已连接的 STA 的 mac 地址。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_wifi_ap_deauth_sta(const uint8_t mac[]);

/**
 * End of group_xf_wal_wifi_ap
 * @}
 */

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @ingroup group_xf_wal_wifi
 * @defgroup group_xf_wal_wifi_sta sta
 * @brief xf_wifi sta.
 * @endcond
 * @{
 */

/**
 * @brief 启用 wifi STA 模式.
 *
 * @note 注意，此函数只是启用 STA 模式，并**不**连接 AP。
 *       连接 AP 请调用 xf_wifi_sta_connect().
 *
 * @param[in] p_cfg STA 配置（目标 AP 配置）。仅在内部拷贝记录。可以为 NULL.
 * @return xf_err_t
 *      - XF_OK             启用成功或连接成功
 *      - XF_FAIL           失败
 *
 * @attention 函数行为约定.
 * - xf_wifi_sta_init() 应在内部完成:
 *   - 判断是否已初始化。
 *     - 已初始化时返回 XF_ERR_INITED.
 *   - 判断参数合法性。
 *     - p_cfg 允许为 NULL, 此时仅启用站点模式。
 *   - 检测内部 netif 组件是否初始化，未初始化则先初始化。
 *     - 不分离 netif 的原因是：
 *       某些系统会在 wifi 组件内部完成对 netif 的注册，不需要显式初始化，
 *       用户使用 netif 时只需 netif_find() 找到对应的网络接口即可。
 *     - 内部实现时需要保存对应 sta 的 netif，以供后续 xf_wifi_ap_get_netif() 使用。
 *   - 配置 wifi 组件为 sta 模式，如果 p_cfg 不为 NULL 则保存配置。
 *     - 对于不支持的配置，应当以 XF_LOGW() 输出。
 *   - 如果 p_cfg->p_static_ip == NULL 内部自动启用 dhcp 客户端器；否则根据配置设置 ip。
 */
xf_err_t xf_wifi_sta_init(const xf_wifi_sta_cfg_t *p_cfg);

/**
 * @brief 反初始化 wifi STA 模式, 并且释放相关资源.
 *
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_wifi_sta_deinit(void);

/**
 * @brief 获取 STA 的 netif 句柄.
 *
 * @param[out] p_netif_hdl 传出对应的 netif 句柄, 通过该句柄可配置 IP 等信息.
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_wifi_sta_get_netif(xf_netif_t *p_netif_hdl);

/**
 * @brief wifi STA 设置回调函数.
 *
 * @param[in] cb_func 回调函数, 见 @ref xf_wifi_cb_t.
 * @param[in] user_args 调用回调函数时的用户参数.
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 *
 * @note 1. 建议在启用 STA 前设置回调.
 */
xf_err_t xf_wifi_sta_set_cb(xf_wifi_cb_t cb_func, void *user_args);

/**
 * @brief STA 设置 ip 事件回调函数.
 *
 * @param[in] cb_func 回调函数, 见 @ref xf_ip_cb_t.
 * @param[in] user_args 调用回调函数时的用户参数。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 *
 * @note 1. 建议在启用 STA 前设置回调.
 */
xf_err_t xf_wifi_sta_set_ip_cb(xf_ip_cb_t cb_func, void *user_args);

/**
 * @brief STA 连接到指定 AP.
 *
 * @param p_cfg 目标 AP 配置.
 *              如果在调用 xf_wifi_sta_init() 时已经填入，可以填入 NULL 使用之前的配置。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_wifi_sta_connect(xf_wifi_sta_cfg_t *p_cfg);

/**
 * @brief STA 检查是否已经连接到 AP.
 *
 * @return true 已经连接到 AP
 * @return false 未连接到 AP
 */
bool xf_wifi_sta_is_connected(void);

/**
 * @brief STA 主动断开 AP 连接。
 *
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_wifi_sta_disconnect(void);

/**
 * @brief STA 获取当前连接到的 AP 的信息。
 *
 * @param[out] p_info 传出 AP 的信息。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败或未连接
 */
xf_err_t xf_wifi_sta_get_ap_info(xf_wifi_ap_info_t *p_info);

/**
 * @brief wifi 启动 AP 扫描。
 *
 * @note AP 模式下能否扫描取决于具体平台。
 *
 * @param p_cfg wifi 扫描参数。
 * @param block 是否阻塞扫描。
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_wifi_scan_start(const xf_wifi_scan_cfg_t *p_cfg, bool block);

/**
 * @brief wifi 停止 AP 扫描。
 *
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_wifi_scan_stop(void);

/**
 * @brief wifi 获取扫描结果。
 *
 * @note 如果需要读取结果，建议使用足够大的内存，一次读取所有结果。
 *
 * @param[out] result_array 保存结果的数组。
 *      result_array 指向用于存放信息的内存。
 *      p_result_num 可以为 NULL(或者 p_result_num 为 0)，
 *      此时仅查询已连接的 STA 个数。
 * @param result_array_size 保存结果数组的最大容量（单位：个）。可以为 0.
 * @param[out] p_result_num result_array 和 result_array_size 有效时传出实际读出数。
 *                          无效时传出扫描结果个数。不能为 NULL.
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_wifi_scan_get_result(
    xf_wifi_ap_info_t result_array[], uint32_t result_array_size,
    uint32_t *p_result_num);

/**
 * @brief wifi 清除扫描结果。
 *
 * @note 1. 小心使用，不同平台对扫描结果的需求不同。
 *       esp32 上不需要扫描结果也可以连接；ws63 上清除了扫描结果则无法连接。
 * @note 2. 使用 xf_wifi_scan_get_result() 读取完毕后，请及时调用此接口
 *      确保具体 SDK 内部申请的内存得以释放。
 * @note 3. 扫描完毕后，如果不需要连接，或者结果中没有目标 AP，则直接调用此接口清理；
 *      或者有目标 AP，连接后调用此接口清理。
 *
 * @return xf_err_t
 *      - XF_OK             成功
 *      - XF_FAIL           失败
 */
xf_err_t xf_wifi_scan_clear_result(void);

/**
 * End of group_xf_wal_wifi_sta
 * @}
 */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_WIFI_H__ */
