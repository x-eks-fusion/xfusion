# fal_flashdb - xf_fal 对接 flashdb 示例

## 支持情况

1.  espressif

    1.  esp32

## 示例简述

本示例演示如何使用 xf_fal 对接到 flashdb, 并使用原生示例。

本例中基于默认的 `default_flash` 设备，注册了一个提供给 FlashDB 使用的分区表：

```c
// 默认情况
static const xf_fal_partition_t sc_default_flash_fdb_partition_table[] = {
    {"fdb_kvdb", "default_flash", 0,     40960, },
    {"fdb_tsdb", "default_flash", 40960, 40960, },
};
```

默认的 `default_flash` 设备在 esp32 中由对接层实现（见 `ports/espressif/esp32/port_fal.c`）。

注意，给 FlashDB 使用的分区实际上与默认分区(`default_part`)产生了交叠：

```log
I (39)-xf_fal: ==================== FAL partition table ===================
I (42)-xf_fal: | name         | flash_dev     |   offset   |    length  |
I (50)-xf_fal: -------------------------------------------------------------
I (57)-xf_fal: | default_part | default_flash | 0x00000000 | 0x00100000 |
I (65)-xf_fal: | fdb_kvdb     | default_flash | 0x00000000 | 0x0000a000 |
I (72)-xf_fal: | fdb_tsdb     | default_flash | 0x0000a000 | 0x0000a000 |
I (80)-xf_fal: =============================================================
```

默认 flash 设备 `default_flash` 和默认分区 `default_part` 为了解耦平台分区差异，提供给实际工程复用的。

实际工程中，用户需要自行处理分区情况。

## 如何使用

