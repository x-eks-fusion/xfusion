# wifi_iperf:softap - wifi ap iperf 示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示启动 wifi ap 后默认启动 iperf udp 服务端，待客户端连接后输出带宽信息.

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

本示例启动 ap 等到 sta 连接后，会每秒输出一次带宽信息。

## 常见问题

无。

## 示例平台差异

此示例对以下目标无明显差异：

1. esp32
1. ws63

## 运行日志

以下是 esp32 的运行日志。

```
I (284)-ex_easy_wifi: AP has started.
I (3854) wifi:new:<6,1>, old:<6,1>, ap:<6,1>, sta:<255,255>, prof:6
I (3856) wifi:station: aa:bb:cc:dd:ee:ff join, AID=1, bgn, 40U
I (3329)-ex_easy_wifi: station aa:bb:cc:dd:ee:ff join
I (3926) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (3368)-ex_easy_wifi: Assign ip 192.168.4.2 to site aa:bb:cc:dd:ee:ff
I (3382)-xf_main:
src_ip4:       192.168.4.1
dest_ip4:      192.168.4.2
I (6383)-xf_main: mode=udp-server sip=192.168.4.1:5001,dip=192.168.4.2:5001,interval=1, time=10
I (6384)-iperf: Socket created
I (6386)-iperf: Socket bound, port 35091
I (8072) wifi:<ba-add>idx:2 (ifx:1, aa:bb:cc:dd:ee:ff), tid:0, ssn:0, winSize:64

        Interval Bandwidth
   0-   1 sec       3.93 Mbits/sec
   1-   2 sec       4.75 Mbits/sec
   2-   3 sec       5.95 Mbits/sec
   3-   4 sec       4.93 Mbits/sec
   4-   5 sec       5.16 Mbits/sec
   5-   6 sec       6.60 Mbits/sec
   6-   7 sec       6.84 Mbits/sec
   7-   8 sec       4.79 Mbits/sec
   8-   9 sec       5.09 Mbits/sec
   9-  10 sec       5.10 Mbits/sec
   0-  10 sec       5.31 Mbits/sec
I (17528)-iperf: Udp socket server is closed.
I (17529)-iperf: iperf done
I (20584)-xf_main: mode=tcp-server sip=192.168.4.1:5001,dip=192.168.4.2:5001,interval=1, time=10
I (20585)-iperf: Socket created
I (20696)-iperf: accept: 192.168.4.2,53216


        Interval Bandwidth
   0-   1 sec       2.92 Mbits/sec
   1-   2 sec       3.31 Mbits/sec
   2-   3 sec       3.11 Mbits/sec
   3-   4 sec       3.21 Mbits/sec
   4-   5 sec       2.90 Mbits/sec
   5-   6 sec       3.14 Mbits/sec
   6-   7 sec       2.79 Mbits/sec
   7-   8 sec       3.27 Mbits/sec
   8-   9 sec       2.88 Mbits/sec
   9-  10 sec       3.40 Mbits/sec
   0-  10 sec       3.09 Mbits/sec
I (30715)-iperf: TCP Socket server is closed.
I (30716)-iperf: iperf done
I (33885)-xf_main: mode=udp-client sip=192.168.4.1:5001,dip=192.168.4.2:5001,interval=1, time=10
I (33886)-iperf: Socket created, sending to 192.168.4.2:5001

        Interval Bandwidth
   0-   1 sec       14.39 Mbits/sec
   1-   2 sec       2.16 Mbits/sec
   2-   3 sec       15.00 Mbits/sec
   3-   4 sec       15.89 Mbits/sec
   4-   5 sec       14.37 Mbits/sec
   5-   6 sec       13.91 Mbits/sec
   6-   7 sec       15.48 Mbits/sec
   7-   8 sec       14.53 Mbits/sec
   8-   9 sec       16.14 Mbits/sec
   9-  10 sec       12.66 Mbits/sec
   0-  10 sec       13.45 MI (43909)-iperf: UDP Socket client is closed
bits/sec
I (43910)-iperf: iperf done
I (47087)-xf_main: mode=tcp-client sip=192.168.4.1:5001,dip=192.168.4.2:5001,interval=1, time=10
I (47294)-iperf: Successfully connected

        Interval Bandwidth
   0-   1 sec       3.27 Mbits/sec
   1-   2 sec       3.53 Mbits/sec
   2-   3 sec       3.01 Mbits/sec
   3-   4 sec       3.14 Mbits/sec
   4-   5 sec       3.40 Mbits/sec
   5-   6 sec       2.88 Mbits/sec
   6-   7 sec       3.01 Mbits/sec
   7-   8 sec       3.14 Mbits/sec
   8-   9 sec       3.27 Mbits/sec
   9-  10 sec       2.62 Mbits/sec
   0-  10 sec       3.13 Mbits/sec
I (57306)-iperf: TCP Socket client is closed.
I (57307)-iperf: iperf done
I (57388)-xf_main: test done!
```

