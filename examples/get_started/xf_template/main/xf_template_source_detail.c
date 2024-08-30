/**
 * @file xf_template_source_detail.c
 * @author 你的名字 (你的邮箱@domain.com)
 *
 * @brief xf_template 编程风格模板。
 *
 * @details
 *
 * 1. 每行字数限制
 *
 *    建议每行**少于 80 个字符**，这样在 vscode 中分两栏也可以看全部代码。
 * 123456789012345678901234567890123456789012345678901234567890123456789012345678
 *    代码正文最长不要超过 120 字符。
 *
 * 2. 注释
 *
 *    2.1 中文与英文、数字之间根据需要加空格。
 *        例如：
 *        1. 你说的对，但是《STM32》是由意法半导体 (ST) 推出的一系列 32 位的单片机。
 *        2. 在 windows 10, 1080p, 缩放 125 %, vscode 默认的缩放的情况下。
 *
 * 3. 缩进
 *
 *    代码中使用 4 个空格作为缩进，不要使用 TAB。
 *
 * 4. 行尾
 *
 *    不要在行尾尾随空格。
 *
 * 5. 大括号
 *
 *    只有函数定义的左大括号必须换行，在函数中的左大括号不需要换行（`K&R`风格）。
 *
 * @version 1.0
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 * @see
 * - [Linux kernel coding style](https://www.kernel.org/doc/html/v5.19/process/coding-style.html)
 * - [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
 * - [5 Making The Best Use of C](https://www.gnu.org/prep/standards/html_node/Writing-C.html)
 * - [Recommended C style and coding rules](https://github.com/MaJerle/c-code-style)
 * - [Espressif IoT Development Framework Style Guide](https://docs.espressif.com/projects/esp-idf/zh_CN/v5.1/esp32/contribute/style-guide.html)
 * - [Documenting Code](https://docs.espressif.com/projects/esp-idf/zh_CN/v5.1/esp32/contribute/documenting-code.html)
 */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 须知 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * @brief 关于格式化程序。
 *
 * 基于 esp-idf 的格式化风格修改。
 *
 * @see esp-idf/tools/format.sh: https://github.com/espressif/esp-idf/blob/v5.0/tools/format.sh
 * @note 安装 Astyle 应用程序以及 vscode 插件后，在 vscode 的 setting.json 中添加：
 * @code{json}
 * "astyle.cmd_options": [
 *     "--indent=spaces=4",
 *     "--attach-namespaces",
 *     "--attach-classes",
 *     "--pad-oper",
 *     "--pad-header",
 *     "--unpad-paren",
 *     "--unpad-brackets",
 *     "--squeeze-lines=2",
 *     "--align-pointer=name",
 *     "--align-reference=name",
 *     "--keep-one-line-statements",
 *     "--convert-tabs",
 *     "--max-code-length=120"
 * ],
 * // xfusion 提供了 astyle-3.4.12: xfusion/tools/format_code/astyle/astyle-3.4.12-linux-x64
 * "astyle.executable": "astyle", ///< 修改为你的 astyle 可执行文件的路径
 * @endcode
 * 注意填写 Astyle 可执行文件路径到 vscode 中。
 */

///
/// @brief 一些说明。
/// @note **不要**用"///"的 doxygen 注释格式，此处使用是因为注释中需要包含多行注释。
/// @details
///
/// # 1. 内容标识：用于指示以下区域应当填写什么内容。
///
/// 如：
/// /* ==================== [Includes] ========================================== */
/// /* ==================== [Defines] =========================================== */
///
/// 注意：
///     1.1 内容标识中，从开头到'['（不含）共有 24 个字符。
///         `/* ==================== `
///     1.2 内容标识中，从开头到末尾共有 80 个字符，编程时尽量不要超过内容标识的长度。
///     1.3 如需更改 "[]" 内的内容，请增删 ']' 右侧的 '='，使总长度维持 80 字符。
///     1.4 内容标识的顺序**不建议更改**。
///
/// # 2. 代码与内容标识之间要有一个空白换行的间隔。
///
/// @code{c}
///                              /* 此行空白换行 */
/// /* ==================== [Includes] ========================================== */
///                              /* 此行空白换行 */
/// #include <stdio.h>
///                              /* 此行空白换行 */
/// /* ==================== [Defines] =========================================== */
///                              /* 此行空白换行 */
/// #define XF_TEMPLATE_DEFINE              (1)
///                              /* 此行空白换行 */
/// @endcode
///
/// # 3. 如果注释中只有一句 brief，注意删除后面的空行。如：
///
/// /**
///  * @brief 我是注释。
///  */
/// /**
///  * @brief 不要这样，下面的空行意义不明的。
///  *
///  */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 须知 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * @brief 头文件: [Includes].
 *
 * @details 在这里放头文件，通常顺序是：
 * 1. 当前组件的公共头文件；如：`#include "xf_template_header_detail.h"`；
 * 2. 标准库，如 `#include <stdio.h>`；
 * 3. 其他 POSIX 标准标头，如：`#include <sys/time.h>`；
 * 4. 本文件所需要的其他组件的头文件，如：`#include "xf_log.h"`、`#include "xf_spi.h"`；
 * 5. 本文件所需要的当前组件的其他头文件或者私有头文件。
 *
 * @see https://docs.espressif.com/projects/esp-idf/zh_CN/v5.1/esp32/contribute/style-guide.html#include-statements
 */
