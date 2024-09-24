/**
 * @file port_common.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief 
 * @version 1.0
 * @date 2024-09-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/* ==================== [Includes] ========================================== */

#include "port_common.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_err_t port_convert_pf2xf_err(pf_err_t pf_err)
{
    switch (pf_err) {
    default:
    case ESP_FAIL:                  return XF_FAIL;                 break;
    case ESP_OK:                    return XF_OK;                   break;
    case ESP_ERR_NO_MEM:            return XF_ERR_NO_MEM;           break;
    case ESP_ERR_INVALID_ARG:       return XF_ERR_INVALID_ARG;      break;
    case ESP_ERR_INVALID_STATE:     return XF_ERR_INVALID_STATE;    break;
    case ESP_ERR_NOT_FOUND:         return XF_ERR_NOT_FOUND;        break;
    case ESP_ERR_NOT_SUPPORTED:     return XF_ERR_NOT_SUPPORTED;    break;
    case ESP_ERR_WIFI_NOT_INIT:     return XF_ERR_UNINIT;           break;
    }
}

/* ==================== [Static Functions] ================================== */
