# static_ip_softap - wifi 热点且设置静态 IP 示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示如何使用 xf_wifi 的接入点模式 (ap)，并且设置静态 IP.

板 A 烧入此程序后，在板 B 烧入对应静态 IP 的 [station](../station/README.md) 示例；两板同时启动，待板 B 连接到板 A 后，手机连接板 A 的 AP，设置静态 IP `192.168.6.3`，并 ping 板 B 静态 IP 地址(默认`192.168.6.2`)，无丢失则为设置成功。

下面为手机 ping `192.168.6.2` 的日志。

```
PING 192.168.6.2 (192.168.6.2): 56 data bytes
64 bytes from 192.168.6.2: icmp_seq=0 ttl=255 time=144 ms
64 bytes from 192.168.6.2: icmp_seq=1 ttl=255 time=15.1 ms
64 bytes from 192.168.6.2: icmp_seq=2 ttl=255 time=15.3 ms
64 bytes from 192.168.6.2: icmp_seq=3 ttl=255 time=8.93 ms
64 bytes from 192.168.6.2: icmp_seq=4 ttl=255 time=21.3 ms
64 bytes from 192.168.6.2: icmp_seq=5 ttl=255 time=15.0 ms
64 bytes from 192.168.6.2: icmp_seq=6 ttl=255 time=6.97 ms
64 bytes from 192.168.6.2: icmp_seq=7 ttl=255 time=6.63 ms
64 bytes from 192.168.6.2: icmp_seq=8 ttl=255 time=5.34 ms
64 bytes from 192.168.6.2: icmp_seq=9 ttl=255 time=6.36 ms
--- 192.168.6.2 ping statistics ---
10 packets transmitted, 10 packets received, 0.00% packet loss
round-trip min/avg/max/stddev = 5.34/24.49/144.00/40.15 ms
```

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

运行该示例的芯片会启动 AP 和 DHCP 服务器。在 AP 启动、停止、有 STA 接入或离开时输出日志。

## 常见问题

无。

## 示例平台差异

此示例对以下目标无明显差异：

1. esp32
1. ws63

## 运行日志

以下是 esp32 的运行日志。

其中 `**:**:**:**:**:**` 为手机的 mac 地址。

```
I (841)-ap: AP has started.
I (241449) wifi:new:<6,1>, old:<6,1>, ap:<6,1>, sta:<255,255>, prof:6
I (241451) wifi:station: fc:f5:c4:31:1b:fc join, AID=1, bgn, 40U
I (241474)-ap: station fc:f5:c4:31:1b:fc join
I (309921) wifi:new:<6,1>, old:<6,1>, ap:<6,1>, sta:<255,255>, prof:6
I (309922) wifi:station: **:**:**:**:**:** join, AID=2, bgn, 20
I (309963)-ap: station **:**:**:**:**:** join
I (310055) wifi:<ba-add>idx:2 (ifx:1, **:**:**:**:**:**), tid:0, ssn:0, winSize:64
I (368981) wifi:<ba-add>idx:3 (ifx:1, fc:f5:c4:31:1b:fc), tid:0, ssn:0, winSize:64
```

以下是 ws63 的运行日志。

其中 `**:**:**:**:**:**` 为手机的 mac 地址。

```
I (230)-ap: AP has started.
xo update temp:4,diff:0,xo:0x30b3c
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
I (38137)-ap: station a8:00:73:9e:24:7e join
drv_soc_ioctl ioctl_cmd->cmd=2.

drv_soc_ioctl ioctl_cmd->cmd=2.
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
Srv:2781:sta_num[2]!!
I (52040)-ap: station **:**:**:**:**:** join
```
