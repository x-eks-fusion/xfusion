<p align="center">
	<img alt="logo" src="docs/public/image/xfusion_icon.svg" width="200">
</p>

<h1 align="center" style="font-size: 3.5em;line-height: 1;">
    <span style="background: linear-gradient(to right, #3b50f2, #521e79);-webkit-background-clip: text;-webkit-text-fill-color: transparent;">XFusion</span>
</h1>

<h4 align="center" style="font-size: 1.2em; line-height: 1;">
    <span style="background: linear-gradient(to right, #ee9ca7, #33b5ff); -webkit-background-clip: text;-webkit-text-fill-color: transparent;font-weight: bold;">轻量级、跨平台、组件式的嵌入式开发框架</span>
</h4>

<p align="center" style="font-weight: bold;">
	<a href="https://www.coral-zone.cc/#/document?path=/document/">文档中心</a>
    ·
	<a href="http://qa.coral-zone.cc/">问答中心</a>
    ·
	<a href="https://www.coral-zone.cc/#/component">组件库</a>
    ·
	<a href="https://www.coral-zone.cc/#/tools">在线工具库</a>
    ·
	<a href="https://space.bilibili.com/3546733642516945/video">视频</a>
    ·
	<a href="https://www.coral-zone.cc/#/document?path=/document/zh_CN/contribute/">贡献指南</a>
    ·
	<a href="https://github.com/x-eks-fusion/xfusion">GitHub</a>
    ·
	<a href="https://gitee.com/x-eks-fusion/xfusion">Gitee</a>
</p>

<p align="center">
    <a href="https://github.com/x-eks-fusion/xfusion/graphs/contributors">
        <img alt="contributors" src="https://img.shields.io/github/contributors/x-eks-fusion/xfusion.svg?style=flat-square" />
    </a>
    <a href="https://github.com/x-eks-fusion/xfusion/network/members">
        <img alt="forks" src="https://img.shields.io/github/forks/x-eks-fusion/xfusion.svg?style=flat-square" />
    </a>
    <a href="https://github.com/x-eks-fusion/xfusion/stargazers">
        <img alt="stars" src="https://img.shields.io/github/stars/x-eks-fusion/xfusion.svg?style=flat-square" />
    </a>
    <a href="https://github.com/x-eks-fusion/xfusion/issues">
        <img alt="issues" src="https://img.shields.io/github/issues/x-eks-fusion/xfusion.svg?style=flat-square" />
    </a>
    <a href="https://github.com/x-eks-fusion/xfusion/pulls">
        <img alt="pulls" src="https://img.shields.io/github/issues-pr/x-eks-fusion/xfusion.svg?style=flat-square" />
    </a>
    <a href="https://github.com/x-eks-fusion/xfusion/blob/main/LICENSE">
        <img alt="license" src="https://img.shields.io/github/license/x-eks-fusion/xfusion.svg?style=flat-square" />
    </a>
</p>

# XFusion 简介

**XFusion**，来自 **X(Embedded Kits System)** —— 嵌入式套件系统，是一个融合多个嵌入式平台的软件开发工具包(SDK)，为开发者提供**统一且便于开发的嵌入式开发环境**。

**一次开发，多端部署**。

基于 XFusion 开发应用时，无需花费过多时间及精力在移植、驱动等与平台底层相关的工作，可以更专注于应用功能的设计与实现。

得益于 XFusion 统一的接口，基于 XFusion 开发的应用可以快速的在不同平台上切换，敏捷评估平台是否符合应用需求。

Fusion，意为联合、聚变，即 XFusion 的愿景：**联合分散平台，凝聚无限能量。**

## 特性

