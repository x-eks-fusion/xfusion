# sockets:tcp_client - socket tcp 客户端通信示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示 wifi 连接到 ap 后启动 socket tcp 客户端，之后与服务器(地址默认为 `gw`)通信.

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

本示例连接到 ap 获取到 ip 后后，创建 tcp 客户端(默认端口号为 `3333`)向 tcp 服务端发送 `This is a message from client.`，接收到数据后通过 log 输出，以此循环。

## 常见问题

无。

## 示例平台差异

此示例对以下目标无明显差异：

1. esp32
1. ws63

## 运行日志

以下是 esp32 的运行日志。

```
I (329)-ex_easy_wifi: STA has started.
I (3312) wifi:new:<6,1>, old:<1,0>, ap:<255,255>, sta:<6,1>, prof:6
I (3314) wifi:state: init -> auth (b0)
I (3335) wifi:state: auth -> assoc (0)
I (3355) wifi:state: assoc -> run (10)
I (3376) wifi:connected with myssid, aid = 1, channel 6, 40U, bssid = 08:3a:f2:bf:26:e1
I (3377) wifi:security: WPA2-PSK, phy: bgn, rssi: -12
I (3381) wifi:pm start, type: 1

I (3382) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (2843)-ex_easy_wifi: The STA is connected to the AP.
I (2845)-ex_easy_wifi:
ssid:      myssid
mac:       08:3a:f2:bf:26:e1
channel:   6
authmode:  4
I (3432) wifi:dp: 2, bi: 102400, li: 4, scale listen interval from 307200 us to 409600 us
I (3432) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (4394) esp_netif_handlers: sta ip: 192.168.4.2, mask: 255.255.255.0, gw: 192.168.4.1
I (3845)-ex_easy_wifi:
got ip:       192.168.4.2
got gw:       192.168.4.1
got netmask:  255.255.255.0
I (4506) wifi:<ba-add>idx:0 (ifx:0, 08:3a:f2:bf:26:e1), tid:0, ssn:0, winSize:64
I (3967)-sta: Successfully connected to 192.168.4.1:3333
I (4970)-sta: sent     30 bytes: "This is a message from client."
I (4978)-sta: Received 30 bytes: "This is a message from server."
I (5981)-sta: sent     30 bytes: "This is a message from client."
I (5990)-sta: Received 30 bytes: "This is a message from server."
I (6993)-sta: sent     30 bytes: "This is a message from client."
I (7004)-sta: Received 30 bytes: "This is a message from server."
I (8007)-sta: sent     30 bytes: "This is a message from client."
I (8020)-sta: Received 30 bytes: "This is a message from server."
I (9022)-sta: sent     30 bytes: "This is a message from client."
I (9037)-sta: Received 30 bytes: "This is a message from server."
I (10040)-sta: sent     30 bytes: "This is a message from client."
I (10058)-sta: Received 30 bytes: "This is a message from server."
I (11061)-sta: sent     30 bytes: "This is a message from client."
I (11077)-sta: Received 30 bytes: "This is a message from server."
...
```

以下是 ws63 的运行日志。

```I (225)-ex_easy_wifi: STA has started.
Srv:2047: last scantime > 5s, trigger scan
wifi_sta_connect_fill_scan_param:: Specifying an SSID for scanning
drv_soc_ioctl ioctl_cmd->cmd=14.
hmac_single_hal_device_scan_complete:vap[1] time[762] chan_cnt[13] chan_0[1] back[0] event[6] mode[0]
Scan::vap[1] find bss_num[0] in regdomain, other bss_num[0]
Srv:548:recive event = 1
Srv:216:scan triggered by connect!
Srv:1974: not found suitable SSID and auth type
Srv:1974: not found suitable SSID and auth type
xo update temp:3,diff:0,xo:0x3083c
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
I (7415)-ex_easy_wifi: The STA is connected to the AP.
I (7420)-ex_easy_wifi: 
ssid:      myssid
mac:       a4:00:73:c0:62:a6
channel:   6
authmode:  4
I (8631)-ex_easy_wifi: 
got ip:       192.168.4.2
got gw:       192.168.4.1
got netmask:  255.255.255.0
I (9654)-sta: Successfully connected to 192.168.4.1:3333
I (10651)-sta: sent     30 bytes: "This is a message from client."
I (10674)-sta: Received 30 bytes: "This is a message from server."
I (11672)-sta: sent     30 bytes: "This is a message from client."
I (11678)-sta: Received 30 bytes: "This is a message from server."
I (12682)-sta: sent     30 bytes: "This is a message from client."
I (12704)-sta: Received 30 bytes: "This is a message from server."
I (13702)-sta: sent     30 bytes: "This is a message from client."
I (13718)-sta: Received 30 bytes: "This is a message from server."
I (14712)-sta: sent     30 bytes: "This is a message from client."
I (14721)-sta: Received 30 bytes: "This is a message from server."
I (15722)-sta: sent     30 bytes: "This is a message from client."
...
```
