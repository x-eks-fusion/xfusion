/**
 * @file xf_main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-23
 *
 * Copyright (c) 2023, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"

/* ==================== [Defines] =========================================== */

#define TAG                 "log"
#define LOG_ID_DEFAULT      0     /*!< 默认（第一个） log 后端 */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    const char *name = "skldfjaslkdfjaslkdfj;asldfja;sldfjasljdflksjdfl;kaj;dlfja;lskdjf;alskjdfaljlasdflj;";
    int date = 20241014;
    float pi = 3.141592;

    xf_log_set_info_level(LOG_ID_DEFAULT, XF_LOG_LVL_VERBOSE);  /*!< 所有等级打印都带有全部信息 */
    xf_log_set_filter_file(LOG_ID_DEFAULT, "file1.c");          /*!< 过滤文件名为 "file1.c" 的打印 */
    xf_log_set_filter_enable(LOG_ID_DEFAULT);                   /*!< 打开过滤器 */
    xf_log_set_filter_is_blacklist(LOG_ID_DEFAULT);             /*!< 设置过滤器为黑名单 */

    xf_log_set_filter_colorful_disable(LOG_ID_DEFAULT);         /*!< 不用彩色打印 */
    xf_log(XF_LOG_LVL_USER, TAG, __FILE__, __LINE__, __func__, "Hello, %.5s, date: %d, pi: %f!\n", name, date, pi);
    xf_log(XF_LOG_LVL_ERROR, TAG, "file1.c", __LINE__, __func__, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    xf_log(XF_LOG_LVL_WARN, TAG, __FILE__, __LINE__, __func__, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    xf_log(XF_LOG_LVL_INFO, TAG, "file2.c", __LINE__, __func__, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    xf_log(XF_LOG_LVL_DEBUG, TAG, __FILE__, __LINE__, __func__, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    xf_log(XF_LOG_LVL_VERBOSE, TAG, "file3.c", __LINE__, __func__, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);

    xf_log_set_filter_colorful_enable(LOG_ID_DEFAULT);          /*!< 启用彩色打印 */
    xf_log_level(XF_LOG_LVL_USER, TAG, "Hello, %.5s, date: %d, pi: %f!", name, date, pi);
    xf_log_level(XF_LOG_LVL_ERROR, TAG, "Hello, %s, date: %d, pi: %f!", name, date, pi);
    xf_log_level(XF_LOG_LVL_WARN, TAG, "Hello, %s, date: %d, pi: %f!", name, date, pi);
    xf_log_level(XF_LOG_LVL_INFO, TAG, "Hello, %s, date: %d, pi: %f!", name, date, pi);
    xf_log_level(XF_LOG_LVL_DEBUG, TAG, "Hello, %s, date: %d, pi: %f!", name, date, pi);
    xf_log_level(XF_LOG_LVL_VERBOSE, TAG, "Hello, %s, date: %d, pi: %f!", name, date, pi);

    XF_LOGU(TAG, "Hello, %s, date: %d, pi: %f!", name, date, pi);
    XF_LOGE(TAG, "Hello, %s, date: %d, pi: %f!", name, date, pi);
    XF_LOGW(TAG, "Hello, %s, date: %d, pi: %f!", name, date, pi);
    XF_LOGI(TAG, "Hello, %s, date: %d, pi: %f!", name, date, pi);
    XF_LOGD(TAG, "Hello, %s, date: %d, pi: %f!", name, date, pi);
    XF_LOGV(TAG, "Hello, %s, date: %d, pi: %f!", name, date, pi);

    xf_log_printf("Hello, %s, date: %d, pi: %f!\n", name, date, pi);
}

/* ==================== [Static Functions] ================================== */
