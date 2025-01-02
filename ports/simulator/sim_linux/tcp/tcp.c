/**
 * @file tcp.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-15
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "uv.h"
#include "tcp.h"
#include <stdlib.h>
#include <string.h>

/* ==================== [Defines] =========================================== */

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000
#define BUFFER_SIZE 1024
#define HEADER_SIZE sizeof(unsigned int) + sizeof(size_t)

/* ==================== [Typedefs] ========================================== */

typedef struct _tcp_msg_t {
    uv_write_t req;
    uv_buf_t buf;
} tcp_msg_t;

/* ==================== [Static Prototypes] ================================= */

static void on_connect(uv_connect_t *req, int status);
static void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);
static void on_write(uv_write_t *req, int status);
static void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);

/* ==================== [Static Variables] ================================== */

static uv_tcp_t s_client;
static uint8_t s_recv_flag = 0;
static uv_buf_t s_recv_buf = {0};
static uv_loop_t *loop = NULL;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void tcp_init(void)
{
    loop = uv_default_loop();  // 获取默认事件循环

    uv_tcp_init(loop, &s_client);           // 初始化 TCP 套接字

    static struct sockaddr_in dest;
    uv_ip4_addr(SERVER_IP, SERVER_PORT, &dest);  // 配置服务器地址

    static uv_connect_t connect_req;
    uv_tcp_connect(&connect_req, &s_client, (const struct sockaddr *)&dest, on_connect);

    // 设置数据读取回调
    uv_read_start((uv_stream_t *)&s_client, alloc_buffer, on_read);

}

void tcp_run(void)
{
    uv_run(loop, UV_RUN_NOWAIT);
}

void tcp_send(unsigned int id, unsigned char *msg, size_t msg_len)
{
    size_t len = HEADER_SIZE + msg_len;
    tcp_msg_t *tcp_msg = (tcp_msg_t *)malloc(sizeof(tcp_msg_t) + len);
    memset(tcp_msg, 0, sizeof(tcp_msg_t) + len);
    unsigned char *buf = (unsigned char *)tcp_msg + sizeof(tcp_msg_t);
    *((unsigned int *)buf) = id;
    *((size_t *)(buf + sizeof(unsigned int))) = msg_len;
    memcpy(buf + HEADER_SIZE, msg, msg_len);
    tcp_msg->buf = uv_buf_init(buf, len);
    uv_write((uv_write_t *)tcp_msg, (uv_stream_t *)&s_client, &tcp_msg->buf, 1, on_write);

}

size_t tcp_get(unsigned int id, unsigned char *msg, size_t msg_len)
{
    size_t len = HEADER_SIZE + 128;
    tcp_msg_t *tcp_msg = (tcp_msg_t *)malloc(sizeof(tcp_msg_t) + len);
    memset(tcp_msg, 0, sizeof(tcp_msg_t) + len);
    unsigned char *buf = (unsigned char *)tcp_msg + sizeof(tcp_msg_t);
    unsigned char *data = buf + HEADER_SIZE;
    sprintf(data, "{\"id\":%d,\"len\":%lu}", id, msg_len);
    *((unsigned int *)buf) = 1;
    *((size_t *)(buf + sizeof(unsigned int))) = strlen(data);
    len = HEADER_SIZE + strlen(data);
    tcp_msg->buf = uv_buf_init(buf, len);
    uv_write((uv_write_t *)tcp_msg, (uv_stream_t *)&s_client, &tcp_msg->buf, 1, on_write);
    s_recv_buf.base = msg;
    while (!s_recv_flag) {
        tcp_run();
    }
    s_recv_flag = 0;
    return s_recv_buf.len;
}

/* ==================== [Static Functions] ================================== */

// 连接完成回调
static void on_connect(uv_connect_t *req, int status)
{
    if (status < 0) {
        fprintf(stderr, "Connection error: %s\n", uv_strerror(status));
        return;
    }
    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);
}

// 读取数据回调
static void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    if (nread > 0) {
        s_recv_flag = 1;
        s_recv_buf.len = *(size_t *)(buf->base + sizeof(unsigned int));
        memcpy(s_recv_buf.base, buf->base + HEADER_SIZE, s_recv_buf.len);
    } else if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "Read error: %s\n", uv_err_name(nread));
        }
        uv_close((uv_handle_t *)stream, NULL);
    }
}

// 分配缓冲区回调
static void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

static void on_write(uv_write_t *req, int status)
{
    tcp_msg_t *write_req = (tcp_msg_t *)req;
    free(write_req);
    write_req = NULL;
}
