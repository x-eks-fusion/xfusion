# fal_littlefs - xf_fal 对接 littlefs 示例

## 支持情况

1.  espressif

    1.  esp32

## 示例简述

本示例演示如何使用 xf_fal 对接到 littlefs。

## 如何使用

1.  所需的软件和硬件。

    1.  外部软件包。

        [littlefs-project/littlefs at v2.9.1](https://github.com/littlefs-project/littlefs/tree/v2.9.1)

        移动到 `examples/storage/fal/fal_littlefs/components/littlefs` 目录，并使用以下命令克隆 littlefs 仓库。

        ```bash
        git clone --recursive https://github.com/littlefs-project/littlefs.git -b v2.9.1
        ```

    1.  硬件。

        esp32.

1.  配置。

    无特殊配置项。

1.  构建和烧录步骤与要点。

    无。

## 运行现象

本示例运行后，会输出一次开机计数。

## 常见问题

无。

## 示例平台差异

此示例对以下目标无明显差异：

1. esp32

## 运行日志

以下是 esp32 的运行日志。

```
I (39)-xf_fal: ==================== FAL partition table ===================
I (42)-xf_fal: | name         | flash_dev     |   offset   |    length  |
I (50)-xf_fal: -------------------------------------------------------------
I (57)-xf_fal: | default_part | default_flash | 0x00000000 | 0x00100000 |
I (65)-xf_fal: =============================================================
I (81)-littlefs_example: boot_count: 2
```
