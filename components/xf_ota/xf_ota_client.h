/**
 * @file xf_ota_client.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-12-09
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_OTA_CLIENT_H__
#define __XF_OTA_CLIENT_H__

/* ==================== [Includes] ========================================== */

#include "xf_ota_types.h"

#if XF_OTA_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 初始化 xf_ota 模块。
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 */
xf_err_t xf_ota_init(void);

/**
 * @brief 反初始化 xf_ota 模块。
 *
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 */
xf_err_t xf_ota_deinit(void);

/**
 * @brief 获取当前程序运行分区信息。
 *
 * @param[out] p_part_hdl       传出当前运行分区句柄信息。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败
 *      - XF_ERR_INVALID_ARG    无效参数
 *
 * @code{c}
 * xf_ota_partition_t running_partition = {0};
 * xf_ota_get_running_partition(&running_partition);
 * @endcode
 */
xf_err_t xf_ota_get_running_partition(xf_ota_partition_t *p_part_hdl);

/**
 * @brief 获取下一个固件分区。
 *
 * @param p_start_part_hdl      传入起始的分区，可以为 NULL.
 * @param[out] p_next_part_hdl  传出下一个可用于固件升级的分区句柄。
 *                              禁止与 p_start_part_hdl 相同。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_FAIL               失败，或者没有下一个升级分区了
 *      - XF_ERR_INVALID_ARG    无效参数
 *
 * @code{c}
 * xf_ota_partition_t running_partition = {0};
 * xf_ota_partition_t update_partition = {0};
 * xf_ota_get_running_partition(&running_partition);
 * xf_err_t xf_ret = xf_ota_get_next_update_partition(&running_partition, &update_partition);
 * // 或 `xf_ota_get_next_update_partition(NULL, &update_partition);`
 * if (xf_ret == XF_OK) {
 *     // 可以用 update_partition 进行后续升级
 * }
 * @endcode
 */
xf_err_t xf_ota_get_next_update_partition(
    xf_ota_partition_t *p_start_part_hdl, xf_ota_partition_t *p_next_part_hdl);

/**
 * @brief 获取分区信息。
 *
 * @param p_part_hdl            需要查询的分区.
 *                              可以从 xf_ota_get_running_partition()
 *                              或 xf_ota_get_next_update_partition() 获取。
 * @param[out] p_info           传出分区信息。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_ERR_NOT_SUPPORTED  传入的分区不支持查询；或者接口未实现
 *      - XF_FAIL               失败
 */
xf_err_t xf_ota_get_partition_info(
    const xf_ota_partition_t *p_part_hdl, xf_ota_partition_info_t *p_info);

/**
 * @brief 获取平台 APP 描述块的大小。
 *
 * @return size_t 平台 APP 描述块的大小。
 */
size_t xf_ota_get_platform_app_desc_size(void);

/**
 * @brief 获取平台 APP 描述块。
 *
 * @note 不同平台的 APP 描述及所含信息不同, xfusion 无法兼容。
 *       获取描述块主要用来发给固件服务器。
 * @note APP 描述块可能有编译时间日期、版本号、SDK 版本号等信息，由具体平台决定。
 *
 * @param p_part_hdl            需要查询的分区。
 *                              可以从 xf_ota_get_running_partition()
 *                              或 xf_ota_get_next_update_partition() 获取。
 * @param[out] p_desc_out       描述块缓冲区。
 * @param buff_size             缓冲区大小，单位字节。可以从
 *                              xf_ota_get_platform_app_desc_size() 获取。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_ERR_NOT_SUPPORTED  传入的分区不支持查询；或者接口未实现
 *      - XF_FAIL               失败
 *      - XF_ERR_INVALID_ARG    参数无效，或者 p_part_hdl 内不含可读取的固件信息
 */
xf_err_t xf_ota_get_platform_app_desc_block(
    xf_ota_partition_t *p_part_hdl, void *p_desc_out, size_t buff_size);

/**
 * @brief 获取平台 APP 摘要块的大小。
 *
 * @return size_t 平台 APP 摘要块的大小。
 */
size_t xf_ota_get_platform_app_digest_size(void);

/**
 * @brief 获取平台 APP 摘要块。
 *
 * @note 不同平台的 APP 摘要算法不同，输出格式也不一定相同, xfusion 无法兼容。
 *       获取摘要块主要用来发给固件服务器。
 * @note APP 摘要块可能是 SHA256 或 MD5 的计算结果，由具体平台决定。
 *
 * @param p_part_hdl            需要查询的分区。
 *                              可以从 xf_ota_get_running_partition()
 *                              或 xf_ota_get_next_update_partition() 获取。
 * @param[out] p_digest_out     摘要块缓冲区。
 * @param buff_size             缓冲区大小，单位字节。可以从
 *                              xf_ota_get_platform_app_digest_size() 获取。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_ERR_NOT_SUPPORTED  传入的分区不支持查询；或者接口未实现
 *      - XF_FAIL               失败
 *      - XF_ERR_INVALID_ARG    参数无效，或者 p_part_hdl 内不含可读取的固件信息
 */
