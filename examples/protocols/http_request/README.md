# http_request - socket 请求 http 报文示例

## 支持情况

1.  espressif

    1.  esp32

1.  nearlink

    1.  ws63

## 示例简述

本示例演示如何使用 xf_wifi 的站点模式 (sta) 连接到 AP，解析域名，建立 socket 连接，并请求 http 报文。

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

在 `ex_easy_wifi_sta()` 内会启动 STA 并尝试连接到目标 AP，直至获取到 IP 后返回。之后输出获取到的域名服务器、解析 `example.com` 域名、创建 socket连接并连接服务器，之后发送 HTTP 请求并接收服务器响应。最后输出接收到的服务器响应信息。

正常情况下

## 常见问题

无。

## 示例平台差异

无。

## 运行日志

以下是 esp32 的运行日志。

```
I (869)-ex_easy_wifi: STA has started.
E (1882) wifi:sta is connecting, return error
E (2892) wifi:sta is connecting, return error
I (3442) wifi:new:<1,1>, old:<1,0>, ap:<255,255>, sta:<1,1>, prof:6
I (3444) wifi:state: init -> auth (b0)
I (3466) wifi:state: auth -> assoc (0)
I (3475) wifi:state: assoc -> run (10)
I (3504) wifi:connected with ***, aid = 1, channel 1, 40U, bssid = **:**:**:**:**:**
I (3505) wifi:security: WPA2-PSK, phy: bgn, rssi: -43
I (3510) wifi:pm start, type: 0

I (3510) wifi:dp: 1, bi: 102400, li: 3, scale listen interval from 307200 us to 307200 us
I (3520)-ex_easy_wifi: The STA is connected to the AP.
I (3523)-ex_easy_wifi:
ssid:      ***
mac:       **:**:**:**:**:**
channel:   1
authmode:  4
I (3551) wifi:AP's beacon interval = 102400 us, DTIM period = 1
I (4126) wifi:<ba-add>idx:0 (ifx:0, **:**:**:**:**:**), tid:0, ssn:0, winSize:64
I (5019) esp_netif_handlers: sta ip: 192.168.96.47, mask: 255.255.248.0, gw: 192.168.100.1
I (5020)-ex_easy_wifi:
got ip:       192.168.96.47
got gw:       192.168.100.1
got netmask:  255.255.248.0
I (5034)-sta: dns main:     *.*.*.*
I (5034)-sta: dns backup:   *.*.*.*
I (6425)-sta: Response:
HTTP/1.1 200 OK
Age: 476677
Cache-Control: max-age=604800
Content-Type: text/html; charset=UTF-8
Date: Wed, 09 Oct 2024 07:23:28 GMT
Etag: "3147526947+gzip+ident"
Expires: Wed, 16 Oct 2024 07:23:28 GMT
Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
Server: ECAcc (sac/2559)
Vary: Accept-Encoding
X-Cache: HIT
Content-Length: 1256

<!doctype html>
<html>
<head>
    <title>Example Domain</title>

    <meta charset="utf-8" />
    <meta http-equiv="Content-type" content="text/html; charset=utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <style type="text/css">
    body {
        background-color: #f0f0f2;
        margin: 0;
        padding: 0;
        font-family: -apple-system, system-ui, BlinkMacSystemFont, "Segoe UI", "Open Sans", "Helvetica Neue", Helvetica, Arial, sans-serif;

    }
    div {
        width: 600px;
        margin: 5em auto;
        padding: 2em;
        background-color: #fdfdff;
        border-radius: 0.5em;
        box-shadow: 2px 3px 7px 2px rgba(0,0,0,0.02);
    }
    a:link, a:visited {
        color: #38488f;
        text-decoration: none;
    }
    @media (max-width: 700px) {
        div {
            margin: 0 auto;
            width: auto;
        }
    }
    </style>
</head>

<body>
<div>
    <h1>Example Domain</h1>
    <p>This domain is for use in illustrative examples in documents. You may use this
    domain in literature without prior coordination or asking for permission.</p>
    <p><a href="https://www.iana.org/domains/example">More information...</a></p>
</div>
</body>
</html>

```

以下是 ws63 的运行日志。

```
I (274)-ex_easy_wifi: STA has started.
Srv:2047: last scantime > 5s, trigger scan
wifi_sta_connect_fill_scan_param:: Specifying an SSID for scanning
drv_soc_ioctl ioctl_cmd->cmd=14.
hmac_single_hal_device_scan_complete:vap[1] time[762] chan_cnt[13] chan_0[1] back[0] event[6] mode[0]
Scan::vap[1] find bss_num[1] in regdomain, other bss_num[0]
Srv:548:recive event = 1
Srv:216:scan triggered by connect!
Srv:find ssid[PTWSMART] auth type[3] pairwise[1] ft_flag[0]
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
I (1246)-ex_easy_wifi: The STA is connected to the AP.
I (1251)-ex_easy_wifi:
ssid:      ***
mac:       **:**:**:**:**:**
channel:   1
authmode:  4
I (3179)-ex_easy_wifi:
got ip:       192.168.96.42
got gw:       192.168.100.1
got netmask:  255.255.248.0
I (3198)-sta: dns main:     *.*.*.*
I (3198)-sta: dns backup:   *.*.*.*
I (3568)-sta: Response:
HTTP/1.1 200 OK
Accept-Ranges: bytes
Age: 476571
Cache-Control: max-age=604800
Content-Type: text/html; charset=UTF-8
Date: Wed, 09 Oct 2024 07:05:46 GMT
Etag: "3147526947"
Expires: Wed, 16 Oct 2024 07:05:46 GMT
Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
Server: ECAcc (sac/2527)
Vary: Accept-Encoding
X-Cache: HIT
Content-Length: 1256

<!doctype html>
<html>
<head>
    <title>Example Domain</title>

    <meta charset="utf-8" />
    <meta http-equiv="Content-type" content="text/html; charset=utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <style type="text/css">
    body {
        background-color: #f0f0f2;
        margin: 0;
        padding: 0;
        font-family: -apple-system, system-ui, BlinkMacSystemFont, "Segoe UI", "Open Sans", "Helvetica Neue", Helvetica, Arial, sans-serif;

    }
    div {
        width: 600px;
        margin: 5em auto;
        padding: 2em;
        background-color: #fdfdff;
        border-radius: 0.5em;
        box-shadow: 2px 3px 7px 2px rgba(0,0,0,0.02);
    }
    a:link, a:visited {
        color: #38488f;
        text-decoration: none;
    }
    @media (max-width: 700px) {
        div {
            margin: 0 auto;
            width: auto;
        }
    }
    </style>
</head>

<body>
<div>
    <h1>Example Domain</h1>
    <p>This domain is for use in illustrative examples in documents. You may use this
    domain in literature without prior coordination or asking for permission.</p>
    <p><a href="https://www.iana.org/domains/example">More information...</a></p>
</div>
</body>
</html>

```
