# 关于 xfusion 的 doxygen 文档

## 安装

以 ubuntu 20.04 版本为例：

1.  首先安装 xfusion.

    见 [快速入门 | XFusion (coralzone.github.io)](https://coralzone.github.io/document/zh_CN/get-started/#%E9%80%9A%E8%BF%87-git-%E9%93%BE%E6%8E%A5).

1.  安装 doxygen 的 Linux 的二进制分发版本。

    > [!NOTE]
    > 注意，目前 doxygen 使用版本是 `1.9.8`.
    > ubuntu 20.04 的 glib 版本较低，而 doxygen 最新版 `1.12.0` 需要 `GLIBC_2.34`.

    ```bash
    # 下载 doxygen
    wget https://github.com/doxygen/doxygen/releases/download/Release_1_9_8/doxygen-1.9.8.linux.bin.tar.gz
    # 解压
    tar xf doxygen-1.9.8.linux.bin.tar.gz
    cd doxygen-1.9.8
    # 安装
    sudo make install
    # 查看版本
    doxygen --version
    # 1.9.8 (c2fe5c3e4986974eb2a97608b24086683502f07f)
    ```

1.  安装其他必要依赖项。

    ```bash
    sudo apt-get install graphviz
    ```

1.  如果 xfusion/docs 中没有 doxygen 样式子模块 `doxygen-awesome-css`，请手动克隆到 `xfusion/docs/doxygen-awesome-css`.

    注意 `git checkout v2.3.3`.

> [!NOTE]
> 以下是一些 doxygen 教程，可以忽略。
>
> -   [【Doxygen】Doxygen 使用配置及注释语法规范-CSDN 博客](https://blog.csdn.net/Stay_Hun_forward/article/details/140873510)
> -   [干货 | 教你如何制作漂亮且专业的程序文档 (qq.com)](https://mp.weixin.qq.com/s/HA352TEBELNJ9pgCTMkWIg)
> -   [技术|在 Linux 上用 Doxygen 生成源代码文档](https://linux.cn/article-16227-1.html)

## 使用方式

### 直接使用

```bash
# 移动到 xfusion 的 docs 目录下
# 注意！！！目前 Doxyfile 使用的是相对路径，只能在 docs/ 目录下生成文档！
cd docs
doxygen Doxyfile
# warning: The selected output language "chinese" has not been updated
# since release 1.9.4.  As a result some sentences may appear in English.
#
# Adding custom extension mapping: 'inc' will be treated as language 'c'
# Doxygen version used: 1.9.8 (c2fe5c3e4986974eb2a97608b24086683502f07f)
# Searching for include files...
# Searching for example files...
# ...
# Running plantuml with JAVA...
# Running dot...
# type lookup cache used 1283/65536 hits=5127 misses=1310
# symbol lookup cache used 1858/65536 hits=2724 misses=1858
# finished...
```

生成完毕后 html 会输出到 `docs/build/html/index.html` ，打开即可看到效果。

> [!IMPORTANT]
> 生成完毕后请检查 `docs/build/doxygen_log.txt`, 确保其中没有警告。

### 生成 pdf

> [!NOTE]
> 此步骤是可选的。

1.  安装生成 pdf 必须的依赖项。

    其中 `doxygen-latex` 和 `latex-cjk-*` 安装可能会花十几分钟（下载慢）。

    ```bash
    sudo apt-get install doxygen graphviz doxygen-latex latex-cjk-*
    ```

1.  运行 doxygen。

    ```bash
    # 如果之前已经运行过，此步骤可忽略
    cd docs
    doxygen Doxyfile
    ```

1.  （可选）修改 refman.tex 配置文件。

    有教程说：

    ```markdown
    需要对 `docs/build/latex/refman.tex` 文件进行修改
    具体为：
    在 `% Packages required by doxygen` 后添加 `\usepackage{CJKutf8}`
    在 `\begin{document}` 后添加 `\begin{CJK}{UTF8}{gbsn}`
    在 `\end{document}` 前添加 `\end{CJK}`
    ```

    但是经过尝试，是可以生成中文文档的，因此此步骤可选。

1.  生成 pdf。

    ```bash
    cd docs/build/latex
    make -j
    ```

生成完毕后输出 pdf 到 `docs/build/latex/refman.pdf`, 打开即可。

> [!NOTE]
> 以下是一些 doxygen 生成 pdf 的教程，可以忽略。
>
> -   [linux 环境下 doxygen 使用方法\_linux doxygen-CSDN 博客](https://blog.csdn.net/qq_30095135/article/details/129171919)
> -   [Linux 下使用 Doxygen 生成中文接口文档（PDF 版本） - 代码先锋网 (codeleading.com)](https://www.codeleading.com/article/13334389084/)
> -   [doxygen 生成的 latex 编译 pdf 中文报错 LaTeX Error: Unicode character 解决方法-CSDN 博客](https://xiaowen.blog.csdn.net/article/details/124393321)

### 生成 chm

> [!NOTE]
> 此步骤是可选的。

目前看来似乎只有在 windows 上才能生成，暂略。

TODO linux 上生成 doxygen chm 文档。

### doxygen 常用文档

> [!NOTE]
> 常用的官方文档如下：
>
> -   [Doxygen: Installation](https://www.doxygen.nl/manual/install.html#install_bin_unix)
> -   [Doxygen: Configuration](https://www.doxygen.nl/manual/config.html)
> -   [Doxygen: Special Commands](https://www.doxygen.nl/manual/commands.html)
> -   [Doxygen: Grouping](https://www.doxygen.nl/manual/grouping.html)
> -   [Doxygen: Custom Commands](https://www.doxygen.nl/manual/custcmd.html)

## 备注

### 待办事项

TODO 文档待办如下：

-   [ ] wal 文档及其注释

    注意，xf_wal 目前在 `docs/Doxyfile` 的 EXCLUDE 中，修改完毕注释后记得移除 EXCLUDE 中的 `../components/xf_wal`.
