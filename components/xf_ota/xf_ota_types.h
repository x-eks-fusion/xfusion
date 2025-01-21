/**
 * @file xf_ota_types.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-12-19
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_OTA_TYPES_H__
#define __XF_OTA_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_ota_config_internal.h"
#include "xf_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_iperf
 * @endcond
 * @{
 */

/* ==================== [Defines] =========================================== */

#define XF_OTA_PACKAGE_LEN_UNKNOWN      0xffffffff /*!< 如果固件大小未知 */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief xf_ota 对象容器类型。
 * @note 用户禁止访问 xf_ota_t.platform_data, 它供给底层使用，不一定是指针。
 * @note 用户禁止修改其中内容。
 * @note 此结构体未来可能扩充。
 */
typedef struct _xf_ota_t {
    /* private: */
    void *platform_data; /*!< 平台私有数据 */
} xf_ota_t;

/**
 * @brief xf_ota 分区枚举代码。
 * @note 不一定所有平台都支持此类型。需要根据具体对接确定。
 *       如 1. 只支持双 OTA 分区，或者 2. 只有一个固件分区，只支持差分升级。
 */
typedef enum xf_ota_partition_id_code_e {
    XF_OTA_PARTITION_ID_INVALID     = 0x00,
    XF_OTA_PARTITION_ID_PACKAGE_STORAGE,    /*!< 区别于 OTA 分区存储固件，此分区只存储升级包 */
    XF_OTA_PARTITION_ID_OTA_0       = 0x10,
    XF_OTA_PARTITION_ID_OTA_1,
    XF_OTA_PARTITION_ID_OTA_2,
    XF_OTA_PARTITION_ID_OTA_3,
    XF_OTA_PARTITION_ID_OTA_4,
    XF_OTA_PARTITION_ID_OTA_5,
    XF_OTA_PARTITION_ID_OTA_6,
    XF_OTA_PARTITION_ID_OTA_8,
    XF_OTA_PARTITION_ID_OTA_9,
    XF_OTA_PARTITION_ID_OTA_10,
    XF_OTA_PARTITION_ID_OTA_11,
    XF_OTA_PARTITION_ID_OTA_12,
    XF_OTA_PARTITION_ID_OTA_13,
    XF_OTA_PARTITION_ID_OTA_14,
    XF_OTA_PARTITION_ID_OTA_15,
    XF_OTA_PARTITION_ID_MAX,
} xf_ota_partition_id_code_t;

/**
 * @brief xf_ota 分区类型。
 */
typedef uint8_t xf_ota_partition_id_t;

/**
 * @brief xf_ota 分区对象容器类型。
 * @note 用户禁止访问 xf_ota_t.platform_data, 它提供给底层使用，不一定是指针。
 * @note 用户禁止修改其中内容。
 * @note 此结构体未来可能扩充。
 */
typedef struct _xf_ota_partition_t {
    /* private: */
    void *platform_data;    /*!< 私有平台数据，禁止访问 */
    bool is_xf_fal_part;    /*!< 表示 xf_ota_partition_t.platform_data
                             *   是否是 xf_fal_part 分区。如果是，则可以
                             *   强转为 xf_fal_partition_t 读取信息。
                             */
    xf_ota_partition_id_t partition_id; /*!< 分区 id, 指示第几个固件分区，不一定全平台支持 */
} xf_ota_partition_t;

/**
 * @brief xf_ota 分区信息类型。
 * @note 用户禁止修改其中内容。
 */
typedef struct _xf_ota_partition_info_t {
    size_t  unit_size;      /*!< 最小写入大小，单位字节。通过
                             *   xf_ota_write() 写入固件数据时必须以此单位写入。
                             */
    size_t  size;           /*!< 分区的容量，单位字节。 */
} xf_ota_partition_info_t;

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

/**
 * End of addtogroup group_xf_iperf
 * @}
 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __XF_OTA_TYPES_H__ */
