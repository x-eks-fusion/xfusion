# BLE GATT 客户端例程

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

I (158)-sample_gattc: xf ble gatt client sample
I (1169)-sample_gattc: SET scan param CMPL
I (1172)-sample_gattc: STAR scan CMPL
I (1202)-sample_gattc: EV:scan_result:target local name:XF_GATTS

I (1256)-port_ble_gattc: connect state change cb:conn_id: 0,status: 1,pair_status:1,addr adce3863,disc_reason:6338

I (1266)-sample_gattc: EV:connect:s_conn_id:0,addr_type:0,addr:63:38:CE:AD:38:63
I (1675)-sample_gattc: discover service:00A264C0 00000000
I (1763)-port_ble_gattc: service_found_cb: handle[1,4],status:0
I (1764)-port_ble_gattc: service_found_cb: handle[5,13],status:0
I (1771)-port_ble_gattc: service_found_cb: handle[14,16],status:0
I (1873)-sample_gattc: discover chara:service_found_set cnt:3
I (2073)-sample_gattc: service[2]:uuid(16):[0X1234],handle[14,0],chara[0]:0XCDEF
I (2373)-sample_gattc: service[1]:uuid(16):[0X18],handle[5,0],chara[3]:0XA62A
I (2375)-sample_gattc: service[1]:uuid(16):[0X18],handle[5,0],chara[2]:0X42A
I (2383)-sample_gattc: service[1]:uuid(16):[0X18],handle[5,0],chara[1]:0X12A
I (2391)-sample_gattc: service[1]:uuid(16):[0X18],handle[5,0],chara[0]:0X2A
I (2573)-sample_gattc: service[0]:uuid(16):[0X118],handle[1,0],chara[0]:0X52A
I (2575)-sample_gattc: REQ READ(uuid) and WRITE(handle):s_app_id:24,s_conn_id:1service_uuid(16):0,chara_uuid(16):0X2A,
I (2663)-sample_gattc: EV:read CMPL:handle:7
MEMORY START ADDRESS: 00A3BC00, OUTPUT 22 BYTES.
------------------------------------------------------------------------
 OFS   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F | ASCII
------------------------------------------------------------------------
0000: 58 46 5F 47 41 54 54 53 00 00 00 00 00 00 00 00 | XF_GATTS........
0001: 00 00 00 00 00 00                               | ......
------------------------------------------------------------------------
I (2863)-sample_gattc: EV:read CMPL:handle:7
MEMORY START ADDRESS: 00A3BC00, OUTPUT 13 BYTES.
------------------------------------------------------------------------
 OFS   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F | ASCII
------------------------------------------------------------------------
0000: 52 45 51 5F 52 45 41 44 20 52 53 50 00          | REQ_READ RSP.
------------------------------------------------------------------------
I (3213)-sample_gattc: EV:write CMPL:handle:9,it will disconnect
I (3264)-port_ble_gattc: connect state change cb:conn_id: 0,status: 0,pair_status:1,addr adce3863,disc_reason:6338

......

```