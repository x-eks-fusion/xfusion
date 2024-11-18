/**
 * @file tcp.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-15
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __TCP_H__
#define __TCP_H__

/* ==================== [Includes] ========================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

void tcp_init(void);
void tcp_run(void);
void tcp_send(unsigned int id, unsigned char *msg, size_t msg_len);
size_t tcp_get(unsigned int id, unsigned char *msg, size_t msg_len);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __TCP_H__
