/**
 * @file xf_ble_sm_types.h
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-08-06
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_BLE_SM_TYPES_H__
#define __XF_BLE_SM_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils.h"
#include "xf_ble_gap_types.h"

#if XF_BLE_IS_ENABLE || defined(__DOXYGEN__)

/**
 * @cond (XFAPI_USER || XFAPI_PORT || XFAPI_INTERNAL)
 * @ingroup group_xf_wal
 * @defgroup group_xf_wal_ble xf_ble
 * @brief xf_ble 是对 ble 的封装。
 * @endcond
 */

/**
 * @cond (XFAPI_USER || XFAPI_PORT)
 * @addtogroup group_xf_wal_ble
 * @endcond
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief AuthReq (Authentication requirements) 鉴权请求。表示设备是否需要绑定，是否需要支持防止中间人攻击，是否支持安全配对等
 * 
 * @details 
 * @code 一些涉及到的专业名词或缩写:
 * - LESC (LE secure connections): 又称SC，蓝牙 4.2 引入的一种新的密钥生成方式和验证方式，
 *      SC 通过基于椭圆曲线的 Diffie-Hellman 密钥交换算法来生成设备A和B的共享密钥，
 *      此密钥生成过程中需要用到公私钥对，以及其他的密码算法库。
 *      LESC 同时还规定了相应的通信协议以生成该密钥，并验证该密钥。
 *      需要注意的是 LESC 对 paring 的其他方面也会产生一定的影响，所以我们经常会把 LESC 看成是一种新的配对方式。
 * - Legacy paring: 在 LESC 引入之前的密钥生成方式，称为 legacy paring，换句话说，legacy paring 是相对 LESC 来说的，不支持 LESC 的配对即为 legacy paring（legacy 配对）。
 * - TK (Temporary Key，临时密钥): legacy paring 里面的概念，
 *      如果采用 just work 配对方式，TK 就是为全 0 ；
 *      如果采用 passkey 配对方式，TK 就是 passkey；如果采用 OOB 配对方式，TK 就是 OOB 里面的信息。
 * - STK (short term key ，短期密钥): legacy 配对里面的概念，STK是通过TK推导出来的，通过 TK 对设备 A 和 B 的随机数进行加密，即得到 STK。
 * - LTK (long term key ，长期密钥): legacy 配对和 LESC 配对都会用到 LTK ，如前所述， LTK 是用来对未来的连接进行加密和解密用的。
 *      Legacy 配对中的 LTK 由从设备根据相应的算法自己生成的（ LTK 生成过程中会用到 EDIV (分散因子) 和 Rand (随机数) )，然后通过蓝牙空中包传给主机。
 *      LESC 配对过程中，先通过 Diffie-Hellman 生成一个共享密钥，然后这个共享密钥再对设备A和B的蓝牙地址和随机数进行加密，
 *      从而得到 LTK ， LTK 由设备A和B各自同时生成，因此 LTK 不会出现在 LESC 蓝牙空中包中，大大提高了蓝牙通信的安全性。
 * - IRK (Identity Resolving Key ，蓝牙设备地址解析密钥): 用于生成和解析私有解析地址。
 *      由于地址为可解析私有类型时，地址会随着时间变化，要确定这些地址是否来自同一个设备需要 IRK 通过解析变化的地址的规律。
 *      IRK 的生成有多种方式，Key Size 在 7-16 octets (bytes) 。IRK 一般是在设备出厂的时候按照一定要求自动生成。
 *      一个 Central (Master) 从 Peripheral (Slave) 那里得到 Peripheral (Slave) 的 IRK ，就可以解析 Peripheral (Slave) 的 random address ；
 *      一个 Peripheral (Slave) 从 Central (Master) 那里得到 Central (Master) 的 IRK，就可以解析 Central (Master) 的random address了。
 * - Identity Address (设备唯一地址): 蓝牙设备地址类型有 4 种: public，random static， private resolvable，random unresolved 。
 *      如果设备不支持 privacy ，那么 identity address 就等于 public 或者 random static 设备地址。
 *      如果设备支持 privacy，即使用 private resolvable 蓝牙设备地址，在这种情况下，虽然其地址每隔一段时间会变化一次，
 *      但是 identity address 仍然保持不变，其取值还是等于内在的 public 或者 random static 设备地址。
 *      Identity Address 和 IRK 都可以用来唯一标识一个蓝牙设备。
 * 
 * @endcode
 * 
 * @code 鉴权请求标志 (Authentication requirements flags)
 * LSB                                                                  MSB
 *  ┌───────────────┬─────────┬─────────┬──────────┬─────────┬───────────┐
 *  │ Bonding_Flags     MITM      SC      Keypress     CT2        RFU
 *  │   (2 bits)      (1 bit)   (1 bit)   (1 bit)    (1 bit)    (2 bits)
 *  └───────────────┴─────────┴─────────┴──────────┴─────────┴───────────┘
 *      绑定标志      中间人位段  安全连接   
 * 
 * Bonding_Flags (2 bits)：绑定标志，指示发起设备 (initiating device) 的绑定类型。
 *  ┌────────────── ┬─────────────┐
 *  │ Bonding_Flags     绑定类型
 *  │   b[1:0]                     
 *  ├───────────────┼─────────────┤
 *  │      00            无绑定    
 *  ├───────────────┼─────────────┤
 *  │      01             绑定   
 *  ├───────────────┼─────────────┤
 *  │      ...        保留未来使用 
 *  └───────────────┴─────────────┘
 * 
 *  MITM (1 bit): 中间人攻击标志 (Man in the MiddleAttack)。
 *  - 如果设备正在请求 MITM 保护，则其设置为1，否则应设置为0。
 *  - 设备将 MITM 标志设置为1，以便在使用 LE 传统 (legacy) 配对时请求 STK 的认证安全属性，在使用 LE 安全连接时请求 LTK 。
 * 
 *  SC (secure connect)(1 bit): 表示安全配对，设置为 1 以请求 LE 安全连接配对。
 *  - 可能产生的配对机制是，如果两个设备都支持 LE 安全连接 (LESC)，则使用 LE 安全连接，
 *  - 否则使用 LE 传统配对 (Legacy paring)。所以这个标志是确定第 2 阶段配对方法的指标。
 * 
 *  KP (keypress)(1 bit): 仅在 Passkey Entry 协议中使用，在其他协议中被忽略。
 *  - 当双方都将该字段设置为1时，keypress 通知应被使用 SMP Pairing Keypress Notification PDUs (配对按键通知 PDU) 生成并发送。
 *  - Passkey Entry 协议是 Legacy Pairing 和 Secure Connection 的典型配对方法。
 *  - Passkey: 又称 pin 码，是指用户在键盘中输入的一串数字，以达到认证设备的目的。低功耗蓝牙的 passkey 必须为6位。
 *  
 * CT2 (1 bit): 略
 * 
 * @endcode
 * 
 * @code    
 * @endcode
 * 
 */
