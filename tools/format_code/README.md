# 格式化

# 关于 astyle

本项目使用 astyle-3.4.12 作为格式化程序。低版本的 astyle 可能不支持一些命令，如`--unpad-brackets`。
本文件夹提供 linux-x64 和 win-x64 环境下的 astyle-3.4.12 可执行程序，未对源码进行任何更改，如无法运行或不信任本文件夹提供的程序可自行编译:D。

# 格式化选项以及说明

1. vscode 安装 chiehyu.vscode-astyle 扩展，并设置 astyle 为默认格式化程序（见第三步）。

2. 创建一个 xfusion vscode 工作区（注意其排序应当是第一位）。

```jsonc
"folders": [
  /* xfusion 目录是第一位 */
  {
    "name": "xfusion",
    "path": "."
  },
  /* 其余工作区文件夹 */
  {
    "name": "uart_echo",
    "path": "examples/peripherals/uart/uart_echo"
  },
],
```

3. 并在 xfusion vscode 工作区"settings"键内添加以下内容：

```jsonc
/* 工作区默认格式化程序设为 astyle */
"editor.defaultFormatter": "chiehyu.vscode-astyle",
/* 可执行文件根据系统选择，除此之外还提供了 astyle-3.4.12-win-x64.exe */
"astyle.executable": "${workspaceFolder}/tools/format_code/astyle/astyle-3.4.12-linux-x64",
/* https://astyle.sourceforge.net/astyle.html */
"astyle.cmd_options": [
  /*
    “One True Brace Style”使用Linux大括号，
    并将大括号添加到无大括号的单行条件语句中。
    左大括号从名称空间、类和函数定义中断开。
    大括号附加到其他所有内容，包括函数中的数组、结构、枚举和语句。
    */
  "--style=1tbs",
  /*
    缩进时，每个空格使用 4 个空格
    */
  "--indent=spaces=4",
  /*
    将大括号附加到命名空间语句。
    */
  "--attach-namespaces",
  /*
    将大括号附加到class语句。
    */
  "--attach-classes",
  /*
    在运算符周围插入空格填充。
    */
  "--pad-oper",
  /*
    在标题之间插入空格填充（例如，'if'，'for'，'while'.）
    */
  "--pad-header",
  /*
    删除括号内外的多余空格填充。
    */
  "--unpad-paren",
  /*
    去掉方括号内外的填充物。
    低版本不起作用。
    */
  "--unpad-brackets",
  /*
    删除超过给定数量的多余空行。
    */
  "--squeeze-lines=1",
  /*
    将指针或引用操作符（*、&或^）附加到变量类型（左）
    或变量名称（右），或将其放置在类型和名称之间（中间）。
    */
  "--align-pointer=name",
  /*
    此选项将与指针分开对齐引用。此选项不会更改指针。
    */
  "--align-reference=name",
  /*
    如果请求keep-one-line-statements，则不会断开多语句行。
    如果请求keep-one-line-blocks 并且标题包含在块中，则不会断开一行块。
    */
  "--keep-one-line-statements",
  /*
    在该行的非缩进部分，将制表符插入空格。
    插入的空格数将保持制表符的间距。
    将使用每个制表符的当前空格设置。
    */
  "--convert-tabs",
  /*
    如果代码超过#个字符，则max-code-length选项将换行。有效值为50到200。
    没有逻辑条件的行将在逻辑条件（||、&&、...），逗号、括号、括号或空格。
    --break-after-logical / -xL
    默认情况下，逻辑条件将放在新行的第一位。
    选项break-after-logical将导致逻辑条件放在前一行的最后。
    如果没有max-code-length，此选项将无效。
    注意！此处虽然设为 120 字符，但是仍然推荐 80 字符优先，便于 vscode
    单屏双栏拆分。
    */
  "--max-code-length=120",
],
```

# 代码格式化脚本: `format.py`

通过以下命令格式化".h", ".c", ".cpp"代码：

```shell
python format.py `<file or dir>`
# 例如：python format.py ${你的xfusion路径}/ports/espressif/esp32/event
```

如果指定文件，则会格式化该文件；如果指定文件夹，则会递归格式化该文件夹下的所有文件。

# astyle 官方资料

- [Artistic Style Install Information](https://astyle.sourceforge.net/install.html)
  该链接说明不同环境下如何编译安装 astyle。

- [Artistic Style](https://astyle.sourceforge.net/astyle.html)
  该链接介绍了格式化样式等信息。