以下是 ws63 的运行日志。

```
I (232)-ex_easy_wifi: AP has started.
drv_soc_ioctl ioctl_cmd->cmd=2.

drv_soc_ioctl ioctl_cmd->cmd=2.
drv_soc_ioctl ioctl_cmd->cmd=1.
drv_soc_ioctl ioctl_cmd->cmd=3.
drv_soc_ioctl ioctl_cmd->cmd=5.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=5.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=1.
drv_soc_ioctl ioctl_cmd->cmd=3.
+NOTICE:STA CONNECTED
drv_soc_ioctl ioctl_cmd->cmd=6.
Srv:548:recive event = 11
Srv:2781:sta_num[1]!!
I (1224)-ex_easy_wifi: station aa:bb:cc:dd:ee:ff join
I (1680)-ex_easy_wifi: Assign ip 192.168.4.2 to site aa:bb:cc:dd:ee:ff
Srv:2781:sta_num[1]!!
I (1741)-xf_main:
src_ip4:       192.168.4.1
dest_ip4:      192.168.4.2
xo update temp:4,diff:0,xo:0x30b3c
I (4740)-xf_main: mode=udp-server sip=192.168.4.1:5001,dip=192.168.4.2:5001,interval=1, time=10
I (4744)-iperf: Socket created
I (4748)-iperf: Socket bound, port 35091

        Interval Bandwidth
   0-   1 sec       22.30 Mbits/sec
   1-   2 sec       23.73 Mbits/sec
   2-   3 sec       28.02 Mbits/sec
   3-   4 sec       28.25 Mbits/sec
   4-   5 sec       26.17 Mbits/sec
   5-   6 sec       28.22 Mbits/sec
   6-   7 sec       28.69 Mbits/sec
   7-   8 sec       27.74 Mbits/sec
   8-   9 sec       27.63 Mbits/sec
   9-  10 sec       27.55 Mbits/sec
   0-  10 sec       26.83 Mbits/sec
I (15785)-iperf: Udp socket server is closed.
I (15788)-iperf: iperf done
I (18861)-xf_main: mode=tcp-server sip=192.168.4.1:5001,dip=192.168.4.2:5001,interval=1, time=10
I (18865)-iperf: Socket created
I (19131)-iperf: accept: 192.168.4.2,64210


        Interval Bandwidth
   0-   1 sec       6.83 Mbits/sec
   1-   2 sec       6.58 Mbits/sec
   2-   3 sec       6.94 Mbits/sec
   3-   4 sec       6.30 Mbits/sec
   4-   5 sec       7.22 Mbits/sec
   5-   6 sec       7.14 Mbits/sec
   6-   7 sec       7.41 Mbits/sec
   7-   8 sec       7.26 Mbits/sec
   8-   9 sec       7.35 Mbits/sec
   9-  10 sec       7.17 Mbits/sec
   0-  10 sec       7.02 Mbits/sec
I (29176)-iperf: TCP Socket server is closed.
I (29176)-iperf: iperf done
I (32291)-xf_main: mode=udp-client sip=192.168.4.1:5001,dip=192.168.4.2:5001,interval=1, time=10
I (32295)-iperf: Socket created, sending to 192.168.4.2:5001

        Interval Bandwidth
   0-   1 sec       36.66 Mbits/sec
   1-   2 sec       32.44 Mbits/sec
   2-   3 sec       40.08 Mbits/sec
   3-   4 sec       31.92 Mbits/sec
   4-   5 sec       34.26 Mbits/sec
   5-   6 sec       36.07 Mbits/sec
   6-   7 sec       31.15 Mbits/sec
   7-   8 sec       30.37 Mbits/sec
   8-   9 sec       32.15 Mbits/sec
   9-  10 sec       30.71 Mbits/sec
   0-  10 sec       33.58 Mbits/sec
I (42523)-iperf: UDP Socket client is closed
I (42529)-iperf: iperf done
I (45722)-xf_main: mode=tcp-client sip=192.168.4.1:5001,dip=192.168.4.2:5001,interval=1, time=10
I (45735)-iperf: Successfully connected

        Interval Bandwidth
   0-   1 sec       7.47 Mbits/sec
   1-   2 sec       7.73 Mbits/sec
   2-   3 sec       7.73 Mbits/sec
   3-   4 sec       7.99 Mbits/sec
   4-   5 sec       7.99 Mbits/sec
   5-   6 sec       7.20 Mbits/sec
   6-   7 sec       7.73 Mbits/sec
   7-   8 sec       7.86 Mbits/sec
   8-   9 sec       8.25 Mbits/sec
   9-  10 sec       7.99 Mbits/sec
   0-  10 sec       7.79 Mbits/sec
I (55779)-iperf: TCP Socket client is closed.
I (55779)-iperf: iperf done
I (55862)-xf_main: test done!
```