/* ==================== [Includes] ========================================== */

#include "xf_template_header_detail.h"

// #include <stdio.h>

#include "xf_component_template.h"
#include "xf_utils.h"

/**
 * @brief 宏定义(无参宏): [Defines].
 *
 * 1. 一般建议加括号。
 * 2. 不强制要求对齐，除非代码已经稳定很少更改。
 */
/* ==================== [Defines] =========================================== */

#define XF_TEMPLATE_HELP_STR            "xf_template v0.1"
#define XF_TEMPLATE_HELP_STR_SPLICING   "test" XF_TEMPLATE_HELP_STR "abc123"
#define XF_TEMPLATE_DEFINE              (1)

/**
 * @brief 类型定义: [Typedefs].
 *
 * @details 类型定义的类型**必须**以`_t`为结尾。前缀`xf_`表示是在 xf_hal 中定义的。
 */
/* ==================== [Typedefs] ========================================== */

/**
 * @brief 枚举类型示例。
 *
 * @details
 * 1. 枚举类型通过类型定义重命名。
 * 2. 枚举类型命名是类型定义名字前加单下划线。如 "_xf_templ_enum_t"。
 *    用 xf_templ_enum_e 也可以。
 * 3. 枚举类型的值需要大写。
 * 4. 枚举值通常要求有一个最大值，并且该最大值通常不作为正常值使用。
 */
typedef enum _xf_templ_enum_t {
    XF_TEMPL_ENUM_0 = 0x00,         /*!< 枚举值 0，第一个枚举值要求手动赋值 */
    XF_TEMPL_ENUM_1,                /*!< 枚举值 1 */
    XF_TEMPL_ENUM_2,                /*!< 枚举值 2 */
    /* 此处保留一行空行，以区分正常值和最大值 */
    XF_TEMPL_ENUM_MAX,              /*!< 枚举值最大值 */

    XF_TEMPL_ENUM_DEFAULT = XF_TEMPL_ENUM_1,    /*!< 枚举值默认值 */
} xf_templ_enum_t;

typedef int xf_templ_int_t; /*!< "!<"表示当前的注释的说明对象是前面的代码，
                             *   没有的话默认都是说明后面的代码。
                             */

/**
 * @brief 结构体示例。
 *
 * @details
 * 1. 结构体**必须**用类型定义。
 * 2. 结构体名字是类型定义名字前加单下划线。如：`_xf_templ_struct_t`。
 *    当然用 xf_templ_struct_s 也可以。
 */
typedef struct _xf_templ_struct_t {
    xf_templ_int_t num;             /*!< 这是一个数字 */
    char *p_str;                    /*!< 这是一个字符串指针，前缀`p_`强调指针类型 */
    union {                         /*!< 结构体内的联合体或结构体等可以匿名 */
        uint8_t all;                /*!< 通过这个值可以修改整个共用体 */
        struct {                    /*!< 结构体内的联合体或结构体等可以匿名 */
            uint8_t val_u4: 4;      /*!< 这是位域的示例，u4 表示有 4 位，根据位置可能是 [0..3] */
            uint8_t val_bit4: 1;    /*!< 这是位域的示例，bit4 表示的是从 bit0 起数的第 5 位 */
            uint8_t reserved: 3;    /*!< 这是位域中未使用的位 */
        } bits;  /*!< 如果使用了英文缩写，应当在此说明缩写的原文 */
    } data;
} xf_templ_struct_t;

/**
 * @brief 静态函数原型: [Static Prototypes].
 *
 * @details
 * 1. 只在当前文件内可以调用的函数，通常建议函数名前加**单下划线**。
 * 2. 注意在此处添加静态函数的说明注释。
 */
/* ==================== [Static Prototypes] ================================= */

/**
 * @brief 一个静态函数。（说明注释）
 *
 * @param p_struct 输入一个的结构体。
 *      参数默认都是输入，可以不标记[in]，
 * @param[out] p_out 指向用于传出数据的指针（会改变原本的变量）。
 *      需要传出的参数注释时添加[out]。
 *
 * @return xf_templ_int_t 返回一个返回值。
 *      - (XF_TEMPL_FAIL)           出错了
 *      - (other)                   不值得注意的返回值
 *
 * @note 函数如果需要传出，那么传出的参数应当放在后面。
 */
