# wifi_iperf:station - wifi sta iperf 示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示 wifi 连接到 ap 后默认启动 iperf udp 客户端并输出带宽信息.

通过 `xf menuconfig` 可以配置 iperf 协议与 iperf 角色（服务端/客户端）。

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

本示例连接到 ap 后，会每秒输出一次带宽信息。

## 常见问题

无。

## 示例平台差异

此示例对以下目标无明显差异：

1. esp32
1. ws63

## 运行日志

以下是 esp32 的运行日志。

```
I (279)-ex_easy_wifi: STA has started.
I (15679) wifi:new:<6,1>, old:<1,0>, ap:<255,255>, sta:<6,1>, prof:6
I (15681) wifi:state: init -> auth (b0)
I (15689) wifi:state: auth -> assoc (0)
I (15700) wifi:state: assoc -> run (10)
I (15725) wifi:connected with myssid, aid = 1, channel 6, 40U, bssid = 08:3a:f2:bf:1a:39
I (15726) wifi:security: WPA2-PSK, phy: bgn, rssi: -14
I (15731) wifi:pm start, type: 1

I (15731) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (15183)-ex_easy_wifi: The STA is connected to the AP.
I (15186)-ex_easy_wifi: 
ssid:      myssid
mac:       08:3a:f2:bf:1a:39
channel:   6
authmode:  4
I (15763) wifi:dp: 2, bi: 102400, li: 4, scale listen interval from 307200 us to 409600 us
I (15764) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (16744) esp_netif_handlers: sta ip: 192.168.4.2, mask: 255.255.255.0, gw: 192.168.4.1
I (16186)-ex_easy_wifi: 
got ip:       192.168.4.2
got gw:       192.168.4.1
got netmask:  255.255.255.0
I (16196)-xf_main: 
src_ip4:       192.168.4.2
dest_ip4:      192.168.4.1
I (19301)-xf_main: mode=udp-client sip=192.168.4.2:5001,dip=192.168.4.1:5001,interval=1, time=10
I (19303)-iperf: Socket created, sending to 192.168.4.1:5001

        Interval Bandwidth
I (19906) wifi:<ba-add>idx:0 (ifx:0, 08:3a:f2:bf:1a:39), tid:0, ssn:0, winSize:64
   0-   1 sec       4.68 Mbits/sec
   1-   2 sec       4.98 Mbits/sec
   2-   3 sec       6.51 Mbits/sec
   3-   4 sec       5.26 Mbits/sec
   4-   5 sec       5.23 Mbits/sec
   5-   6 sec       6.84 Mbits/sec
   6-   7 sec       6.91 Mbits/sec
   7-   8 sec       5.15 Mbits/sec
   8-   9 sec       5.20 Mbits/sec
   9-  10 sec       5.09 Mbits/sec
   0-  10 sec       5.58 Mbits/sec
I (29322)-iperf: UDP Socket client is closed
I (29323)-iperf: iperf done
I (32503)-xf_main: mode=tcp-client sip=192.168.4.2:5001,dip=192.168.4.1:5001,interval=1, time=10
I (32519)-iperf: Successfully connected

        Interval Bandwidth
   0-   1 sec       2.75 Mbits/sec
   1-   2 sec       3.40 Mbits/sec
   2-   3 sec       3.14 Mbits/sec
   3-   4 sec       3.27 Mbits/sec
   4-   5 sec       2.88 Mbits/sec
   5-   6 sec       3.01 Mbits/sec
   6-   7 sec       2.75 Mbits/sec
   7-   8 sec       3.14 Mbits/sec
   8-   9 sec       2.88 Mbits/sec
   9-  10 sec       3.40 Mbits/sec
   0-  10 sec       3.06 Mbits/sec
I (42553)-iperf: TCP Socket client is closed.
I (42554)-iperf: iperf done
I (45604)-xf_main: mode=udp-server sip=192.168.4.2:5001,dip=192.168.4.1:5001,interval=1, time=10
I (45605)-iperf: Socket created
I (45607)-iperf: Socket bound, port 35091

        Interval Bandwidth
   0-   1 sec       9.27 Mbits/sec
   1-   2 sec       3.21 Mbits/sec
   2-   3 sec       13.20 Mbits/sec
   3-   4 sec       12.97 Mbits/sec
   4-   5 sec       13.44 Mbits/sec
   5-   6 sec       11.53 Mbits/sec
   6-   7 sec       13.08 Mbits/sec
   7-   8 sec       11.80 Mbits/sec
   8-   9 sec       14.31 Mbits/sec
   9-  10 sec       11.61 Mbits/sec
   0-  10 sec       11.44 Mbits/sec
I (55622)-iperf: Udp socket server is closed.
I (55625)-iperf: iperf done
I (58706)-xf_main: mode=tcp-server sip=192.168.4.2:5001,dip=192.168.4.1:5001,interval=1, time=10
I (58708)-iperf: Socket created
I (59135)-iperf: accept: 192.168.4.1,60582


        Interval Bandwidth
   0-   1 sec       3.39 Mbits/sec
   1-   2 sec       3.42 Mbits/sec
   2-   3 sec       3.06 Mbits/sec
   3-   4 sec       3.21 Mbits/sec
   4-   5 sec       3.29 Mbits/sec
   5-   6 sec       2.90 Mbits/sec
   6-   7 sec       2.97 Mbits/sec
   7-   8 sec       3.28 Mbits/sec
   8-   9 sec       3.15 Mbits/sec
   9-  10 sec       2.70 Mbits/sec
   0-  10 sec       3.14 Mbits/sec
I (69148)-iperf: TCP Socket server is closed.
I (69149)-iperf: iperf done
I (69208)-xf_main: test done!
```

