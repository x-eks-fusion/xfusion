# station - wifi 站点示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示如何使用 xf_wifi 的站点模式 (sta) 快速连接到目标 AP，以及在 AP 下电后重启，本机 sta 断开后重连。

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

运行该示例的芯片会启动 STA 和 DHCP 客户端。在 STA 启动、停止、连接到目标 AP 或从目标 AP 断连时输出日志；在 AP 的 DHCP 服务器分配 IP 给本机后会输出本机获得的 IP 地址。

## 常见问题

1.  esp32

    对于 esp32 平台出现如下日志：

    ```
    I (1866)-sta: STA attempts to connect.
    E (1887) wifi:sta is connecting, return error
    ```

    是因为 esp32 内部有自动重连机制，会自动尝试连接。ESP-IDF v5.0.5 没有提供查询`连接中`状态的 API。

1.  wifi 回调函数

    建议不要在 wifi 回调函数中连接，在 ws63 平台中，如果在 wifi 回调函数中连接可能会导致递归调用连接函数。

## 示例平台差异

1.  wifi 断开检测时间

    esp32 和 ws63 的对 wifi 断开检测时间不一致，esp32 下，只要 ap 重启，esp32 sta 能很快(0~3s)检测到断开；而 ws63 则需要 30s 以上才能检测到。

1.  mac 地址

    esp32 的 mac 地址固定，而 ws63 mac 地址随机（可能烧写 efuse 后会固定）。

## 运行日志

以下是 esp32 的运行日志。

```
I (834)-sta: STA has started.
I (863)-sta: _wifi_ntask start!
I (1866)-sta: STA attempts to connect.
E (1887) wifi:sta is connecting, return error
I (2888)-sta: STA attempts to connect.
E (2909) wifi:sta is connecting, return error
I (3278) wifi:new:<6,1>, old:<1,0>, ap:<255,255>, sta:<6,1>, prof:6
I (3281) wifi:state: init -> auth (b0)
I (3287) wifi:state: auth -> assoc (0)
I (3293) wifi:Association refused temporarily, comeback time 1500 (TUs)
I (3910)-sta: STA attempts to connect.
E (3931) wifi:sta is connecting, return error
I (4830) wifi:state: assoc -> assoc (0)
I (4840) wifi:state: assoc -> run (10)
I (4858) wifi:connected with myssid, aid = 1, channel 6, 40U, bssid = 08:3a:f2:bf:21:d9
I (4859) wifi:security: WPA2-PSK, phy: bgn, rssi: -10
I (4865) wifi:pm start, type: 1

I (4865) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (4875)-sta: The STA is connected to the AP.
I (4876)-sta:
ssid:      myssid
mac:       08:3a:f2:bf:21:d9
channel:   6
authmode:  4
I (4932)-sta: STA attempts to connect.
I (4942) wifi:dp: 2, bi: 102400, li: 4, scale listen interval from 307200 us to 409600 us
I (4943) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (5877) esp_netif_handlers: sta ip: 192.168.4.2, mask: 255.255.255.0, gw: 192.168.4.1
I (5878)-sta:
got ip:       192.168.4.2
got gw:       192.168.4.1
got netmask:  255.255.255.0
```

以下是 ws63 的运行日志。

```
I (223)-sta: STA has started.
I (240)-sta: _wifi_ntask start!
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
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=47.
I (1260)-sta: STA attempts to connect.
drv_soc_ioctl ioctl_cmd->cmd=6.

drv_soc_ioctl ioctl_cmd->cmd=5.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=6.
drv_soc_ioctl ioctl_cmd->cmd=5.
drv_soc_ioctl ioctl_cmd->cmd=1.
drv_soc_ioctl ioctl_cmd->cmd=3.
drv_soc_ioctl ioctl_cmd->cmd=1.
drv_soc_ioctl ioctl_cmd->cmd=1.
+NOTICE:CONNECTED
drv_soc_ioctl ioctl_cmd->cmd=6.
Srv:548:recive event = 2
I (2213)-sta: The STA is connected to the AP.
I (2217)-sta:
ssid:      myssid
mac:       08:3a:f2:bf:21:d9
channel:   6
authmode:  4
xo update temp:4,diff:0,xo:0x3083c
I (4631)-sta:
got ip:       192.168.4.4
got gw:       192.168.4.1
got netmask:  255.255.255.0

```
