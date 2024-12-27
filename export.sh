#!/bin/bash

# 该脚本无法直接运行，需要通过 sourcing 方式（通过 source​ 或 .​ 命令）来执行脚本
# sourcing的方式可以设置环境变量， 所以必须要以sourcing方式执行，否则脚本功能无法实现

# 获取真实路径
__realpath() {
    wdir="$PWD"; [ "$PWD" = "/" ] && wdir=""
    arg=$1
    case "$arg" in
        /*) scriptdir="${arg}";;
        *) scriptdir="$wdir/${arg#./}";;
    esac
    scriptdir="${scriptdir%/*}"
    echo "$scriptdir"
}

__verbose() {
    [ -n "${EXPORT_QUIET-}" ] && return
    echo "$@"
}

# 获取脚本所在的路径
__script_dir(){
    if [[ "$OSTYPE" == "darwin"* ]]; then # 针对MACOS的处理
        # 将可能的相对路径转换为绝对路径
        script_dir="$(__realpath "${self_path}")"
        # 解析任何 ../ 引用以使路径更短
        script_dir="$(cd "${script_dir}" || return 1; pwd)"
    else
        script_name="$(readlink -f "${self_path}")" # readlink解析并获取真实路径
        script_dir="$(dirname "${script_name}")" # dirname获取路径文件夹
    fi

    if [ "$script_dir" = '.' ]
    then
       script_dir="$(pwd)"
    fi

    echo "$script_dir"
}

# 如果脚本不是使用sourcing方式运行的则无法运行
__is_sourcing()
{
    if [ -n "${BASH_SOURCE-}" ] && [ "${BASH_SOURCE[0]}" = "${0}" ]
    then
        echo "This script should be sourced, not executed:"
        echo ". ${BASH_SOURCE[0]}"
        exit 1
    fi
}

# 主功能，两个目标，设置环境变量
__main()
{
    if [ -z "${XF_ROOT-}" ]
    then
        # 环境中未设置 XF_ROOT。
        export XF_ROOT="${script_dir}"
    else
        # 检查路径是否有效
        if [ ! "${XF_ROOT}" = "${script_dir}" ]
        then
            # 当不同目录中有两个版本的时候，就必须要更改XF_ROOT，否则则会产生错误
            echo "Resetting XF_ROOT from '${XF_ROOT}' to '${script_dir}' "
            export XF_ROOT="${script_dir}"
        fi
    fi
}

__set_port(){
    port_target=()

    # 调用 Python 脚本，并逐行读取输出到数组
    while IFS= read -r line; do
        port_target+=("$line")
    done < <(python3 "${XF_ROOT}"/tools/export_script/gen_kconfig.py)

    if [ $# -lt 1 ]; then
        printf "You need to specify a target:"
        printf " %s" "${port_target[@]}"
        printf "\n"
        return 1
    fi

    if [ $# -gt 1 ]; then
        printf "Too many parameters, only one parameter is supported\n"
        return 1
    fi

    not_found=true
    for item in "${port_target[@]}"; do
        if [ "$item" = "$1" ]; then
            not_found=false
            break
        fi
    done

    if $not_found; then
        printf "You need to choose one of the following targets:"
        printf " %s" "${port_target[@]}"
        printf "\n"
        return 1
    fi

    export XF_TARGET="$1"
    export XF_VERSION="v1.2.0"
    export XF_TARGET_PATH="$(python3 ${XF_ROOT}/tools/export_script/get_path.py $1)"
}

__get_script_dir()
{
    # 获取此脚本的绝对路径
    self_path=""

    if [ -n "${BASH_SOURCE-}" ]
    then
        self_path="${BASH_SOURCE}"
    elif [ -n "${ZSH_VERSION-}" ]
    then
        self_path="${(%):-%x}"
    fi
    script_dir="$(__script_dir)"
}

__enter_virtualenv()
{
    python ${XF_ROOT}/tools/export_script/check_virtualenv.py
    result=$?
    if [ "$result" -eq 2 ] || [ "$result" -eq 1 ]; then
        source ~/.xfusion/${XF_VERSION}/bin/activate
    fi
    python3 -m pip install --upgrade pip -i https://pypi.tuna.tsinghua.edu.cn/simple
    pip install xf_build==0.4.1 -i https://pypi.tuna.tsinghua.edu.cn/simple
}

__print_value()
{
    printf "XF_ROOT:        %s\n" "${XF_ROOT}"
    printf "XF_TARGET:      %s\n" "${XF_TARGET}"
    printf "XF_VERSION:     %s\n" "${XF_VERSION}"
    printf "XF_TARGET_PATH: %s\n" "${XF_TARGET_PATH}"
}

__cleanup() {
    unset script_dir
    unset self_path
    unset port_target
    unset not_found
    unset item
    unset res
    unset export_script
    unset result

    unset __realpath
    unset __main
    unset __verbose
    unset __cleanup
    unset __set_port
    unset __enter_virtualenv
    unset __print_value

    return $1
}

__is_sourcing

__get_script_dir

__main

if [ $? -ne 0 ]; then
    return
fi

__set_port $@

if [ $? -ne 0 ]; then
    return
fi

__enter_virtualenv

__print_value

__cleanup $?