typedef uint8_t xf_ble_sm_authen_req_t;         /*!< combination of the above bit pattern */

#define XF_BLE_SM_AUTHEN_REQ_NO_BOND        (0 << 0)    /*!< 0 no bonding */
#define XF_BLE_SM_AUTHEN_REQ_BOND           (1 << 0)    /*!< 1 << 0 device in the bonding with peer */
#define XF_BLE_SM_AUTHEN_REQ_MITM           (1 << 2)    /*!< 1 << 2 MITM (防) 中间人攻击 */
#define XF_BLE_SM_AUTHEN_REQ_SC             (1 << 3)    /*!< 1 << 3 安全连接 SC secure connection */
#define XF_BLE_SM_AUTHEN_REQ_BOND_MITM      (XF_BLE_SM_AUTHEN_REQ_BOND | XF_BLE_SM_AUTHEN_REQ_MITM)     /*!< 0101 绑定，带 MITM (防) 中间人攻击 */
#define XF_BLE_SM_AUTHEN_REQ_SC_BOND        (XF_BLE_SM_AUTHEN_REQ_BOND | XF_BLE_SM_AUTHEN_REQ_SC)       /*!< 1001 安全连接，带绑定 */
#define XF_BLE_SM_AUTHEN_REQ_SC_MITM        (XF_BLE_SM_AUTHEN_REQ_MITM | XF_BLE_SM_AUTHEN_REQ_SC)       /*!< 1100 安全连接，带 MITM */
#define XF_BLE_SM_AUTHEN_REQ_SC_MITM_BOND   (XF_BLE_SM_AUTHEN_REQ_MITM | XF_BLE_SM_AUTHEN_REQ_SC | XF_BLE_SM_AUTHEN_REQ_BOND)   /*!< 1101 安全连接，带绑定及 MITM */