xf_err_t xf_ota_get_platform_app_digest_block(
    xf_ota_partition_t *p_part_hdl, void *p_digest_out, size_t buff_size);

/**
 * @brief 开始 OTA 固件写入流程。
 *
 * @param p_part_hdl            需要升级的分区。
 *                              可以从 xf_ota_get_next_update_partition() 获取。
 * @param package_len           固件包长。不同平台对此参数要求不同，通常需要填入此信息。
 *                              如果确认平台支持不指定固件包长，可填入 @ref XF_OTA_PACKAGE_LEN_UNKNOWN.
 * @param sequential_write      是否顺序写入。不同平台对能否随机写入的支持程度不同。
 *                              顺序写入是最低要求。
 * @param[out] p_hdl            传出 OTA 句柄。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_ERR_NOT_SUPPORTED  不支持随机写入
 *      - XF_FAIL               失败
 *
 * @code{c}
 * xf_ota_partition_t update_partition = {0};
 * xf_ota_get_next_update_partition(NULL, &update_partition);`
 * xf_ota_t ota = {0};
 * xf_ret = xf_ota_start(&update_partition, PACKAGE_LEN, true, &ota);
 * if (xf_ret != XF_OK) {
 *     // 错误处理
 * }
 * while (1) {
 *     // 收包到 p_buf, 并且更新数据包大小 buf_size
 *     xf_ret = xf_ota_write(&ota, p_buf, buf_size);    XF_ERROR_CHECK(xf_ret);
 *     // 收包完毕后 break;
 * }
 * // 结束 OTA 写入流程
 * xf_ret = xf_ota_end(p_ota);                          XF_ERROR_CHECK(xf_ret);
 * // 更新，并设置下一次开机启动更新流程，或在新分区启动
 * xf_ret = xf_ota_upgrade(p_update_part, false);       XF_ERROR_CHECK(xf_ret);
 * @endcode
 */
xf_err_t xf_ota_start(
    xf_ota_partition_t *p_part_hdl,
    uint32_t package_len, bool sequential_write, xf_ota_t *p_hdl);

/**
 * @brief 中断 OTA 固件写入流程。
 *
 * @param p_hdl                 由 xf_ota_start() 传出的 OTA 句柄。
 *                              调用 xf_ota_abort() 后句柄内容无效。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_ERR_NOT_SUPPORTED  不支持非顺序写入
 *      - XF_FAIL               失败
 */
xf_err_t xf_ota_abort(xf_ota_t *p_hdl);

/**
 * @brief 终止 OTA 固件写入流程。
 *
 * @note 可能会此函数调用后校验写入的固件数据。
 *
 * @param p_hdl                 由 xf_ota_start() 传出的 OTA 句柄。
 *                              调用 xf_ota_end() 后句柄内容无效。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_ERR_INVALID_CHECK  写入的固件无效
 *      - XF_FAIL               其他错误情况
 */
xf_err_t xf_ota_end(xf_ota_t *p_hdl);

/**
 * @brief 更新固件。
 *
 * @param p_part_hdl            需要升级的分区。
 *                              从 xf_ota_get_next_update_partition() 获取，并且程序写入完毕。
 * @param reboot                如果内部检测成功，是否重启。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_FAIL               失败
 */
xf_err_t xf_ota_upgrade(xf_ota_partition_t *p_part_hdl, bool reboot);

/**
 * @brief 顺序写入 OTA 固件。
 *
 * @note 不能与 xf_ota_write_to() 一起使用。
 *
 * @param p_hdl                 OTA 句柄，从 xf_ota_start() 获取。
 * @param src                   OTA 固件数据。
 * @param size                  本次 OTA 固件数据大小。
 *                              必须以 xf_ota_partition_info_t.unit_size 为单位写入。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_FAIL               失败
 */
xf_err_t xf_ota_write(xf_ota_t *p_hdl, const void *src, size_t size);

/**
 * @brief 指定偏移地址写入 OTA 固件。
 *
 * @note xf_ota_write_to() 不一定所有平台都支持。
 *
 * @param p_hdl                 OTA 句柄，从 xf_ota_start() 获取。
 * @param dst_offset            偏移量，单位字节。
 * @param src                   OTA 固件数据。
 * @param size                  本次 OTA 固件数据大小。
 *                              必须以 xf_ota_partition_info_t.unit_size 为单位写入。
 * @return xf_err_t
 *      - XF_OK                 成功
 *      - XF_ERR_INVALID_ARG    无效参数
 *      - XF_FAIL               失败
 */
xf_err_t xf_ota_write_to(
    xf_ota_t *p_hdl, size_t dst_offset, const void *src, size_t size);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XF_OTA_IS_ENABLE */

#endif /* __XF_OTA_CLIENT_H__ */
