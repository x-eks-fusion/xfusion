# static_ip_station - wifi 站点且设置静态 IP 示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示如何使用 xf_wifi 的站点模式 (sta) 快速连接到目标 AP，以及设置静态 IP。

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

1.  运行现象。

运行该示例的芯片会启动 STA 和停止 DHCP 客户端。在 STA 启动、停止、连接到目标 AP 或从目标 AP 断连时输出日志。

## 常见问题

无。

## 示例平台差异

此示例对以下目标无明显差异：

1. esp32
1. ws63

## 运行日志

以下是 esp32 的运行日志。

```
I (839)-sta: STA has started.
I (869)-sta: _wifi_ntask start!
I (1872)-sta: STA attempts to connect.
E (2873) wifi:sta is connecting, return error
I (3284) wifi:new:<6,1>, old:<1,0>, ap:<255,255>, sta:<6,1>, prof:6
I (3286) wifi:state: init -> auth (b0)
I (3293) wifi:state: auth -> assoc (0)
I (3304) wifi:state: assoc -> run (10)
I (3320) wifi:connected with myssid, aid = 1, channel 6, 40U, bssid = 08:3a:f2:bf:21:d9
I (3321) wifi:security: WPA2-PSK, phy: bgn, rssi: -8
I (3327) wifi:pm start, type: 1

I (3327) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (3336)-sta: The STA is connected to the AP.
I (3338)-sta:
ssid:      myssid
mac:       08:3a:f2:bf:21:d9
channel:   6
authmode:  4
I (3340) wifi:dp: 2, bi: 102400, li: 4, scale listen interval from 307200 us to 409600 us
I (3355) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (3362) esp_netif_handlers: sta ip: 192.168.6.2, mask: 255.255.255.0, gw: 192.168.6.1
I (3369)-sta:
got ip:       192.168.6.2
got gw:       192.168.6.1
got netmask:  255.255.255.0
I (3874)-sta: STA attempts to connect.
I (131031) wifi:<ba-add>idx:0 (ifx:0, 08:3a:f2:bf:21:d9), tid:0, ssn:0, winSize:64
```

以下是 ws63 的运行日志。

```
I (272)-sta: STA has started.
I (289)-sta: _wifi_ntask start!
Srv:2047: last scantime > 5s, trigger scan
wifi_sta_connect_fill_scan_param:: Specifying an SSID for scanning
drv_soc_ioctl ioctl_cmd->cmd=14.
hmac_single_hal_device_scan_complete:vap[1] time[762] chan_cnt[13] chan_0[1] back[0] event[6] mode[0]
Scan::vap[1] find bss_num[1] in regdomain, other bss_num[0]
Srv:548:recive event = 1
Srv:216:scan triggered by connect!
Srv:find ssid[myssid] auth type[3] pairwise[3] ft_flag[0]
drv_soc_ioctl ioctl_cmd->cmd=47.
drv_soc_ioctl ioctl_cmd->cmd=47.
drv_soc_ioctl ioctl_cmd->cmd=47.
drv_soc_ioctl ioctl_cmd->cmd=16.
drv_soc_ioctl ioctl_cmd->cmd=47.
I (1225)-sta:
got ip:       192.168.6.2
got gw:       192.168.6.1
got netmask:  255.255.255.0
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=5.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=5.
drv_soc_ioctl ioctl_cmd->cmd=1.
drv_soc_ioctl ioctl_cmd->cmd=3.
drv_soc_ioctl ioctl_cmd->cmd=1.
+NOTICE:CONNECTED
drv_soc_ioctl ioctl_cmd->cmd=6.
Srv:548:recive event = 2
I (1272)-sta: The STA is connected to the AP.
I (1276)-sta:
ssid:      myssid
mac:       be:00:73:7a:f9:b7
channel:   6
authmode:  4
I (1319)-sta: STA attempts to connect.
xo update temp:4,diff:0,xo:0x3083c
```
