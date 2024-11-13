/**
 * @file websocket.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-11
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "websocket.h"
#include <libwebsockets.h>
#include "port_common.h"

/* ==================== [Defines] =========================================== */

#define BUFFER_LEN  1024

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static int websocket_callback(struct lws *wsi,
                              enum lws_callback_reasons reason,
                              void *user, void *in, size_t len);

static struct lws *s_wsi = NULL;
static struct lws_context *s_context = NULL;
static unsigned char s_is_start = 0;
static void *s_recv_data = NULL;
static size_t s_recv_len = 0;
static unsigned char s_is_recv = 0;

/* ==================== [Static Variables] ================================== */

static struct lws_protocols protocols[] = {
    {
        "xfusion",
        websocket_callback,
        0,
        0,
    },
    { NULL, NULL, 0, 0 } // terminator
};

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void websocket_init(void)
{
    struct lws_context_creation_info info;

    memset(&info, 0, sizeof(info));
    info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;
    s_context = lws_create_context(&info);

    if (!s_context) {
        fprintf(stderr, "创建 lws 上下文失败\n");
        return;
    }

    struct lws_client_connect_info i = {0};
    i.context = s_context;
    i.address = "127.0.0.1";
    i.port = 8000;
    i.path = "/ws/send";
    i.host = i.address;
    i.origin = i.address;
    i.protocol = "xfusion";

    s_wsi = lws_client_connect_via_info(&i);
    if (s_wsi == NULL) {
        printf("连接失败\n");
        lws_context_destroy(s_context); // 销毁上下文
        return;
    }

}

void websocket_wait_start(void)
{
    while (!s_is_start) {
        lws_service(s_context, 0);
    }

}

int websocket_service(void)
{
    return lws_service(s_context, 0);
}

void websocket_send(unsigned int id, unsigned char *msg, size_t msg_len)
{
    size_t len = sizeof(unsigned int) + sizeof(size_t) + msg_len;
    unsigned char buffer[BUFFER_LEN] = {0};
    *((unsigned int *)buffer) = id;
    *((size_t *)(buffer + sizeof(unsigned int))) = msg_len;
    memcpy(buffer + sizeof(unsigned int) + sizeof(size_t), msg, msg_len);
    lws_write(s_wsi, (unsigned char *)buffer, len, LWS_WRITE_BINARY);
}

size_t websocket_recv(void *in)
{
    while (s_is_recv == 0) {
        lws_service(s_context, 0);
    }
    memcpy(in, s_recv_data, s_recv_len);
    s_is_recv = 1;
    return s_recv_len;
}

/* ==================== [Static Functions] ================================== */


static int websocket_callback(struct lws *wsi,
                              enum lws_callback_reasons reason,
                              void *user, void *in, size_t len)
{
    switch (reason) {
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        s_is_start = 1;
        break;

    case LWS_CALLBACK_CLIENT_RECEIVE:
        s_is_recv = 1;
        s_recv_data = in;
        s_recv_len = len;
        break;

    case LWS_CALLBACK_CLIENT_CLOSED:
        break;

    default:
        break;
    }
    return 0;
}
