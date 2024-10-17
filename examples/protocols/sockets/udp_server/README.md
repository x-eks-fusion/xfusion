# sockets:udp_server - wifi socket udp 服务端通信示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示启动 wifi ap 后启动 socket udp 服务端，之后与客户端通信.

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

本示例启动 ap 等到 sta 连接后，创建 udp 服务端(默认端口号为 `3333`)接收任意地址的数据，接收到数据后通过 log 输出，之后向来源发送 `This is a message from server.`，以此循环。

## 常见问题

无。

## 示例平台差异

此示例对以下目标无明显差异：

1. esp32
1. ws63

## 运行日志

以下是 esp32 的运行日志。

```
I (828)-ex_easy_wifi: AP has started.
I (5275) wifi:new:<6,1>, old:<6,1>, ap:<6,1>, sta:<255,255>, prof:6
I (5276) wifi:station: aa:bb:cc:dd:ee:ff join, AID=1, bgn, 40U
I (5310)-ex_easy_wifi: station aa:bb:cc:dd:ee:ff join
I (5347) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (5348)-ex_easy_wifi: Assign ip 192.168.4.2 to site aa:bb:cc:dd:ee:ff
I (5431)-ap: Socket created
I (5432)-ap: Socket bound, port 3333
I (5432)-ap: Waiting for data.
I (6363)-ap: Received 27 bytes from 192.168.4.2:
I (6363)-ap: This is a message from client.
I (6364) wifi:<ba-add>idx:2 (ifx:1, aa:bb:cc:dd:ee:ff), tid:0, ssn:0, winSize:64
I (6470)-ap: Waiting for data.
I (6477)-ap: Received 27 bytes from 192.168.4.2:
I (6477)-ap: This is a message from client.
I (6578)-ap: Waiting for data.
I (6587)-ap: Received 27 bytes from 192.168.4.2:
I (6588)-ap: This is a message from client.
I (6688)-ap: Waiting for data.
I (6700)-ap: Received 27 bytes from 192.168.4.2:
I (6700)-ap: This is a message from client.
...
```

以下是 ws63 的运行日志。

```
I (281)-ex_easy_wifi: AP has started.
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
I (1199)-ex_easy_wifi: station e2:00:73:95:66:27 join
I (1619)-ex_easy_wifi: Assign ip 192.168.4.2 to site e2:00:73:95:66:27
I (1699)-ap: Socket created
I (1700)-ap: Socket bound, port 3333
I (1701)-ap: Waiting for data.
I (2643)-ap: Received 27 bytes from 192.168.4.2:
I (2643)-ap: This is a message from client.
I (2739)-ap: Waiting for data.
I (2758)-ap: Received 27 bytes from 192.168.4.2:
...
```
