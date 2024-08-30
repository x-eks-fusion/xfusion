/**
 * @file xf_main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-23
 *
 * Copyright (c) 2023, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define TOPIC_ID 11
#define TAG "example_mbus"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void task_pub(xf_task_t task);
static void mbus_handle(xf_task_t task);
static void bus_cb(const void *const data, void *user_data);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    // 创建协作式任务
    xf_ntask_create_loop(task_pub, (void *)1, 1, 2000);
    // 绑定topic到任务管理器上
    xf_task_mbus_reg_topic(TOPIC_ID, sizeof(int));
    // 订阅这个topic，设置处理topic的回调
    xf_task_mbus_sub(TOPIC_ID, bus_cb, NULL);
}

/* ==================== [Static Functions] ================================== */

/**
 * @brief 用于测试发布的ntask，定时发布一些数据
 *
 * @param task 任务对象
 */
static void task_pub(xf_task_t task)
{
    int num = 1;
    XF_LOGI(TAG, "task:%d", num);
    // 异步发送（慢，但是不会阻塞本任务）
    xf_task_mbus_pub_async(TOPIC_ID, &num);
    num++;
    // 同步发送（快，但是会阻塞本任务）
    xf_task_mbus_pub_sync(TOPIC_ID, &num);
}

static void mbus_handle(xf_task_t task)
{
    xf_task_mbus_handle();
}

/**
 * @brief 订阅回调函数，用于处理用户发布的消息
 *
 * @param data 用户发布的消息
 * @param user_data 创建订阅时候的用户自定义参数
 */
static void bus_cb(const void *const data, void *user_data)
{
    int num = *(int *)data;
    XF_LOGI(TAG, "data:%d", num);
}