/**
 * @brief 本端 IO (人机交互) 能力 (IO capabilities)：表示设备的输入和输出能力。
 * @note 输入是按键、键盘，输出是显示数字用的界面。
 * @note IO能力交换的目的是确定在安全简单配对的认证阶段1阶段中使用的认证算法。
 * @note 手机上弹出的不同的配对界面能对应到以下的 IO 能力
 * 
 * @details IO 能力详情
 * @code
 * ┌──────────────────────┬────────────────────────────────────────────────┐
 * │      输出能力                        描述
 * │ (Output Capability)                     
 * ├──────────────────────┼────────────────────────────────────────────────┤
 * │    No output           设备无法显示 (indicate) 或通信6位十进制数字         
 * ├──────────────────────┼────────────────────────────────────────────────┤
 * │   Numeric output       设备能够显示或通信一个6位数字的十进制数字 
 * └──────────────────────┴────────────────────────────────────────────────┘
 * 
 * ┌──────────────────────┬───────────────────────────────────────┐
 * │      输入能力                        描述
 * │ (Input Capability)                     
 * ├──────────────────────┼───────────────────────────────────────┤
 * │     No input           设备不能指示“是”或“否”        
 * ├──────────────────────┼───────────────────────────────────────┤
 * │     Yes / No           设备至少有两个按钮，能映射到“是”和“否”，
 * │                        或者有机制可以指示“是”或“否”        
 * ├──────────────────────┼───────────────────────────────────────┤
 * │     Keyboard           设备有一个数字键盘，可输入数字“0”到“9”，
 * │                        并进行确认。
 * │                        设备也有两个按钮，能映射到“是”和“否”，
 * │                        或者有机制可以指示“是”或“否”        
 * └──────────────────────┴───────────────────────────────────────┘
 * 
 * ┌────────────────────┬────────────────────────────────────┐
 * │                                 本端 输出 能力
 * │                    ├──────────────────┬─────────────────┤
 * │                         No output       Numeric output       
 * ├──────┬─────────────╔══════════════════╦═════════════════╗
 * │ 本端     No input     NoInputNoOutput    DisplayOnly
 * │      ├─────────────╠══════════════════╬═════════════════╣
 * │ 输入     Yes / No     NoInputNoOutput    DisplayYesNo
 * │      ├─────────────╠══════════════════╬═════════════════╣
 * │ 能力     Keyboard      KeyboardOnly      DisplayYesNo
 * └──────┴─────────────╚══════════════════╩═════════════════╝
 * 
 * DisplayOnly:     No input                                        + Numeric Output
 *                  不能指示"是"或"否"                                  能显示或数字通信 (6 位十进制数字)
 *                  (手机上表现: 一次确认配对后，需本端键入对端显示的配对 PIN 码)
 * 
 * DisplayYesNo:    ( (Yes / No) / (Keyboard) )                     + Numeric Output
 *                  (Yes / No) 能指示"是"或"否"                         能显示或数字通信 (6 位十进制数字)
 *                  (Keyboard) 能指示"是"或"否"，可输入十进制数字         能显示或数字通信 (6 位十进制数字)
 *                  鉴权方式: 数字比较模式 (Numeric Comparison)
 * 
 * KeyboardOnly:    Keyboard                                        + No Output 
 *                  能指示"是"或"否"，可输入十进制数字                  不能显示或数字通信 (6 位十进制数字)
 *                  鉴权方式: 密码输入模式 (Passkey Entry)
 *
 * NoInputNoOutput: ( (No input) / (Yes / No) )                     + No Output
 *                  不能指示"是"或"否"                                  不能显示或数字通信 (6 位十进制数字)
 *                  鉴权方式: Just Works
 * 
 * KeyboardDisplay: Keyboard                                        + Numeric Output
 *                  能指示"是"或"否"                                    能显示或数字通信 (6 位十进制数字)    
 * 
 * 鉴权方式:
 * - Just Works：不鉴权，不需要用户来参与，是最简单的方法，但不能防 MITM 。
 *      - 适用于没有显示屏或输入能力的设备，例如一些传感器或智能灯泡。
 *      - 手机上表现: 仅需点两次配对确认即可
 * - Passkey Entry：这种方式是由两端设备分别输入 (单端或双端) 配对码的方式进行鉴权的。
 *      - 是蓝牙最早最传统的一种鉴权方式，有两种操作方法：
 *          - 一个设备显示 6 位数字 (Passkey)，另一个设备的需要输入这个数字来，其后 SMP 会将输入的数字传输给对端，来校验数字是否正确。
 *          - 需要分别在两个设备上输入相同的 6 位数字 (Passkey)，其后配对过程会分别校验对端设备的输入来鉴权设备。
 *      - 这提供一定程度的防护来抵御 MITM 攻击，因为攻击者需要知道或拦截 Passkey 来进行攻击。
 *      - 手机上表现: 一次确认配对后，需等待对端键入的本端显示的配对数字
 * - Numeric Comparison：种方式需要两端设备生成且并显示相同的 6 位数字，用户比较后进行确认。
 *      - 适用于双方设备都有显示屏的场景。
 *      - 这也能提供一定程度的防护来抵御 MITM 攻击。
 *      - 手机上表现: 一次确认配对后，提示让本端确认对端配对 PIN 码是否与本端显示一致
 * - OOB (Out of Band)：这种方法中，设备使用一个外部的通道或方法来交换或获取密钥信息。
 *      - 例如通过 NFC ，二维码或者 WIFI 之类的来鉴权。
 * @endcode
 * 
 * 
 * @details 
 *  Initiator (发起设备) 与 Responder (响应设备) 间 IO能力与验证情况 (authentication) 的映射详参: 
 *  《Core_v5.4》 >| BLUETOOTH CORE SPECIFICATION Version 5.4 | Vol 3, Part C 
 *  >| 5.2.2.6 IO and OOB capability mapping to authentication stage 1 method 
 *  >| Table 5.7: IO capability mapping to authentication stage 1
 * 
 * @details 以下提到的输出显示情况可对比一般手机 ble 连接时的配对弹窗情况进行解读:
 *  输出显示是(或)否 : 配对弹窗: 要与此设备配对吗? [取消](这等同否) [配对](这等同是)
 * 
 * 也可通过以下视频 《BLE蓝牙配对模式演示》 进行理解，
 * 地址: https://www.bilibili.com/video/BV1oK411Y7Vh/?spm_id_from=333.337.search-card.all.click&vd_source=92939d5ab16fcf7b067d00f2c763d2b6
 */
