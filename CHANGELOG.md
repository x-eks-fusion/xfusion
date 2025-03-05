<!--
    更新本文档时请注意，新版本号及描述放到最前面。
 -->

# Changelog

本文记录 XFusion 当前版本的显著更改。

## [1.3.0]

1.  `components/xf_fal`

    flash 抽象层。

    1.  新增
        1.  支持注册与注销分区表及 flash 设备。
        1.  esp32 对接。
        1.  sim_linux 对接示例可见 [port_xf_fal_by_memory](examples/storage/vfs/vfs_littlefs/components/sim_linux_ext_port/port_xf_fal_by_memory.c)。
        1.  添加 xf_fal 对接 littlefs 示例 [fal_littlefs](examples/storage/fal/fal_littlefs).
        1.  添加 fal_flashdb 示例 [fal_flashdb](examples/storage/fal/fal_flashdb).

1.  `components/xf_heap`

    动态内存。

    1.  新增
        1.  支持重定义 malloc、free 等函数实现。

1.  `components/xf_task`

    协作式调度器。

    1.  修改
        1.  原本定时任务由 ntask 更名为 ttask。
        1.  无栈协程仍然使用 ntask 名字。
        1.  重构了 mbus 和 task_pool。
    1.  新增
        1.  支持 async 和 await，支持无栈协程嵌套。

1.  `components/xf_ota`

    ota 接口。

    1.  新增
        1.  首次新增。
        1.  已添加 ws63，esp32 对接。
        1.  示例见 [uart_ota_based_on_osal](examples/system/ota/uart_ota_based_on_osal)。

1.  `components/xf_vfs`

    虚拟文件系统。

    1.  新增
        1.  首次新增。
        1.  已添加 esp32 对接。
        1.  示例见 [vfs_littlefs](examples/storage/vfs/vfs_littlefs)。

1.  `boards/simulator/sim_linux`

    linux 模拟器。

    1.  新增
        1.  首次新增，可使用 `. ./export.sh sim_linux` 激活 linux 模拟器；或使用 `. ./export.sh <其他目标>` 后使用 `xf sim` 命令一键激活、编译、烧录。

1.  `boards/espressif/esp32`

    esp32 平台。

    1.  修改
        1.  更新分区表，默认使用 2 ota + 1 存储分区。

## [1.2.0]

1.  `components/xf_fal`

    flash 抽象层。

    1.  新增
        1.  首次更新。
        1.  暂未对接到具体平台，仅 linux 中模拟测试。

1.  `components/xf_hal`

    硬件抽象层。

    1.  新增
        1.  gpio 初始化后支持设置方向([#2](https://github.com/x-eks-fusion/xf_hal/pull/2))。

1.  `components/xf_init`

    自动初始化。

    1.  修改
        1.  移除预初始化，添加 xfusion 内部专用的 `SETUP` 和 `CLEANUP` 等级代替原本预初始化的功能。
        1.  修改 `xfusion_run()` 的调用方式为外部循环调用，见 `boards/espressif/esp32/main/main.c`。
        1.  详见 ([#1](https://github.com/x-eks-fusion/xf_init/pull/1))。

1.  `components/xf_log`

    系统日志。

    1.  修改
        1.  移除内置 `printf` 实现，可通过宏自定义 `vsnprintf` 实现位置。
    1.  新增
        1.  示例见 `examples/system/log`.
        1.  注册多 log 后端。
        1.  过滤器。
        1.  颜色开关。
        1.  输出信息等级。
        1.  使用 log 后端的 printf.
    1.  备注
        1.  详见 ([#2](https://github.com/x-eks-fusion/xf_log/pull/2)).

1.  `components/xf_nal`

    网络抽象层。目前主要用于设置 DHCP、IP、DNS。

    1.  新增
        1.  首次更新。
        1.  示例见 `examples/wireless/wifi/static_ip`.
        1.  含 `xf_netif`.
    1.  备注
        1.  详见 ([#8](https://github.com/x-eks-fusion/xfusion/pull/8)).

1.  `components/xf_net_apps`

    网络应用。目前含 PING 和吞吐量测试功能。

    1.  新增
        1.  首次更新。
        1.  示例见 `examples/protocols/iperf`, `examples/protocols/icmp_echo`.
        1.  含 `xf_iperf` 及 `xf_ping`.
        1.  依赖 `xf_utils`, `xf_osal`, `xf_sys`.
        1.  已在 esp32, ws63 上测试可用。
        1.  默认不启用，在 `esp32` 和 `ws63` 上默认启用。菜单配置路径见 `(Top) -> public components -> xf_net_apps -> xf_net_apps Configuration`.
    1.  备注
        1.  详见 ([#17](https://github.com/x-eks-fusion/xfusion/pull/17)).

1.  `components/xf_osal`

    操作系统抽象层。

    1.  新增
        1.  首次更新。
        1.  示例见 `examples/osal`.
        1.  含事件、内核管理、互斥锁、消息队列、信号量、线程、定时器等功能.
    1.  备注
        1.  详见 ([#9](https://github.com/x-eks-fusion/xfusion/pull/9)).

1.  `components/xf_sys`

    系统级接口。

    1.  新增
        1.  首次更新。
        1.  示例见 `examples/system/sys`.
        1.  含看门狗控制、开关系统中断、获取时间戳及阻塞精确延迟等功能.
    1.  备注
        1.  详见 ([#15](https://github.com/x-eks-fusion/xfusion/pull/15)).

1.  `components/xf_wal`

    1.  `xf_wifi`

        wifi 抽象接口。

        1.  新增
            1.  首次更新。
        1.  示例见 `examples/wireless/wifi`.
            1.  已在 esp32, ws63 上测试可用。

    1.  `xf_sle`

        星闪接口抽象。

        1.  修改
            1.  修改错误命名。调整参数顺序，不向前兼容。

1.  `examples/protocols/sockets`

    socket 相关示例。

    1.  新增
        1.  首次更新。
    1.  备注
        1.  依赖底层 lwip 实现，XFusion 目前不含 lwip.
        1.  已在 esp32, ws63 上测试可用。

1.  `xf_build`

    构建脚本生成系统。

    1.  新增
        1.  支持收集 cflag 参数。
        1.  支持自定义文件夹。
        1.  支持 monitor, target 命令。
    1.  修复
        1.  修复 user_dirs 无效.
