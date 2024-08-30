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

#define MAX_WORKERS 3
#define TAG "task_pool"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void ntask(xf_task_t task);

/* ==================== [Static Variables] ================================== */

static xf_task_pool_t npool = NULL;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    npool = xf_ntask_pool_create(MAX_WORKERS, 1000, 1);
    xf_task_init_from_pool(npool, ntask, (void *)1, 1);
}

/* ==================== [Static Functions] ================================== */

static void ntask(xf_task_t task)
{
    static int num = 0;
    num++;
    // 不断的创建任务（由于任务池的存在，所以不会申请内存，反而会回收已经使用完的任务）
    XF_LOGI(TAG, "ntask:%d", num);
    if (num < 5)
    {
        xf_task_init_from_pool(npool, ntask, (void *)1, 1);
    }
    else
    {
        xf_task_pool_delete(npool);
        XF_LOGI(TAG, "delete npool");
    }

}
