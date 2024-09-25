# base_scan - wifi 基础扫描示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示如何使用 xf_wifi 的站点模式逐通道扫描周围的 AP，如果找到目标 AP 后连接。

本示例不含断开重连，如需断开重连见 [station](../../get_started/station/README.md).

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

运行该示例的芯片会逐通道扫描 AP，所有通道扫描完毕后输出，并且匹配有无目标 AP。如果有则连接，连接完毕后结束 ntask；如果没有则开启下一轮扫描。

## 常见问题

1.  wifi 回调函数

    建议不要在 wifi 回调函数中连接，在 ws63 平台中，如果在 wifi 回调函数中连接可能会导致递归调用连接函数。

## 示例平台差异

此示例对以下目标无明显差异：

1. esp32
1. ws63

## 运行日志

以下是 esp32 的运行日志。

```
I (841)-scan: STA has started.
I (871)-scan: _wifi_ntask start!
I (3920)-scan: done!
I (3921)-scan: ------------------------------
I (3921)-scan:
    index:     0
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -88
    authmode:  XF_WIFI_AUTH_WPA_WPA2_PSK
    channel:   1

# ...

I (4235)-scan:
    index:     20
    ssid:
    bssid:     **:**:**:**:**:**
    rssi:      -80
    authmode:  XF_WIFI_AUTH_WPA_WPA2_PSK
    channel:   12
I (4250)-scan: ------------------------------
I (4255)-scan: AP(myssid) has been found, try to connect.
I (6706) wifi:new:<6,1>, old:<1,0>, ap:<255,255>, sta:<6,1>, prof:6
I (7015) wifi:state: init -> auth (b0)
I (7020) wifi:state: auth -> assoc (0)
I (7034) wifi:state: assoc -> run (10)
I (7049) wifi:connected with myssid, aid = 1, channel 6, 40U, bssid = 08:3a:f2:bf:21:d9
I (7050) wifi:security: WPA2-PSK, phy: bgn, rssi: -11
I (7056) wifi:pm start, type: 1

I (7057) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (7066)-scan: The STA is connected to the AP.
I (7067)-scan:
ssid:      myssid
mac:       08:3a:f2:bf:21:d9
channel:   6
authmode:  4
I (7114)-scan: successful connection!
I (7128) wifi:dp: 2, bi: 102400, li: 4, scale listen interval from 307200 us to 409600 us
I (7129) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (8068) esp_netif_handlers: sta ip: 192.168.4.2, mask: 255.255.255.0, gw: 192.168.4.1
I (8069)-scan:
got ip:       192.168.4.2
got gw:       192.168.4.1
got netmask:  255.255.255.0
```

以下是 ws63 的运行日志。

```
I (275)-scan: STA has started.
I (293)-scan: _wifi_ntask start!
drv_soc_ioctl ioctl_cmd->cmd=14.
hmac_single_hal_device_scan_complete:vap[1] time[59] chan_cnt[1] chan_0[1] back[0] event[6] mode[0]
Scan::vap[1] find bss_num[0] in regdomain, other bss_num[0]
Srv:548:recive event = 1
drv_soc_ioctl ioctl_cmd->cmd=14.

# ...

hmac_single_hal_device_scan_complete:vap[1] time[59] chan_cnt[1] chan_0[13] back[0] event[6] mode[0]
Scan::vap[1] find bss_num[11] in regdomain, other bss_num[0]
Srv:548:recive event = 1
Srv:1723:sta_scan_results cnt 0
I (1207)-scan: done!
I (1209)-scan: ------------------------------
I (1214)-scan:
    index:     0
    ssid:      ***
    bssid:     **:**:**:**:**:**
    rssi:      -79
    authmode:  XF_WIFI_AUTH_WPA2_PSK
    channel:   2

# ...

I (1382)-scan: ------------------------------
I (1387)-scan: AP(myssid) has been found, try to connect.
Srv:find ssid[myssid] auth type[3] pairwise[3] ft_flag[0]
drv_soc_ioctl ioctl_cmd->cmd=47.
drv_soc_ioctl ioctl_cmd->cmd=47.
drv_soc_ioctl ioctl_cmd->cmd=47.
drv_soc_ioctl ioctl_cmd->cmd=16.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=47.
drv_soc_ioctl ioctl_cmd->cmd=6.

drv_soc_ioctl ioctl_cmd->cmd=5.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=5.
drv_soc_ioctl ioctl_cmd->cmd=1.
drv_soc_ioctl ioctl_cmd->cmd=3.
drv_soc_ioctl ioctl_cmd->cmd=1.
drv_soc_ioctl ioctl_cmd->cmd=1.
+NOTICE:CONNECTED
drv_soc_ioctl ioctl_cmd->cmd=6.
Srv:548:recive event = 2
I (2559)-scan: The STA is connected to the AP.
I (2564)-scan:
ssid:      myssid
mac:       08:3a:f2:bf:21:d9
channel:   6
authmode:  4
I (2583)-scan: successful connection!
xo update temp:3,diff:0,xo:0x3083c
I (4684)-scan:
got ip:       192.168.4.3
got gw:       192.168.4.1
got netmask:  255.255.255.0
```
