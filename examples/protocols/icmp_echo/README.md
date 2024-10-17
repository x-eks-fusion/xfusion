# icmp_echo - socket ping 示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示如何使用 xf_wifi 的站点模式 (sta) 连接到能连接以太网的 ap 后，ping 指定域名。

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

在 `ex_easy_wifi_sta()` 内会启动 STA 并尝试连接到目标 AP，直至获取到 IP 后返回。之后逐个解析域名，并发出 ping 请求。

正常情况下

## 常见问题

无。

## 示例平台差异

无。

## 运行日志

以下是 esp32 的运行日志。

```
I (3790)-sta: target_addr: 93.184.215.14
64 bytes from 93.184.215.14 icmp_seq=1 ttl=52 time=258 ms
64 bytes from 93.184.215.14 icmp_seq=2 ttl=52 time=285 ms
64 bytes from 93.184.215.14 icmp_seq=3 ttl=52 time=207 ms
64 bytes from 93.184.215.14 icmp_seq=4 ttl=52 time=232 ms
64 bytes from 93.184.215.14 icmp_seq=5 ttl=52 time=251 ms

--- 93.184.215.14 ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 1233ms
I (9917)-sta: target_addr: 183.2.172.42
64 bytes from 183.2.172.42 icmp_seq=1 ttl=52 time=67 ms
64 bytes from 183.2.172.42 icmp_seq=2 ttl=52 time=21 ms
64 bytes from 183.2.172.42 icmp_seq=3 ttl=52 time=39 ms
64 bytes from 183.2.172.42 icmp_seq=4 ttl=52 time=73 ms
64 bytes from 183.2.172.42 icmp_seq=5 ttl=52 time=20 ms

--- 183.2.172.42 ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 220ms
I (16054)-sta: target_addr: 121.14.77.201
64 bytes from 121.14.77.201 icmp_seq=1 ttl=52 time=58 ms
64 bytes from 121.14.77.201 icmp_seq=2 ttl=52 time=29 ms
64 bytes from 121.14.77.201 icmp_seq=3 ttl=52 time=36 ms
64 bytes from 121.14.77.201 icmp_seq=4 ttl=52 time=11 ms
64 bytes from 121.14.77.201 icmp_seq=5 ttl=52 time=15 ms

--- 121.14.77.201 ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 149ms
I (22181)-sta: target_addr: 106.63.103.8
64 bytes from 106.63.103.8 icmp_seq=1 ttl=50 time=53 ms
64 bytes from 106.63.103.8 icmp_seq=2 ttl=50 time=52 ms
64 bytes from 106.63.103.8 icmp_seq=3 ttl=50 time=49 ms
64 bytes from 106.63.103.8 icmp_seq=4 ttl=50 time=47 ms
From 106.63.103.8 icmp_seq=5 timeout

--- 106.63.103.8 ping statistics ---
5 packets transmitted, 4 received, 19% packet loss, time 2200ms
```

以下是 ws63 的运行日志。

```
I (2678)-sta: target_addr: 93.184.215.14
64 bytes from 93.184.215.14 icmp_seq=1 ttl=52 time=178 ms
64 bytes from 93.184.215.14 icmp_seq=2 ttl=52 time=166 ms
xo update temp:4,diff:0,xo:0x3083c
64 bytes from 93.184.215.14 icmp_seq=3 ttl=52 time=162 ms
I (5203)-ex_easy_wifi:
got ip:       192.168.96.23
got gw:       192.168.100.1
got netmask:  255.255.248.0
64 bytes from 93.184.215.14 icmp_seq=4 ttl=52 time=168 ms
64 bytes from 93.184.215.14 icmp_seq=5 ttl=52 time=193 ms

--- 93.184.215.14 ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 867ms
I (8809)-sta: target_addr: 183.2.172.42
64 bytes from 183.2.172.42 icmp_seq=1 ttl=52 time=26 ms
64 bytes from 183.2.172.42 icmp_seq=2 ttl=52 time=21 ms
64 bytes from 183.2.172.42 icmp_seq=3 ttl=52 time=21 ms
64 bytes from 183.2.172.42 icmp_seq=4 ttl=52 time=48 ms
64 bytes from 183.2.172.42 icmp_seq=5 ttl=52 time=23 ms

--- 183.2.172.42 ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 139ms
I (14880)-sta: target_addr: 121.14.77.221
64 bytes from 121.14.77.221 icmp_seq=1 ttl=246 time=17 ms
64 bytes from 121.14.77.221 icmp_seq=2 ttl=246 time=24 ms
64 bytes from 121.14.77.221 icmp_seq=3 ttl=246 time=15 ms
64 bytes from 121.14.77.221 icmp_seq=4 ttl=246 time=29 ms
64 bytes from 121.14.77.221 icmp_seq=5 ttl=246 time=19 ms

--- 121.14.77.221 ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 104ms
I (20956)-sta: target_addr: 106.63.103.8
64 bytes from 106.63.103.8 icmp_seq=1 ttl=50 time=55 ms
64 bytes from 106.63.103.8 icmp_seq=2 ttl=50 time=57 ms
64 bytes from 106.63.103.8 icmp_seq=3 ttl=50 time=56 ms
64 bytes from 106.63.103.8 icmp_seq=4 ttl=50 time=49 ms
64 bytes from 106.63.103.8 icmp_seq=5 ttl=50 time=54 ms

--- 106.63.103.8 ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 271ms
```
