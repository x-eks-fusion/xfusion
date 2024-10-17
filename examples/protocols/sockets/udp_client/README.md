# sockets:udp_client - wifi socket udp 客户端通信示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示 wifi 连接到 ap 后启动 socket udp 客户端，之后与服务器(地址默认为 `gw`)通信.

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

本示例连接到 ap 获取到 ip 后后，创建 udp 客户端(默认端口号为 `3333`)向 udp 服务端发送 `This is a message from client.`，接收到数据后通过 log 输出，以此循环。

## 常见问题

无。

## 示例平台差异

此示例对以下目标无明显差异：

1. esp32
1. ws63

## 运行日志

以下是 esp32 的运行日志。

```
I (808)-ex_easy_wifi: STA has started.
E (1822) wifi:sta is connecting, return error
E (2832) wifi:sta is connecting, return error
E (4854) wifi:sta is connecting, return error
E (5864) wifi:sta is connecting, return error
E (7886) wifi:sta is connecting, return error
E (8896) wifi:sta is connecting, return error
E (10918) wifi:sta is connecting, return error
E (11928) wifi:sta is connecting, return error
I (12319) wifi:new:<6,1>, old:<1,0>, ap:<255,255>, sta:<6,1>, prof:6
I (12320) wifi:state: init -> auth (b0)
I (12333) wifi:state: auth -> assoc (0)
I (12355) wifi:state: assoc -> run (10)
I (12377) wifi:connected with myssid, aid = 1, channel 6, 40U, bssid = 08:3a:f2:bf:1a:39
I (12378) wifi:security: WPA2-PSK, phy: bgn, rssi: -17
I (12381) wifi:pm start, type: 0

I (12382) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (12393)-ex_easy_wifi: The STA is connected to the AP.
I (12396)-ex_easy_wifi:
ssid:      myssid
mac:       08:3a:f2:bf:1a:39
channel:   6
authmode:  4
I (12411) wifi:dp: 2, bi: 102400, li: 4, scale listen interval from 307200 us to 409600 us
I (12414) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (13392) esp_netif_handlers: sta ip: 192.168.4.2, mask: 255.255.255.0, gw: 192.168.4.1
I (13394)-ex_easy_wifi:
got ip:       192.168.4.2
got gw:       192.168.4.1
got netmask:  255.255.255.0
I (13409)-sta: Socket created, sending to 192.168.4.1:3333
I (13411)-sta: Message sent: "This is a message from client.".
I (13417) wifi:<ba-add>idx:0 (ifx:0, 08:3a:f2:bf:1a:39), tid:0, ssn:0, winSize:64
I (13441)-sta: Received 26 bytes from 192.168.4.1:
I (13441)-sta: This is a message from server.
I (13543)-sta: Message sent: "This is a message from client.".
I (13546)-sta: Received 26 bytes from 192.168.4.1:
I (13546)-sta: This is a message from server.
I (13650)-sta: Message sent: "This is a message from client.".
I (13656)-sta: Received 26 bytes from 192.168.4.1:
I (13656)-sta: This is a message from server.
I (13758)-sta: Message sent: "This is a message from client.".
I (13771)-sta: Received 26 bytes from 192.168.4.1:
I (13771)-sta: This is a message from server.
...
```

以下是 ws63 的运行日志。

```
I (259)-ex_easy_wifi: STA has started.
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
I (1241)-ex_easy_wifi: The STA is connected to the AP.
I (1245)-ex_easy_wifi:
ssid:      myssid
mac:       d2:00:73:71:2a:f7
channel:   6
authmode:  4
I (2631)-ex_easy_wifi:
got ip:       192.168.4.2
got gw:       192.168.4.1
got netmask:  255.255.255.0
I (2680)-sta: Socket created, sending to 192.168.4.1:3333
I (2682)-sta: Message sent: "This is a message from client.".
I (2709)-sta: Received 26 bytes from 192.168.4.1:
I (2709)-sta: This is a message from server.
I (2811)-sta: Message sent: "This is a message from client.".
I (2818)-sta: Received 26 bytes from 192.168.4.1:
I (2819)-sta: This is a message from server.
I (2921)-sta: Message sent: "This is a message from client.".
...
```
