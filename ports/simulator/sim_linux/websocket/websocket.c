/**
 * @file websocket.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-11-13
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "websocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <libwebsockets.h>
#include <mqueue.h>
#include <time.h>

/* ==================== [Defines] =========================================== */

#define MSG_SIZE 1024

/* ==================== [Typedefs] ========================================== */

typedef struct _queue_msg_t {
    unsigned char data[MSG_SIZE];
    size_t size;
} queue_msg_t;

/* ==================== [Static Prototypes] ================================= */

static void *websocket_task(void *args);
static int websocket_callback(struct lws *wsi,
                              enum lws_callback_reasons reason,
                              void *user, void *in, size_t len);

/* ==================== [Static Variables] ================================== */

static struct lws *s_wsi = NULL;
static mqd_t s_mq_recv, s_mq_send, s_mq_isr;
static void (*s_isr_callback[2])(unsigned char *msg, size_t len);
static unsigned int s_writeable_flag = 0;
static pthread_mutex_t s_writeable_mutex = PTHREAD_MUTEX_INITIALIZER;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void websocket_init(void)
{

    struct lws_context_creation_info info;
    struct lws_context *context;

    struct mq_attr attr1, attr2, attr3;

    pthread_mutex_init(&s_writeable_mutex, NULL);

    // 设置队列属性
    attr1.mq_flags = 0;
    attr1.mq_maxmsg = 10;
    attr1.mq_msgsize = sizeof(queue_msg_t);
    attr1.mq_curmsgs = 0;
    attr2 = attr1;
    attr3 = attr1;

    // 创建消息队列
    s_mq_recv = mq_open("/recv", O_CREAT | O_RDWR, 0644, &attr1);
    if (s_mq_recv == (mqd_t) -1) {
        perror("mq_open");
        return NULL;
    }
    s_mq_send = mq_open("/send", O_CREAT | O_RDWR | O_NONBLOCK, 0644, &attr2);
    if (s_mq_send == (mqd_t) -1) {
        perror("mq_open");
        return NULL;
    }
    s_mq_isr = mq_open("/isr", O_CREAT | O_RDWR, 0644, &attr3);
    if (s_mq_isr == (mqd_t) -1) {
        perror("mq_open");
        return NULL;
    }

    static struct lws_protocols protocols[] = {
        {
            "xfusion",
            websocket_callback,
            0,
            0,
        },
        { NULL, NULL, 0, 0 } // terminator
    };

    memset(&info, 0, sizeof(info));
    info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;
    context = lws_create_context(&info);

    if (!context) {
        fprintf(stderr, "创建 lws 上下文失败\n");
        return NULL;
    }

    struct lws_client_connect_info i = {0};
    i.context = context;
    i.address = "127.0.0.1";
    i.port = 8000;
    i.path = "/ws/send";
    i.host = i.address;
    i.origin = i.address;
    i.protocol = "xfusion";

    s_wsi = lws_client_connect_via_info(&i);
    if (s_wsi == NULL) {
        lws_context_destroy(context); // 销毁上下文
        return NULL;;
    }

    static pthread_t ws_thread;
    pthread_attr_t pthread_attr;
    pthread_attr_init(&pthread_attr);
    pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&ws_thread, &pthread_attr, websocket_task, context);
    pthread_attr_destroy(&pthread_attr);
}

void websocket_send(unsigned int id, unsigned char *msg, size_t msg_len)
{
    queue_msg_t queue_msg;
    queue_msg.size = sizeof(unsigned int) + sizeof(size_t) + msg_len;
    *((unsigned int *)queue_msg.data) = id;
    *((size_t *)(queue_msg.data + sizeof(unsigned int))) = msg_len;
    memcpy(queue_msg.data + sizeof(unsigned int) + sizeof(size_t), msg, msg_len);
    mq_send(s_mq_send, (char *)&queue_msg, sizeof(queue_msg), 1);

    pthread_mutex_lock(&s_writeable_mutex);
    s_writeable_flag = 1;
    pthread_mutex_unlock(&s_writeable_mutex);
}

