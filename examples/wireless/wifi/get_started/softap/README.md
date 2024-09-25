# softap - wifi 热点示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示如何使用 xf_wifi 的接入点模式 (ap).

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

运行该示例的芯片会启动 AP 和 DHCP 服务器。在 AP 启动、停止、有 STA 接入或离开时输出日志；在给 STA 的 DHCP 客户端分配 IP 后会输出分配的 IP 地址。

## 常见问题

无。

## 示例平台差异

此示例对以下目标无明显差异：

1. esp32
1. ws63

## 运行日志

以下是 esp32 的运行日志。

```
I (838)-ap: AP has started.
I (3306) wifi:new:<6,1>, old:<6,1>, ap:<6,1>, sta:<255,255>, prof:6
I (3307) wifi:station: fc:f5:c4:31:1b:fc join, AID=1, bgn, 40U
I (3335)-ap: station fc:f5:c4:31:1b:fc join
I (3375) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (3376)-ap: Assign ip 192.168.4.2 to site fc:f5:c4:31:1b:fc
I (33047) wifi:new:<6,1>, old:<6,1>, ap:<6,1>, sta:<255,255>, prof:6
I (33048) wifi:station: c8:00:73:9a:f8:61 join, AID=2, bgn, 40U
I (33110)-ap: station c8:00:73:9a:f8:61 join
I (33895) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.3
I (33896)-ap: Assign ip 192.168.4.3 to site c8:00:73:9a:f8:61
I (34899) wifi:<ba-add>idx:2 (ifx:1, c8:00:73:9a:f8:61), tid:0, ssn:0, winSize:64
```

以下是 ws63 的运行日志。

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
I (4360)-ap: station d6:00:73:47:d1:e1 join
I (4790)-ap: Assign ip 192.168.4.2 to site d6:00:73:47:d1:e1
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
I (9339)-ap: station fc:f5:c4:31:1b:fc join
I (9410)-ap: Assign ip 192.168.4.3 to site fc:f5:c4:31:1b:fc
```