-   所有子模块可作为软件中间件独立移植
-   全部由 C 编写完成，遵从 C99 语法
-   丰富详实的例程
-   丰富强大的组件库
-   采用 Apache2.0 开源协议
-   支持导出原生工程，可以使用 keil 等原生 IDE 开发调试
-   使用基于 python 的 xf_build 生成对应平台的构建脚本，简化构建方法，统一构建语言
-   使用 menuconfig 可视化配置模块
-   拥有[包管理器](https://www.coral-zone.cc/#/document?path=/document/zh_CN/get-started/install_a_component.html)，可以通过命令行下载第三方组件包

## 硬件要求

-   32 或 64 位微控制器或处理器。
-   建议使用 >16 MHz 时钟速度。
-   闪存/ROM：

    \> 64 kB 用于非常重要的组件 (> 建议使用 180 kB)。

-   RAM:

    -   静态 RAM 使用量：~2 kB，取决于使用的功能和对象类型。
    -   堆: > 2kB (> 建议使用 8 kB 以上)。

-   C99 或更新的编译器。

## 已支持的平台

1. esp32 (基于 esp-idf v5.0.7)
1. ws63 (HI3863 芯片)
1. bs21 (HI2821 芯片)
1. linux 模拟器

## 使用 XFusion 进行开发

关于不同芯片如何搭建 XFusion 的开发环境，请参考 **《[XFusion 入门指南](https://www.coral-zone.cc/#/document?path=/document/zh_CN/get-started/)》**。

### 快速参考

详细的使用方法请参考上面入门指南的链接，这里仅列举一些 XFusion 项目开发中常用的命令：

1.  激活 XFusion

    ```bash
    cd xfusion              # 进入 XFusion 文件夹
    . ./export.sh <target>  # 激活指定的芯片
    ```

1.  选择示例

    ```bash
    cd examples/system/log  # 进入 log 示例文件夹
    ```

1.  配置

    ```bash
    xf menuconfig           # 配置菜单
    ```

1.  编译

    ```bash
    xf build                # 编译当前示例
    ```

1.  烧录

    ```bash
    xf flash                # 烧录到芯片中(需要平台对接支持)
    ```

1.  监视器

    ```bash
    xf monitor <port>       # 查看串口
    ```

如果目标平台需要使用原生 IDE，请在上面的"配置"步骤之后执行以下命令：

1.  导出

    ```bash
    xf export <工程名>      # <工程名> 为导出的工程的名字
    ```

1.  之后用原生平台 IDE 打开工程，完成编译烧录等步骤。

1.  更新导出的工程

    ```bash
    xf update <工程名>      # <工程名> 为前面导出的工程的名字
    ```

其他详细命令可以查看《[xf build 构建脚本命令](https://www.coral-zone.cc/#/document?path=/document/zh_CN/insight/xf_build_script.html)》。

**请注意：**

在 XFusion 开发过程中，XFusion 的 xf_main() 代替了原生的 main 函数的功能。用户需要在 xf_main() 函数内开发，且 xf_main() 不应阻塞，否则会导致其他协作式任务无法执行。

## 示例

XFusion 目前提供了以下几类示例：

1. [get_started-快速入门示例](https://github.com/x-eks-fusion/xfusion/tree/main/examples/get_started)
2. [misc-杂项](https://github.com/x-eks-fusion/xfusion/tree/main/examples/misc)
3. [osal-操作系统抽象层示例](https://github.com/x-eks-fusion/xfusion/tree/main/examples/osal)
4. [peripherals-外设示例](https://github.com/x-eks-fusion/xfusion/tree/main/examples/peripherals)
5. [protocols-协议示例](https://github.com/x-eks-fusion/xfusion/tree/main/examples/protocols)
6. [storage-存储示例](https://github.com/x-eks-fusion/xfusion/tree/main/examples/storage)
7. [system-系统示例](https://github.com/x-eks-fusion/xfusion/tree/main/examples/system)
8. [task-协作式多任务示例](https://github.com/x-eks-fusion/xfusion/tree/main/examples/task)
9. [wireless-无线通信示例](https://github.com/x-eks-fusion/xfusion/tree/main/examples/wireless)

可以点击 [示例详情](examples/README.md) 查看更详细的情况。

一旦找到了需要的示例，便可以进入该目录，执行配置和构建操作。

如果要基于示例工程开始你自己的项目，建议将示例工程复制到 XFusion 目录之外。

## 组件

| 名称        | 描述           |
| :---------- | :------------- |
| xf_fal      | flash 抽象层   |
| xf_hal      | 硬件抽象层     |
| xf_heap     | 动态内存       |
| xf_init     | 自动初始化工具 |
| xf_log      | 日志系统       |
| xf_nal      | 网络接口抽象层 |
| xf_net_apps | 网络应用组件   |
| xf_osal     | 系统抽象层     |
| xf_ota      | 空中升级接口   |
| xf_sys      | 系统基础接口   |
| xf_task     | 协作式调度器   |
| xf_utils    | 通用工具集     |
| xf_vfs      | 虚拟文件系统   |
| xf_wal      | 无线抽象层     |
| xf_build    | 跨平台构建工具 |

## 开源证书

XFusion 及其子模块全部使用 [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0) 许可证开源。

## 贡献

-   如果你有兴趣为 XFusion 作贡献，请先阅读 《[贡献指南](https://www.coral-zone.cc/#/document?path=/document/zh_CN/contribute/)》。

## 其他

-   如果你对 XFusion 的设计背景、详情感兴趣，可以看看《[XFusion 详情](DETAILS.md)》以及《[深入了解](https://www.coral-zone.cc/#/document?path=/document/zh_CN/insight/)》。
-   可以前往 [问答中心](http://qa.coral-zone.cc/) 提问，挖掘社区资源。
-   如果你在使用中发现了错误或者需要新的功能，请先查看 [Issues](https://github.com/x-eks-fusion/xfusion/issues)，确保该问题没有重复提交。
-   如果你想和 XFusion 开发者交流，欢迎加入我们的 QQ 群！ QQ: `993422109`。

## 鸣谢

感谢各位贡献者为 XFusion 所作出的所有贡献！

### 贡献记录 (2024 年)

**年度 MVP 贡献者**

-   [JasonH457](https://github.com/JasonH457)

**体验官**

-   [ZC](https://github.com/zhuchen99899)
-   [surfingwu](https://gitee.com/surfingwu)
-   [beichenzhuo](https://gitee.com/beichenzhuo)
-   [Wan](https://gitee.com/wan_runner)
-   [＂Sunshine](https://github.com/Adragon-0223)
-   [Never ](https://github.com/intreturn0)
-   ​[起飞 ](https://gitee.com/VikcyLiu)
-   ​[罐装可乐](https://gitee.com/wzbzzazyt)
-   [紫千](https://gitee.com/ziqian_null)
-   [邓老师](https://gitee.com/Deng-COCO)
-   [刘恩廷](https://gitee.com/liu-enting)

**贡献官**

-   [Kevincoooool](https://github.com/Kevincoooool)（提供 p4 开发板）
-   [keysking](https://github.com/keysking)（授权在线小工具）
