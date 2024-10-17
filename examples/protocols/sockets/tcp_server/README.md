# sockets:tcp_server - wifi socket tcp 服务端通信示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示启动 wifi ap 后启动 socket tcp 服务端并监听客户端连接，之后与客户端相互收发数据.

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

本示例启动 ap 等到 sta 连接后，创建 tcp 服务端(默认端口号为 `3333`)接收任意地址的数据，接收到数据后通过 log 输出，之后向来源发送 `This is a message from server.`，以此循环。

## 常见问题

无。

## 示例平台差异

此示例对以下目标无明显差异：

1. esp32
1. ws63

## 运行日志

以下是 esp32 的运行日志。

```I (281)-ex_easy_wifi: AP has started.
I (5870) wifi:new:<6,1>, old:<6,1>, ap:<6,1>, sta:<255,255>, prof:6
I (5871) wifi:station: aa:bb:cc:dd:ee:ff join, AID=1, bgn, 40U
I (5349)-ex_easy_wifi: station aa:bb:cc:dd:ee:ff join
I (5936) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (5385)-ex_easy_wifi: Assign ip 192.168.4.2 to site aa:bb:cc:dd:ee:ff
I (5477)-ap: Socket created
I (5478)-ap: Socket bound, port 3333
I (5479)-ap: Socket listening
I (7032) wifi:<ba-add>idx:2 (ifx:1, aa:bb:cc:dd:ee:ff), tid:0, ssn:0, winSize:64
I (7495)-ap: Received 30 bytes from 192.168.4.2: "This is a message from client."
I (7498)-ap: send     30 bytes to   192.168.4.2: "This is a message from server."
I (8506)-ap: Received 30 bytes from 192.168.4.2: "This is a message from client."
I (8508)-ap: send     30 bytes to   192.168.4.2: "This is a message from server."
I (9518)-ap: Received 30 bytes from 192.168.4.2: "This is a message from client."
I (9520)-ap: send     30 bytes to   192.168.4.2: "This is a message from server."
I (10534)-ap: Received 30 bytes from 192.168.4.2: "This is a message from client."
I (10536)-ap: send     30 bytes to   192.168.4.2: "This is a message from server."
I (11555)-ap: Received 30 bytes from 192.168.4.2: "This is a message from client."
I (11557)-ap: send     30 bytes to   192.168.4.2: "This is a message from server."
I (12576)-ap: Received 30 bytes from 192.168.4.2: "This is a message from client."
I (12578)-ap: send     30 bytes to   192.168.4.2: "This is a message from server."
...
```

以下是 ws63 的运行日志。

```
I (232)-ex_easy_wifi: AP has started.
xo update temp:3,diff:0,xo:0x30b3c
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
I (6493)-ex_easy_wifi: station aa:bb:cc:dd:ee:ff join
I (6730)-ex_easy_wifi: Assign ip 192.168.4.2 to site aa:bb:cc:dd:ee:ff
I (6740)-ap: Socket created
I (6740)-ap: Socket bound, port 3333
I (6742)-ap: Socket listening
I (9716)-ap: Received 30 bytes from 192.168.4.2: "This is a message from client."
I (9721)-ap: send     30 bytes to   192.168.4.2: "This is a message from server."
I (10736)-ap: Received 30 bytes from 192.168.4.2: "This is a message from client."
I (10740)-ap: send     30 bytes to   192.168.4.2: "This is a message from server."
I (11755)-ap: Received 30 bytes from 192.168.4.2: "This is a message from client."
I (11759)-ap: send     30 bytes to   192.168.4.2: "This is a message from server."
xo update temp:4,diff:0,xo:0x30b3c
I (12769)-ap: Received 30 bytes from 192.168.4.2: "This is a message from client."
I (12773)-ap: send     30 bytes to   192.168.4.2: "This is a message from server."
I (13777)-ap: Received 30 bytes from 192.168.4.2: "This is a message from client."
I (13782)-ap: send     30 bytes to   192.168.4.2: "This is a message from server."
...
```