1.  所需的软件和硬件。

    1.  外部软件包。

        [armink/FlashDB at 2.1.1](https://github.com/armink/FlashDB/tree/2.1.1)

        移动到 `examples/storage/fal/fal_flashdb/components/FlashDB` 目录，并使用以下命令克隆 FlashDB 仓库。

        ```bash
        git clone --recursive https://github.com/armink/FlashDB.git -b 2.1.1
        ```

    1.  硬件。

        esp32.

1.  配置。

    见 `examples/storage/fal/fal_flashdb/main/XFKconfig`。
    用户可以自行配置：

    1.  PORT_FDB_XF_FAL_FLASH_NAME: 提供给 flashdb 使用的 xf_fal flash 设备名字。
    1.  PORT_FDB_XF_FAL_KVDB_PARTITION_NAME: 键值数据库（KVDB）分区名字。
    1.  PORT_FDB_XF_FAL_KVDB_PARTITION_OFFSET: 键值数据库（KVDB）分区在 flash 设备中的偏移量。
    1.  PORT_FDB_XF_FAL_KVDB_PARTITION_LENGTH: 键值数据库（KVDB）分区的长度。
    1.  PORT_FDB_XF_FAL_TSDB_PARTITION_NAME: 时序数据（TSDB）分区名字。
    1.  PORT_FDB_XF_FAL_TSDB_PARTITION_OFFSET: 时序数据（TSDB）分区在 flash 设备中的偏移量。
    1.  PORT_FDB_XF_FAL_TSDB_PARTITION_LENGTH: 时序数据（TSDB）分区的长度。

1.  构建和烧录步骤与要点。

    无。

## 运行现象

本示例运行后，会运行 FlashDB 原生示例，并输出日志。

## 常见问题

无。

## 示例平台差异

无。

## 运行日志

以下是 esp32 的运行日志。

```
I (490) main_task: Calling app_main()
I (39)-xf_fal: ==================== FAL partition table ===================
I (42)-xf_fal: | name         | flash_dev     |   offset   |    length  |
I (50)-xf_fal: -------------------------------------------------------------
I (57)-xf_fal: | default_part | default_flash | 0x00000000 | 0x00100000 |
I (65)-xf_fal: | fdb_kvdb     | default_flash | 0x00000000 | 0x0000a000 |
I (72)-xf_fal: | fdb_tsdb     | default_flash | 0x0000a000 | 0x0000a000 |
I (80)-xf_fal: =============================================================
[FlashDB] FlashDB V2.1.1 is initialize success.
[FlashDB] You can get the latest version on https://github.com/armink/FlashDB .
[FlashDB][sample][kvdb][basic] ==================== kvdb_basic_sample ====================
[FlashDB][sample][kvdb][basic] get the 'boot_count' value is 3
[FlashDB][sample][kvdb][basic] set the 'boot_count' value to 4
[FlashDB][sample][kvdb][basic] ===========================================================
[FlashDB][sample][kvdb][string] ==================== kvdb_type_string_sample ====================
[FlashDB][sample][kvdb][string] create the 'temp' string KV, value is: 36C
[FlashDB][sample][kvdb][string] get the 'temp' value is: 36C
[FlashDB][sample][kvdb][string] set 'temp' value to 38C
[FlashDB][sample][kvdb][string] delete the 'temp' finish
[FlashDB][sample][kvdb][string] ===========================================================
[FlashDB][sample][kvdb][blob] ==================== kvdb_type_blob_sample ====================
[FlashDB][sample][kvdb][blob] create the 'temp' blob KV, value is: 36
[FlashDB][sample][kvdb][blob] get the 'temp' value is: 36
[FlashDB][sample][kvdb][blob] set 'temp' value to 38
[FlashDB][sample][kvdb][blob] delete the 'temp' finish
[FlashDB][sample][kvdb][blob] ===========================================================
[FlashDB][sample][tsdb] ==================== tsdb_sample ====================
[FlashDB][sample][tsdb] append the new status.temp (36) and status.humi (85)
[FlashDB][sample][tsdb] append the new status.temp (38) and status.humi (90)
[FlashDB][sample][tsdb] [query_cb] queried a TSL: time: 1, temp: 36, humi: 85
[FlashDB][sample][tsdb] [query_cb] queried a TSL: time: 2, temp: 38, humi: 90
[FlashDB][sample][tsdb] [query_cb] queried a TSL: time: 3, temp: 36, humi: 85
[FlashDB][sample][tsdb] [query_cb] queried a TSL: time: 4, temp: 38, humi: 90
[FlashDB][sample][tsdb] [query_cb] queried a TSL: time: 5, temp: 36, humi: 85
[FlashDB][sample][tsdb] [query_cb] queried a TSL: time: 6, temp: 38, humi: 90
[FlashDB][sample][tsdb] [query_cb] queried a TSL: time: 7, temp: 36, humi: 85
[FlashDB][sample][tsdb] [query_cb] queried a TSL: time: 8, temp: 38, humi: 90
[FlashDB][sample][tsdb] [query_by_time_cb] queried a TSL: time: 1, temp: 36, humi: 85
[FlashDB][sample][tsdb] [query_by_time_cb] queried a TSL: time: 2, temp: 38, humi: 90
[FlashDB][sample][tsdb] [query_by_time_cb] queried a TSL: time: 3, temp: 36, humi: 85
[FlashDB][sample][tsdb] [query_by_time_cb] queried a TSL: time: 4, temp: 38, humi: 90
[FlashDB][sample][tsdb] [query_by_time_cb] queried a TSL: time: 5, temp: 36, humi: 85
[FlashDB][sample][tsdb] [query_by_time_cb] queried a TSL: time: 6, temp: 38, humi: 90
[FlashDB][sample][tsdb] [query_by_time_cb] queried a TSL: time: 7, temp: 36, humi: 85
[FlashDB][sample][tsdb] [query_by_time_cb] queried a TSL: time: 8, temp: 38, humi: 90
[FlashDB][sample][tsdb] query count is: 2
[FlashDB][sample][tsdb] set the TSL (time 1) status from 3 to 3
[FlashDB][sample][tsdb] set the TSL (time 2) status from 3 to 3
[FlashDB][sample][tsdb] set the TSL (time 3) status from 3 to 3
[FlashDB][sample][tsdb] set the TSL (time 4) status from 3 to 3
[FlashDB][sample][tsdb] set the TSL (time 5) status from 3 to 3
[FlashDB][sample][tsdb] set the TSL (time 6) status from 3 to 3
[FlashDB][sample][tsdb] set the TSL (time 7) status from 2 to 3
[FlashDB][sample][tsdb] set the TSL (time 8) status from 2 to 3
[FlashDB][sample][tsdb] ===========================================================
I (446)-flashdb_example: flashdb_demo done!
```