typedef uint8_t xf_ble_sm_io_cap_t;  

#define XF_BLE_SM_IO_CAP_OUT                    0   /*!< SIG: DisplayOnly = No input + Numeric Output
                                                     * 详情浏览本类型的注释中 "@details IO 能力详情" 的部分 */
#define XF_BLE_SM_IO_CAP_IO                     1   /*!< SIG: DisplayYesNo = ( (Yes / No) / (Keyboard) ) + Numeric Output
                                                     * 详情浏览本类型的注释中 "@details IO 能力详情" 的部分 */
#define XF_BLE_SM_IO_CAP_IN                     2   /*!< SIG: KeyboardOnly = Keyboard + No Output 
                                                     * 详情浏览本类型的注释中 "@details IO 能力详情" 的部分 */
#define XF_BLE_SM_IO_CAP_NONE                   3   /*!< SIG: NoInputNoOutput = ( (No input) / (Yes / No) ) + No Output
                                                     * 详情浏览本类型的注释中 "@details IO 能力详情" 的部分 */
#define XF_BLE_SM_IO_CAP_KBDISP                 4   /*!< SIG: KeyboardDisplay 
                                                     * : 设备能指示"是"或"否"，可输入十进制数字
                                                     * : 能显示或数字通信 (6 位十进制数字) */

typedef uint8_t xf_ble_sm_authen_option_t;  
#define XF_BLE_SM_AUTHEN_OPTION_DISABLE 0     /*!< authentication disable*/
#define XF_BLE_SM_AUTHEN_OPTION_ENABLE  1     /*!< authentication enable*/

/**
 * @brief OOB (Out of Band) 数据标志位。表示是否支持使用外部通信方式来交换配对过程中使用的一些信息
 * @note 如果一个设备具有对等设备的带外认证数据，则应设置OOB数据标志。
 * @note 0x00 表示不支持 OOB，0x01 表示支持 OOB
 * @details 详参: 《Core_v5.4》 >| BLUETOOTH CORE SPECIFICATION Version 5.4 | Vol 3, Part H
 *  >| 3.5 PAIRING METHODS >| 3.5.1 Pairing Request
 *  >| Table 3.5: OOB data present values
 */
typedef uint8_t xf_ble_sm_oob_enable_t;  
#define XF_BLE_SM_OOB_DISABLE 0     /*!< 禁用 OOB，OOB 验证数据不存在 */
#define XF_BLE_SM_OOB_ENABLE  1     /*!< 启用 OOB，OOB 验证数据来自远程设备 */

/**
 * @if Eng
 * @brief Struct of BLE advertising parameters.
 * @else
 * @brief BLE广播参数。
 * @endif
 */
typedef union _xf_ble_sm_param_val_t
{
    xf_ble_sm_authen_req_t authen_req;  /*!< 鉴权请求， 见 @ref xf_ble_sm_authen_req_t */
    xf_ble_sm_io_cap_t io_capability;   /*!< 输入输出能力 */
    xf_ble_sm_authen_option_t authen_option;    /*!< 安全配对能力选择：
                                                    0x01----支持安全配对
                                                    0x00----不支持安全配对 */
} xf_ble_sm_param_val_t;

typedef enum {
    XF_BLE_SM_PARAM_AUTHEN_REQ_MODE,            /*!< 本端设备的鉴权请求 */
    XF_BLE_SM_PARAM_IO_CAP_MODE,                /*!< 本端设备的 (人机交互) 能力 */
    XF_BLE_SM_PARAM_AUTHEN_OPTION,              /*!< Accept only specified SMP Authentication requirement */
    _XF_BLE_SM_PARAM_MAX_PARAM,                  /*!< authentication max param */
} xf_ble_sm_param_type_t;

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * End of addtogroup group_xf_wal_ble
 * @}
 */

#endif /* XF_BLE_IS_ENABLE */

#endif /* __XF_BLE_SM_TYPES_H__ */
