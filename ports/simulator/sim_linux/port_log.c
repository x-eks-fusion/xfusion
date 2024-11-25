/**
 * @file port_log.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-09
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include <stdio.h>
#include "xf_init.h"
#include "xf_log.h"
#include "cJSON.h"
#include "cJSON_Utils.h"
#include "port_utils.h"
#include "port_common.h"
#include "tcp.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

typedef struct _port_log_t {
    uint32_t id;
    cJSON *json;
} port_log_t;

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

static port_log_t s_port_log = {0};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

/* ==================== [Static Functions] ================================== */

static void xf_log_out(const char *str, size_t len, void *arg)
{
    if ((NULL == str) || (0 == len)) {
        return;
    }

    tcp_send(s_port_log.id, (unsigned char *)str, len);
}

static int port_log_init(void)
{
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        return XF_ERR_NO_MEM;
    }
    s_port_log.id = ID_WITH_TYPE_AND_INDEX(XF_HAL_UART_TYPE, 0);
    s_port_log.json = json;
    cJSON_AddNumberToObject(json, "id", s_port_log.id);

    char *json_str = cJSON_PrintUnformatted(json);

    tcp_send(XF_HAL_CONFIG_ID, json_str, strlen(json_str));

    xf_log_register_obj(xf_log_out, NULL);
    return 0;
}
XF_INIT_EXPORT_SETUP(port_log_init);
