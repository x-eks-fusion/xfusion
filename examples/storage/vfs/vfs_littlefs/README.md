# vfs_littlefs - xf_vfs 对接 littlefs 示例

## 支持情况

1.  espressif

    1.  esp32

1.  linux

    1.  sim_linux

## 示例简述

本示例内含两个测试项目。

1.  test_base

    此示例演示了如何使用 xf_vfs_littlefs，包括以下几个步骤：

    1.  注册 xf_fal 分区表以及初始化 xf_fal.
    1.  配置 xf_vfs_littlefs, 并注册到 xf_vfs.
    1.  使用 xf_vfs 读写文件以及其他操作。
    1.  卸载分区并禁用 LittleFS.

1.  test_benchmark

    此示例演示了使用 xf_vfs_littlefs 测试性能。

## 如何使用

1.  所需的软件和硬件。

    1.  外部软件包。

        [littlefs-project/littlefs at v2.9.1](https://github.com/littlefs-project/littlefs/tree/v2.9.1)

        移动到 `components/xf_vfs_littlefs` 目录，并使用以下命令克隆 littlefs 仓库。

        ```bash
        git clone --recursive https://github.com/littlefs-project/littlefs.git -b v2.9.1
        ```

    1.  硬件。

        esp32，或者使用 sim_linux.

1.  配置。

    1.  工程配置。

        见 `components/xf_vfs_littlefs/XFKconfig`。

    1.  相关平台配置

        1.  esp32

            使用 `xf menuconfig sub` 打开原生菜单，修改以下两个配置（ **一定要增大 main task 任务栈大小** ）：

            ```
            CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_240=y
            CONFIG_ESP_MAIN_TASK_STACK_SIZE=8192
            ```

            使用 `xf menuconfig` 打开 XFusion 菜单，修改以下配置：

            ```
            CONFIG_XF_FAL_ENABLE=y
            CONFIG_XF_VFS_ENABLE=y
            CONFIG_XF_VFS_CUSTOM_FD_SETSIZE_ENABLE=y
            CONFIG_XF_OSAL_FREERTOS=y
            ```

        1.  sim_linux

            使用 `xf menuconfig` 打开 XFusion 菜单，修改以下配置：

            ```
            # Welcome to xfusion
            # CONFIG_XF_HAL_GPIO_ENABLE is not set
            # CONFIG_XF_HAL_PWM_ENABLE is not set
            # CONFIG_XF_HAL_TIM_ENABLE is not set
            # CONFIG_XF_HAL_UART_ENABLE is not set
            # CONFIG_XF_HAL_I2C_ENABLE is not set
            # CONFIG_XF_HAL_SPI_ENABLE is not set
            # CONFIG_XF_HAL_ADC_ENABLE is not set
            # CONFIG_XF_HAL_DAC_ENABLE is not set
            CONFIG_XF_INIT_IMPL_BY_CONSTRUCTOR_ENABLE=y
            # CONFIG_XF_OSAL_TIMER_ENABLE is not set
            # CONFIG_XF_OSAL_QUEUE_ENABLE is not set
            CONFIG_XF_OSAL_USER=y
            CONFIG_XF_FAL_ENABLE=y
            CONFIG_XF_LOG_OBJ_NUM=2
            CONFIG_XF_VFS_ENABLE=y
            CONFIG_XF_VFS_CUSTOM_FD_SETSIZE_ENABLE=y
            ```

1.  构建和烧录步骤与要点。

    无。

## 运行现象

本示例运行后，会运行两个测试示例，并输出日志。

## 常见问题

无。

## 示例平台差异

无。

## 运行日志

### esp32

以下是 esp32 的运行日志。

```

88000 bytes written in 1261898 us
88000 bytes written in 1265063 us
88000 bytes written in 1265010 us
88000 bytes written in 1350683 us
88000 bytes written in 1265603 us
------------
88000 bytes read in 1205168 us
88000 bytes read in 1204913 us
88000 bytes read in 1204939 us
88000 bytes read in 1204863 us
88000 bytes read in 1204548 us
------------
deleted file 0 in 5163 us
deleted file 1 in 4807 us
deleted file 2 in 6054 us
deleted file 3 in 5632 us
deleted file 4 in 5955 us
------------
Total (440000) Write: 6408257 us
Total (440000) Read: 6024431 us
Total (440000) Delete: 27611 us
```

### sim_linux

```
I (1318344695)-test_base: Initializing LittleFS
../../../temp/example_xf_vfs/components/xf_vfs_littlefs/littlefs/lfs.c:1369:error: Corrupted dir pair at {0x0, 0x1}
W (1318344695)-xf_littlefs: mount failed,  (-84). formatting...
I (1318344695)-test_base: Partition size: total: 8388608, used: 8192
I (1318344695)-test_base: Opening file
I (1318344695)-test_base: File written
I (1318344695)-test_base: Renaming file
I (1318344695)-test_base: Reading file
I (1318344695)-test_base: Read from file: 'LittleFS Rocks!'
I (1318344695)-test_base: LittleFS unmounted
I (1318344695)-test_benchmark: TEST_BENCHMARK
I (1318344695)-test_benchmark: TEST LittleFS:

88000 bytes written in 585 us
88000 bytes written in 582 us
88000 bytes written in 576 us
88000 bytes written in 630 us
88000 bytes written in 547 us
------------
88000 bytes read in 7697 us
88000 bytes read in 11011 us
88000 bytes read in 7557 us
88000 bytes read in 8939 us
88000 bytes read in 8032 us
------------
deleted file 0 in 24 us
deleted file 1 in 21 us
deleted file 2 in 21 us
deleted file 3 in 27 us
deleted file 4 in 22 us
------------
Total (440000) Write: 2920 us
Total (440000) Read: 43236 us
Total (440000) Delete: 115 us
```
