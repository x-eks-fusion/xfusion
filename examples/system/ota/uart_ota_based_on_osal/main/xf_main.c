/**
 * @file xf_main.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2025-01-02
 *
 * Copyright (c) 2025, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "ex_init.h"

#include "xf_ota.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void _example_thread(void *argument);

/* ==================== [Static Variables] ================================== */

static const char *const TAG = "uart_ota";
static xf_osal_thread_t s_thread_hdl = NULL;
#define EX_THREAD_NAME          "ex_thread"
#define EX_THREAD_PRIORITY      XF_OSAL_PRIORITY_NORMOL
#define EX_THREAD_STACK_SIZE    (1024 * 4)
static const xf_osal_thread_attr_t s_thread_attr = {
    .name       = EX_THREAD_NAME,
    .priority   = EX_THREAD_PRIORITY,
    .stack_size = EX_THREAD_STACK_SIZE,
};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    s_thread_hdl = xf_osal_thread_create(_example_thread, NULL, &s_thread_attr);
    if (s_thread_hdl == NULL) {
        XF_LOGE(TAG, "xf_osal_thread_create error");
        return;
    }
}

/* ==================== [Static Functions] ================================== */

static void _example_thread(void *argument)
{
    xf_err_t xf_ret = XF_OK;

    xf_ymodem_t *p_ym               = NULL;

    char file_name[65]              = {0};

    xf_ymodem_file_info_t file_info = {0};
    file_info.p_name_buf            = file_name;
    file_info.buf_size              = ARRAY_SIZE(file_name);
    file_info.file_len              = 0;

    uint8_t *p_buf                  = NULL;
    uint32_t buf_size               = 0;

    ex_init_xf_ymodem();
    xf_sys_watchdog_disable();
    p_ym = ex_get_xf_ymodem();

    xf_ota_partition_info_t update_part_info = {0};
    xf_ota_t ota_cont = {0};
    xf_ota_t *p_ota = &ota_cont;
    xf_ota_partition_t update_partition_cont = {0};
    xf_ota_partition_t *p_update_part = &update_partition_cont;

    xf_ret = xf_ota_init();
    XF_ERROR_CHECK(xf_ret);

    /* 获取下一个需要更新的分区 */
    xf_ret = xf_ota_get_next_update_partition(NULL, p_update_part);
    XF_ERROR_CHECK(xf_ret);
    xf_ret = xf_ota_get_partition_info(p_update_part, &update_part_info);
    XF_ERROR_CHECK(xf_ret);
    XF_LOGI(TAG, "update_part_info.size:        %d", (int)update_part_info.size);
    XF_LOGI(TAG, "update_part_info.unit_size:   %d", (int)update_part_info.unit_size);

    while (1) {
        /* 请求 ota 固件文件 */
        xf_ret = xf_ymodem_recv_handshake(p_ym, &file_info);
        if (xf_ret != XF_OK) {
            xf_osal_delay_ms(50);
            continue;
        }

        XF_LOGI(TAG, "image_name:    %s", file_info.p_name_buf);
        XF_LOGI(TAG, "image_len:     %d", (int)file_info.file_len);

        xf_ret = xf_ota_start(p_update_part, file_info.file_len, true, p_ota);
        XF_ERROR_CHECK(xf_ret);
        XF_LOGI(TAG, "xf_ota_start succeeded");

        /* 接收 ota 固件数据 */
        while (1) {
            xf_ret = xf_ymodem_recv_data(p_ym, &p_buf, &buf_size);
            if (xf_ret != XF_OK) {
                break;
            }
            xf_ret = xf_ota_write(p_ota, p_buf, buf_size);
            XF_ERROR_CHECK(xf_ret);
            XF_LOGI(TAG, "progress: %8d/%d", (int)p_ym->file_len_transmitted, (int)p_ym->file_len);
        }
        if (!((xf_ret == XF_ERR_RESOURCE) && (p_ym->error_code == XF_YMODEM_OK))) {
            /* 发生错误，重新开始接收固件 */
            xf_ota_abort(p_ota);
            continue;
        }

        /* 正常结束，已接受完所有数据 */
        XF_LOGI(TAG, "All received.");
        xf_osal_delay_ms(100);

        /* 结束 ota 流程 */
        xf_ret = xf_ota_end(p_ota);
        XF_ERROR_CHECK(xf_ret);

        /* 更新，并设置下一次开机启动更新流程或在新分区启动 */
        xf_ret = xf_ota_upgrade(p_update_part, false);
        XF_ERROR_CHECK(xf_ret);

        /* 正常结束，已接受完所有数据 */
        XF_LOGI(TAG, "Upgrade complete, restart!");

        /* 重启进入新的分区 */
        xf_sys_reboot();
    }

    xf_ret = xf_ota_deinit();
    XF_ERROR_CHECK(xf_ret);

    xf_osal_thread_delete(NULL);
}
