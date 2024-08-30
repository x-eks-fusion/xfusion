# BLE GATT 服务端例程

## 此例程将会顺序执行以下步骤：
  - 服务端的初始化（使能BLE、注册事件回调）  
  - 设置本地名称、外观 
  - 注册服务端 app profile
  - 服务描述的添加
  - 启动服务
  - 广播参数、广播数据的设置 （把设备名加入广播数据中）
  - 广播开启  
  - 等待客户端连上后会等待客户端发起读或写请求，并根据请求是否需要响应去进行回应

## log 输出
```text

......

I (158)-sample_gatts: xf ble gatt server sample

I (1174)-port_ble_gatts: register_app_profile: len:2 uuid(16):0
I (1178)-port_ble_gatts: service_start_cb: app_id:1 srv_hdl: 1 status: 0

I (1205)-port_ble_gatts: service_start_cb: app_id:1 srv_hdl: 5 status: 0

I (1212)-port_ble_gatts: service uuid:0X1234
I (1217)-port_ble_gatts: service_added_cb:app_id:1,service_uuid:0X34 0X12,service_handle:14 status:0
I (1226)-port_ble_gatts: gatts_add_chara[0]: uuid:0XEF 0XCD
I (1233)-port_ble_gatts: chara_added_cb:app_id:1,service_handle:14,chara_uuid:0XEF 0XCD,chara_handle:15,chara_val_handle:16,status:0
I (1245)-port_ble_gatts: chara_added_cb 2222:chara_handle:15
I (1251)-port_ble_gatts: add_service chara_set[0]
I (1257)-port_ble_gatts: add_service time cnt:0
I (1262)-port_ble_gatts: service_start_cb: app_id:1 srv_hdl: 14 status: 0

I (1269)-port_ble_gap: adv_data_size_all:15
I (1292)-sample_gatts: CONFIG ADV CMPL
I (1302)-port_ble_gatts: adv enable adv_id: 1, status:1

I (1306)-sample_gatts: STAR ADV CMPL
I (2405)-port_ble_gatts: connect state change conn_id: 0, status: 1, pair_status:1, addr adce3863 disc_reason 6338

I (2415)-sample_gatts: EV:mtu changed:app_id:1,conn_id:0,mtu_size:23
I (2425)-port_ble_gatts: ble_gap_terminate_adv_cb status: 0

I (3762)-sample_gatts: EV:RECV READ_REQ:app_id:1,conn_id:0,need_rsp:1,attr_handle:7
update send report handle: pre handle:ffff, current:0 
I (3862)-sample_gatts: EV:RECV READ_REQ:app_id:1,conn_id:0,need_rsp:1,attr_handle:7
I (3962)-sample_gatts: EV:RECV READ_REQ:app_id:1,conn_id:0,need_rsp:1,attr_handle:7
I (4312)-sample_gatts: EV:RECV WRITE_REQ:app_id:1,conn_id:0,need_rsp:0,attr_handle:9
MEMORY START ADDRESS: 00A3BDF8, OUTPUT 10 BYTES.
------------------------------------------------------------------------
 OFS   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F | ASCII
------------------------------------------------------------------------
0000: 49 20 4D 20 47 41 54 54 43 00                   | I M GATTC.
------------------------------------------------------------------------
I (4412)-port_ble_gatts: connect state change conn_id: 0, status: 0, pair_status:1, addr adce3863 disc_reason 6338

I (4422)-sample_gatts: EV:peer disconnect:app_id:1,conn_id:0,reason:19,addr_type:0,addr:63:38:CE:AD:38:63
I (4432)-sample_gatts: It will restart ADV
I (4443)-port_ble_gatts: adv enable adv_id: 1, status:1

......

```