以下是 ws63 的运行日志。

```
I (225)-ex_easy_wifi: STA has started.
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
I (1215)-ex_easy_wifi: The STA is connected to the AP.
I (1220)-ex_easy_wifi: 
ssid:      myssid
mac:       b6:00:73:89:71:79
channel:   6
authmode:  4
I (2631)-ex_easy_wifi: 
got ip:       192.168.4.2
got gw:       192.168.4.1
got netmask:  255.255.255.0
I (2650)-xf_main: 
src_ip4:       192.168.4.2
dest_ip4:      192.168.4.1
xo update temp:3,diff:0,xo:0x3083c
I (5750)-xf_main: mode=udp-client sip=192.168.4.2:5001,dip=192.168.4.1:5001,interval=1, time=10
I (5754)-iperf: Socket created, sending to 192.168.4.1:5001

        Interval Bandwidth
xo update temp:4,diff:0,xo:0x3083c
   0-   1 sec       44.05 Mbits/sec
   1-   2 sec       40.38 Mbits/sec
   2-   3 sec       34.30 Mbits/sec
   3-   4 sec       34.48 Mbits/sec
   4-   5 sec       37.54 Mbits/sec
   5-   6 sec       33.38 Mbits/sec
   6-   7 sec       33.49 Mbits/sec
   7-   8 sec       34.25 Mbits/sec
   8-   9 sec       35.19 Mbits/sec
   9-  10 sec       40.80 Mbits/sec
   0-  10 sec       36.79 Mbits/sec
I (15983)-iperf: UDP Socket client is closed
I (15986)-iperf: iperf done
I (19121)-xf_main: mode=tcp-client sip=192.168.4.2:5001,dip=192.168.4.1:5001,interval=1, time=10
I (19135)-iperf: Successfully connected

        Interval Bandwidth
   0-   1 sec       6.81 Mbits/sec
   1-   2 sec       6.68 Mbits/sec
   2-   3 sec       6.81 Mbits/sec
   3-   4 sec       6.29 Mbits/sec
   4-   5 sec       7.20 Mbits/sec
   5-   6 sec       7.20 Mbits/sec
   6-   7 sec       7.34 Mbits/sec
   7-   8 sec       7.34 Mbits/sec
   8-   9 sec       7.20 Mbits/sec
   9-  10 sec       7.20 Mbits/sec
   0-  10 sec       7.01 Mbits/sec
I (29154)-iperf: TCP Socket client is closed.
I (29154)-iperf: iperf done
I (32221)-xf_main: mode=udp-server sip=192.168.4.2:5001,dip=192.168.4.1:5001,interval=1, time=10
I (32226)-iperf: Socket created
I (32229)-iperf: Socket bound, port 35091

        Interval Bandwidth
   0-   1 sec       22.42 Mbits/sec
   1-   2 sec       23.03 Mbits/sec
   2-   3 sec       18.59 Mbits/sec
   3-   4 sec       21.83 Mbits/sec
   4-   5 sec       20.08 Mbits/sec
   5-   6 sec       20.85 Mbits/sec
   6-   7 sec       20.29 Mbits/sec
   7-   8 sec       21.68 Mbits/sec
   8-   9 sec       21.77 Mbits/sec
   9-  10 sec       21.36 Mbits/sec
   0-  10 sec       21.19 Mbits/sec
I (42313)-iperf: Udp socket server is closed.
I (42317)-iperf: iperf done
I (45332)-xf_main: mode=tcp-server sip=192.168.4.2:5001,dip=192.168.4.1:5001,interval=1, time=10
I (45336)-iperf: Socket created
I (45741)-iperf: accept: 192.168.4.1,64210


        Interval Bandwidth
   0-   1 sec       7.53 Mbits/sec
   1-   2 sec       7.88 Mbits/sec
   2-   3 sec       7.80 Mbits/sec
   3-   4 sec       7.92 Mbits/sec
   4-   5 sec       8.06 Mbits/sec
   5-   6 sec       7.11 Mbits/sec
   6-   7 sec       7.73 Mbits/sec
   7-   8 sec       7.91 Mbits/sec
   8-   9 sec       7.87 Mbits/sec
   9-  10 sec       8.03 Mbits/sec
   0-  10 sec       7.78 Mbits/sec
I (55780)-iperf: TCP Socket server is closed.
I (55783)-iperf: iperf done
I (55802)-xf_main: test done!
```
