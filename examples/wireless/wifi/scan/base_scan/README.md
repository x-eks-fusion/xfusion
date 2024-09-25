# base_scan - wifi 基础扫描示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示如何使用 xf_wifi 的站点模式逐通道扫描周围的 AP.

注意，本示例只演示扫描，如果扫描后还需连接请见 [scan_and_connect](../scan_and_connect/README.md) 示例。

## 如何使用

1.  所需的软件和硬件。

    1.  外部软件包。

        无。

    1.  硬件。

        片内支持 wifi 的芯片，如 esp32、ws63.

1.  配置。

    无特殊配置项。

1.  构建和烧录步骤与要点。

    无。

## 运行现象

运行该示例的芯片会逐通道扫描 AP，扫描完毕后直接输出。

## 常见问题

无。

## 示例平台差异

此示例对以下目标无明显差异：

1. esp32
1. ws63

## 运行日志

以下是 esp32 的运行日志。

```
I (891)-scan: _wifi_ntask start!
I (1094)-scan: ------------------------------
I (1098)-scan: Channel 1 scanning is complete. There are 4 APs.
I (1099)-scan:
    index:     0
    ssid:
    bssid:     **:**:**:**:**:**
    rssi:      -81
    authmode:  XF_WIFI_AUTH_WPA_WPA2_PSK
I (1109)-scan:
    index:     1
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -80
    authmode:  XF_WIFI_AUTH_WPA_WPA2_PSK
I (1123)-scan:
    index:     2
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -81
    authmode:  XF_WIFI_AUTH_WPA2_PSK
I (1138)-scan:
    index:     3
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -81
    authmode:  XF_WIFI_AUTH_WPA_WPA2_PSK
I (1152)-scan: ------------------------------

# ...

I (4230)-scan: ------------------------------
I (4233)-scan: Channel 13 scanning is complete. There are 2 APs.
I (4233)-scan:
    index:     0
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -90
    authmode:  XF_WIFI_AUTH_WPA_WPA2_PSK
I (4246)-scan:
    index:     1
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -85
    authmode:  XF_WIFI_AUTH_WPA_WPA2_PSK
I (4261)-scan: ------------------------------
I (4267)-scan: done!
```

以下是 ws63 的运行日志。

```
I (295)-scan: _wifi_ntask start!
drv_soc_ioctl ioctl_cmd->cmd=14.
hmac_single_hal_device_scan_complete:vap[1] time[59] chan_cnt[1] chan_0[1] back[0] event[6] mode[0]
Scan::vap[1] find bss_num[4] in regdomain, other bss_num[0]
Srv:548:recive event = 1
I (366)-scan: ------------------------------
Srv:1723:sta_scan_results cnt 4
I (373)-scan: Channel 1 scanning is complete. There are 4 APs.
I (380)-scan:
    index:     0
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -75
    authmode:  XF_WIFI_AUTH_WPA_WPA2_PSK
I (393)-scan:
    index:     1
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -78
    authmode:  XF_WIFI_AUTH_WPA2_PSK
I (407)-scan:
    index:     2
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -79
    authmode:  XF_WIFI_AUTH_WPA2_PSK
I (421)-scan:
    index:     3
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -80
    authmode:  XF_WIFI_AUTH_WPA2_PSK
I (435)-scan: ------------------------------
drv_soc_ioctl ioctl_cmd->cmd=14.
hmac_single_hal_device_scan_complete:vap[1] time[68] chan_cnt[1] chan_0[2] back[0] event[6] mode[0]
Scan::vap[1] find bss_num[1] in regdomain, other bss_num[0]
Srv:548:recive event = 1

# ...

I (1696)-scan: ------------------------------
Srv:1723:sta_scan_results cnt 2
I (1704)-scan: Channel 13 scanning is complete. There are 2 APs.
I (1710)-scan:
    index:     0
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -79
    authmode:  XF_WIFI_AUTH_WPA_WPA2_PSK
I (1724)-scan:
    index:     1
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -84
    authmode:  XF_WIFI_AUTH_WPA_WPA2_PSK
I (1739)-scan: ------------------------------
I (1744)-scan: done!
```
