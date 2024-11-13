/**
 * @file websocket.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-11
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __WEBSOCKET_H__
#define __WEBSOCKET_H__

/* ==================== [Includes] ========================================== */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

void websocket_init(void);
void websocket_wait_start(void);
int websocket_service(void);
void websocket_send(unsigned int id, unsigned char *msg, size_t msg_len);
size_t websocket_recv(void *in);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __WEBSOCKET_H__
