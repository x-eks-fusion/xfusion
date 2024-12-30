/**
 * @file xf_sle_port_utils.c
 * @author dotc (dotchan@qq.com)
 * @brief 主要为 SLE 对接时辅助的一些方法，可简化对接处理。
 * @version 0.1
 * @date 2024-12-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/* ==================== [Includes] ========================================== */

#include "xf_sle_port_utils.h"
#include "xf_utils.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

uint8_t xf_sle_adv_data_packed_size_get(xf_sle_adv_struct_t *adv_struct_set)
{
    if(adv_struct_set == NULL)
    {
        return 0;
    }
    uint8_t adv_struct_cnt = 0;
    uint8_t data_packed_size = 0;
    while (adv_struct_set[adv_struct_cnt].ad_data_len != 0) {
        data_packed_size += XF_SLE_ADV_STRUCT_LEN_FILED_SIZE
                             + XF_SLE_ADV_STRUCT_TYPE_FILED_SIZE
                             + adv_struct_set[adv_struct_cnt].ad_data_len;
        ++adv_struct_cnt;
    }
    return data_packed_size;
}

xf_err_t xf_sle_adv_data_packed_by_adv_struct_set(
    uint8_t *adv_data_buf, xf_sle_adv_struct_t *adv_struct_set)
{
    /* 遍历将各个 ad structure 顺序放入 */
    XF_SLE_ADV_STRUCT_TYPE_ARRAY_FIXED(std_adv_struct_t);
    std_adv_struct_t adv_struct;
    uint8_t *ptr_current = adv_data_buf;
    uint8_t cnt = 0;
    while (adv_struct_set[cnt].ad_data_len != 0) {
        uint8_t adv_struct_size = 0;
        

        xf_memset(&adv_struct, 0, sizeof(std_adv_struct_t));

        // 获取整个 adv_struct 实际大小 (只包含有效数据部分)
        adv_struct_size = adv_struct_set[cnt].ad_data_len 
            + XF_SLE_ADV_STRUCT_LEN_FILED_SIZE + XF_SLE_ADV_STRUCT_TYPE_FILED_SIZE;
        // > 设置 ad_data_len
        adv_struct.ad_data_len = adv_struct_set[cnt].ad_data_len;
        // > 设置 ad_type
        adv_struct.ad_type = adv_struct_set[cnt].ad_type;

        if(adv_struct_set[cnt].is_ptr == true)
        {
            // > 设置 ad_data (值在 ad_data 指向的内存中 (指针变量))
            xf_memcpy(adv_struct.ad_data, adv_struct_set[cnt].ad_data.adv_var.ptr_u8,
                    adv_struct_set[cnt].ad_data_len);
        }
        else
        {
            // > 设置 ad_data (值在 ad_data 中 (值变量))
            xf_memcpy(adv_struct.ad_data, &adv_struct_set[cnt].ad_data.adv_var.val_u32,
                   adv_struct_set[cnt].ad_data_len);
        }

        /* 将临时构造的 adv_struct 整个复制至 data_packed_adv 对应位置 */
        xf_memcpy(ptr_current, &adv_struct, adv_struct_size);

        /* data_packed_adv 写入位置更新 */
        ptr_current += adv_struct_size;
        ++cnt;
    }
    return XF_OK;
}

/* ==================== [Static Functions] ================================== */