static xf_templ_int_t _xf_template_func(xf_templ_struct_t *p_struct, int32_t *p_out);

/**
 * @brief 一个演示函数参数换行的示例。
 *
 * @param a_parameter_with_a_very_long_name 无用参数。
 * @param another_parameter_with_a_very_long_name 无用参数。
 * @param a_parameter_with_a_very_long_name_too 无用参数。
 * @param a_parameter_with_a_very_long_name_that_is_really_long 无用参数。
 *
 * @note void 声明的函数无需写返回值的注释。
 */
void a_function_of_very_many_parameters(
    uint32_t a_parameter_with_a_very_long_name,
    uint32_t another_parameter_with_a_very_long_name,
    uint32_t a_parameter_with_a_very_long_name_too,
    uint32_t a_parameter_with_a_very_long_name_that_is_really_long
);

/**
 * @brief 静态变量: [Static Variables].
 *
 * 1. 只在当前文件内可以调用的变量。
 * 2. **必须**用`s_`前缀标识静态变量。
 * 3. **强烈建议**指针用`p_`前缀标识。可以与`s_`组合使用(`sp_`)，表示静态指针类型。
 */
/* ==================== [Static Variables] ================================== */

static const char *const TAG        = "xf_main"; /*!< 日志标签 */
static xf_templ_int_t s_data        = 0;    /*!< 一个全局静态数据 */
static xf_templ_struct_t s_struct   = {0};  /*!< 一个结构体 */
static void *p_a_pointer            = NULL; /*!< 一个指针 */

/**
 * @brief 全局变量: [Global Variables].
 *
 * 1. 有可能会在别的源文件(.c 文件)内调用的变量。
 * 2. 必须用`g_`前缀标识全局变量。
 * 3. 通常**禁止**使用全局变量，如需使用，可以封装获取和修改该变量的函数。
 *
 * @warning 通常**禁止**使用全局变量。
 */
/* ==================== [Global Variables] ================================== */

int g_xf_templ_global_data1_in_head = 0; /*!< 有在头文件中声明的全局变量 */
int g_xf_templ_global_data2_no_in_head = 0; /*!< 没有在头文件中声明的全局变量 */

/**
 * @brief 带参宏: [Macros].
 *
 * 1. 与宏定义（[Defines]）的区别是是否有参数（不严格区分）。
 * 2. 宏中参数通常建议以下划线开头，以区别于普通变量。
 * 3. 宏参数在使用时通常加以括号，防止表达式传入宏参数时造成逻辑错误。
 * 4. 宏如果需要多行，不要求对齐继续符"\"。
 */
/* ==================== [Macros] ============================================ */

/**
 * @brief 带参宏示例。
 */
#define XF_TEMPLATE_MACROS_ADD(_a, _b) ((_a) + (_b))

/**
 * @brief 无需返回参数的宏。
 *
 * 1. 通常需要用 do { } while (0) 包围（除非通过宏定义变量等情况）。
 * 2. while (0) 后不要加分号（使用时强制加分号）。
 */
#define XF_TEMPLATE_MACROS_NO_RETURN(_a, _b) \
    do { \
        s_data = (_a) + (_b); \
    } while (0)

/**
 * @brief 需要返回参数的宏。
 *
 * 1. 使用 ({ }) 包围。
 * 2. 明显括出返回值。
 */
#define XF_TEMPLATE_MACROS_HAS_RETURN(_x)   \
    ({                                      \
        typeof(_x) __ret = (_x);            \
        __ret = __ret + (_x);               \
        (__ret);                            \
    })

/**
 * @brief 关于条件编译。
 *
 * 1. 需要在对应的 #else 后追加相应的的条件（如：`!defined(xf_printf)`），
 *    在 #endif 后标注 #if 的信息（如：`defined(xf_printf)`）。
 *
 * @note 如何宏需要缩进保持美观，请在'#'号后面缩进。如下缩进所示。
 */
/* xf_template 输出接口 */
#ifdef xf_printf
#   define XF_TEMPL_PRINTF(_fmt, ...)   xf_printf(_fmt, ##__VA_ARGS__)
#else /* !defined(xf_printf) */
#   define XF_TEMPL_PRINTF(_fmt, ...)
#endif /* defined(xf_printf) */

#ifndef UNUSED
#   define UNUSED(_x)                   ((void)(_x))     /*!< 未使用的变量 */
#endif

/**
 * @brief 全局函数定义: [Global Functions].
 *
 * 1. 全局函数原型在**头文件**中声明并编写注释，此处可以不加 brief。
 */
/* ==================== [Global Functions] ================================== */

/**
 * @brief 主函数。
 */