size_t websocket_get(unsigned int id, unsigned char *msg, size_t msg_len, unsigned int timeout_ms)
{
    queue_msg_t queue_msg;
    struct timespec timeout;
    char data[128] = {0};
    // 发送get请求
    sprintf(data, "{\"id\":%d,\"len\":%lu}", id, msg_len);
    queue_msg.size = sizeof(unsigned int) + sizeof(size_t) + strlen(data);
    *((unsigned int *)queue_msg.data) = 1;
    *((size_t *)(queue_msg.data + sizeof(unsigned int))) = strlen(data);
    memcpy(queue_msg.data + sizeof(unsigned int) + sizeof(size_t), data, strlen(data));
    mq_send(s_mq_send, (char *)&queue_msg, sizeof(queue_msg), 0);
    // 获取数据
    ssize_t bytes_read = mq_receive(s_mq_recv, (char *)&queue_msg, sizeof(queue_msg), NULL);
    if (bytes_read > 0) {
        memcpy(msg, queue_msg.data, queue_msg.size);
        return queue_msg.size;
    }

    return 0;
}

size_t websocket_isr(unsigned char *msg)
{
    queue_msg_t queue_msg;
    ssize_t bytes_read = mq_receive(s_mq_isr, (char *)&queue_msg, sizeof(queue_msg), NULL);
    if (bytes_read > 0) {
        memcpy(msg, queue_msg.data, queue_msg.size);
        return queue_msg.size;
    }
    return 0;
}

void websocket_gpio_isr_cb(void (*cb)(unsigned char *msg, size_t len))
{
    s_isr_callback[0] = cb;
}

void websocket_tim_isr_cb(void (*cb)(unsigned char *msg, size_t len))
{
    s_isr_callback[1] = cb;
}

/* ==================== [Static Functions] ================================== */

static void *websocket_task(void *args)
{

    struct lws_context *context = args;

    while (1) {
        lws_service(context, 0);
        pthread_mutex_lock(&s_writeable_mutex);
        if (s_writeable_flag == 1)
        {
            s_writeable_flag = 0;
            pthread_mutex_unlock(&s_writeable_mutex);
            lws_callback_on_writable(s_wsi);
        }
        else
        {
            pthread_mutex_unlock(&s_writeable_mutex);
        }
        // lws_callback_on_writable(s_wsi);
    }
    return NULL;
}

static int websocket_callback(struct lws *wsi,
                              enum lws_callback_reasons reason,
                              void *user, void *in, size_t len)
{
    switch (reason) {
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        lws_callback_on_writable(s_wsi);
        break;

    case LWS_CALLBACK_CLIENT_WRITEABLE: {
        queue_msg_t msg;
        ssize_t bytes_read = mq_receive(s_mq_send, (char *)&msg, sizeof(msg), NULL);
        while (bytes_read > 0) {
            lws_write(s_wsi, (unsigned char *)msg.data, msg.size, LWS_WRITE_BINARY);
            bytes_read = mq_receive(s_mq_send, (char *)&msg, sizeof(msg), NULL);
        }
        break;
    }

    case LWS_CALLBACK_CLIENT_RECEIVE: {
        queue_msg_t queue_msg;
        unsigned int id = *(unsigned int *)in;
        memcpy(queue_msg.data, in, len);
        queue_msg.size = len;

        if (id == 2) {
            unsigned int device_id = id >> 24;
            if (device_id == 1) {
                // 执行gpio中断
                s_isr_callback[0](in, len);
            } else if (device_id == 3) {
                // 执行tim中断
                s_isr_callback[1](in, len);
            }
            mq_send(s_mq_isr, (char *)&queue_msg, sizeof(queue_msg), 0);
        } else {
            mq_send(s_mq_recv, (char *)&queue_msg, sizeof(queue_msg), 0);
        }
        break;
    }

    case LWS_CALLBACK_CLIENT_CLOSED:
        break;

    default:
        break;
    }
    return 0;
}


