/**
 * @file port_utils.h
 * @author dotc (dotchan@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __PORT_UTILS_H__
#define __PORT_UTILS_H__

/* ==================== [Includes] ========================================== */

#ifdef __cplusplus
extern "C" {
#endif

#include "cJSON.h"
#include "cJSON_Utils.h"

/* ==================== [Defines] =========================================== */

#define CJSON_SET_VAL_WITH_STRUCT(set_method, obj, ptr_struct, member) \
    do { \
        cJSON *ptr = cJSONUtils_GetPointer(obj, "/" #member); \
        set_method(ptr, (ptr_struct)->member); \
    } while (0)

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __PORT_UTILS_H__