void xf_main(void)
{
    /* ↑函数定义首行换行 */
    /* 不要以空行开始函数 */
    xf_templ_int_t ret = 0;
    int32_t val = 0; /*!< 用于接收 _xf_template_func 的传出 */

    UNUSED(p_a_pointer); /*!< 防止警告 */

    XF_LOGI(TAG, "hello");

    /**
     * @brief 分支语句哪怕只有一句也必须加花括号。
     */
    if (XF_TEMPLATE_VERSION != XF_TEMPLATE_VERSION_CHECK(1, 0, 0)) {
        XF_TEMPL_PRINTF("error: version check failed\n");
    } else {
        XF_TEMPL_PRINTF("version check: ok\n");
    } /*!< 必要时此处需要添加判断条件，以说明该花括号的来源  */

    XF_TEMPL_PRINTF("XF_TEMPLATE_VERSION: %d\n", (int)XF_TEMPLATE_VERSION);

    /**
     * @brief 判断语句中，如果一行不足以放下判断语句，
     * 分行时需要将逻辑符号放在新行的开头。
     * 此时最好还是用变量表达判断语句。
     */
    if ((XF_TEMPLATE_VERSION != XF_TEMPLATE_VERSION_CHECK(1, 0, 0))
            || ((XF_TEMPLATE_VERSION != XF_TEMPLATE_VERSION_CHECK(1, 1, 0))
                && (XF_TEMPLATE_VERSION != XF_TEMPLATE_VERSION_CHECK(1, 2, 0)))) {
        /* 空语句 */
    }

    xf_component_func();
    xf_template_another_func();
    xf_template_init();

    ret = _xf_template_func(&s_struct, &val);
    /**
     * @brief 常量在前可以避免不必要的逻辑错误。
     * 如 if (XF_TEMPL_FAIL = ret) 时编译会报错。
     */
    if (XF_TEMPL_FAIL == ret) {
        XF_TEMPL_PRINTF("error: ret is XF_TEMPL_FAIL\n");
        return;
    }

    XF_TEMPL_PRINTF("ret: %d\n", ret);
    XF_TEMPL_PRINTF("ok\n");

    /**
     * @brief 对于返回值为 void 的函数，
     * `return;`可有可无，文件风格统一即可。
     */
    return;
    /* 也不要以空行结束函数 */
}
/*!< 正常情况下此行空白：函数之间需要有一个空行作为分隔 */
xf_templ_err_t xf_template_init(void)
{

    /* **错误**示例 ↑，不要以空行开始函数 */
    s_struct.num =
        XF_TEMPL_ENUM_2 +
        XF_TEMPLATE_MACROS_ADD(
            g_xf_templ_global_data1_in_head,
            g_xf_templ_global_data2_no_in_head
        );
    XF_TEMPL_PRINTF("xf_template_init: s_struct.num: %d\n", (int)s_struct.num);
    return XF_TEMPL_OK;
    /* **错误**示例 ↓，也不要以空行结束函数 */

}

/* ==================== [Static Functions] ================================== */

static xf_templ_int_t _xf_template_func(xf_templ_struct_t *p_struct, int32_t *p_out)
{
    xf_templ_int_t ret = 0;
    /* 参数检查 */
    if ((NULL == p_struct) || (NULL == p_out) || (p_struct->num >= 100)) {
        XF_TEMPL_PRINTF("参数错误\n");
        goto _xf_template_func_err; /*!< goto 通常用作出错后处理。 */
    }

    /* 具体操作 */
    p_struct->p_str                 = XF_TEMPLATE_HELP_STR;
    p_struct->data.all              = 0;
    p_struct->data.bits.val_u4      = 0x0a;
    p_struct->data.bits.val_bit4    = true;
    for (uint8_t i = 0; i < sizeof(XF_TEMPLATE_HELP_STR); i++) {
        /**
         * @brief 单字节变量通常只能用作循环变量。
         */
        ret += p_struct->p_str[i];
    }
    ret += p_struct->data.all;

    s_data = ret; /*!< 记录返回值 */
    *p_out = ret;

    /* 返回 */
    return ret;

_xf_template_func_err: /*!< 注意 label 前面要有单下划线 */
    return XF_TEMPL_FAIL;
}

void a_function_of_very_many_parameters(
    uint32_t a_parameter_with_a_very_long_name,
    uint32_t another_parameter_with_a_very_long_name,
    uint32_t a_parameter_with_a_very_long_name_too,
    uint32_t a_parameter_with_a_very_long_name_that_is_really_long
)
{
    UNUSED(a_parameter_with_a_very_long_name);
    UNUSED(another_parameter_with_a_very_long_name);
    UNUSED(a_parameter_with_a_very_long_name_too);
    UNUSED(a_parameter_with_a_very_long_name_that_is_really_long);
}

/* 为了消除警告，文件末尾(↓)通常带一个换行 */
