# SLE 客户端例程

## 此例程将会顺序执行以下步骤：
  - 客户端的初始化（使能BLE、注册事件回调） 
  - 注册客户端 app profile 
  - 设置扫描参数
  - 扫描开启
  - 扫描到广播后，通过对广播数据解析进行过滤，仅连接包含设备名的广播的服务端
  - 连接服务端后进行服务的发现（包括其下的特征、特征描述符等）
  - 服务发现完成后发起读写请求
  - 最后写成功后与服务端断开连接

## log 输出
```text

......

I (158)-sample_ssapc: XF SLE SSAP CLient
I (967)-sample_ssapc: >> register app:aap_id:1
I (972)-sample_ssapc: >> STAR seek CMPL
I (1009)-sample_ssapc: > target name:XF_SSAPS
I (1068)-sample_ssapc: EV:connect:conn_id:0,33:22:22:66:11:22
I (1109)-port_sle_ssapc: port_ssapc_find_struct_cb
I (1114)-sample_ssapc: service:uuid:0XABCD,hdl[1,2]
I (1118)-sample_ssapc: >> request write data,app_id:1,conn_id:0,hdl:1
I (1234)-sample_ssapc: EV:WRITE confirm:conn_id:0,hdl:1
I (1241)-sample_ssapc: >> request read,app_id:1,conn_id:0,hdl:1
I (1272)-sample_ssapc: EV:READ confirm:conn_id:0,hdl:0
MEMORY START ADDRESS: 00A3FB78, OUTPUT 14 BYTES.
--------------------------------------------------------------------------------------------------------
 OFS   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F | ASCII
--------------------------------------------------------------------------------------------------------
0000: 49 20 4D 20 53 53 41 50 53 20 52 53 50 00       |  I     M     S  S  A  P  S     R  S  P \0 
--------------------------------------------------------------------------------------------------------

......

```