# OSAL 对接说明

1. 该仓库包含了一些常见的 RTOS 的移植。
2. 我们在 XFKconfig 中对其做了配置选项。
3. 不同 target 移植时需要通过配置 board 下对应的 target 文件夹内的 sdkconfig.defaults 来让默认配置修改
   例如：

```kconfig
# Welcome to xfusion
CONFIG_XF_OSAL_FREERTOS=y

```

4. 如果想屏蔽 OSAL 可以直接配置 CONFIG_XF_OSAL_ENABLE 为 n。
