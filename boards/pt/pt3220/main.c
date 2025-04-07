/**
 * @file main.c
 * @brief 平台侧应用主函数入口。
 * @author lijianpeng (bosco@pthyidh.com)
 * @date 2024-12-08 03:27:37
 *
 * @copyright Copyright (c) 2024 by 深圳市鹏天微智能科技有限公司, All Rights Reserved.
 */

#include "xfusion.h"

#if defined(CONFIG_XF_BLE_ENABLE)
#include "bledef.h"
#endif
#include "port_common.h"

int main(void)
{
    sys_pre_init();
    xfusion_init();

    while (1)
    {
        #if defined(CONFIG_XF_BLE_ENABLE)
        ble_schedule();
        #endif

        xfusion_run();
    }
}
