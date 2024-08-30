#!/usr/bin/env python3

from pathlib import Path
import os

PORTS_DIR = "boards"
CHARACTERISTIC_FILE = "target.json"


def collect_folders(directory, base_dir=None):
    """
    在指定目录中递归搜索 CHARACTERISTIC_FILE 文件。

    :param directory: 要搜索的目录。
    :return: 两个列表，一个包含找到文件的文件夹路径，另一个包含文件夹名称。
    """
    folder_name = []  # 存储文件夹名称
    if not base_dir:
        base_dir = directory

    directory = Path(directory)
    for entry in directory.iterdir():
        if not entry.is_dir():
            continue

        folder_path_inside = directory / entry.name
        if CHARACTERISTIC_FILE in [f.name for f in folder_path_inside.iterdir() if f.is_file()]:
            folder_name.append(entry.name)
            print(entry.name)  # 不是用于打印，用于给脚本捕获
        else:
            _res = collect_folders(folder_path_inside, base_dir)
            folder_name.extend(_res)

    return folder_name


def gen_kconfig(path, folder_name):
    # 防止归零错误
    if len(folder_name) == 0:
        with open(Path(path)/"XFKconfig", "w", encoding="utf-8") as f:
            f.write("")
            return

    # 格式化平台配置
    unit_list = []
    for i in folder_name:
        unit = f'    config PLATFORM_{i}\n'\
            f'        bool "{i} Platform"\n'\
            f'        depends on PLATFORM="{i.lower()}"\n'\
            f'        default \"y\" if PLATFORM="{i.lower()}"\n'
        unit_list.append(unit)

    # 格式化xfkconfig
    unit_str = "\n".join(unit_list)
    xfkconfig = "config PLATFORM\n"\
        "    string\n"\
        '    default "$XF_TARGET"\n'\
        '\n'\
        "config PLATFORM_PATH\n"\
        "    string\n"\
        '    default "$XF_TARGET_PATH"\n'\
        '\n'\
        'menu "Platform Selection"\n'\
        f'\n{unit_str}\n'\
        'endmenu\n'\
        '\norsource \"$(XF_TARGET_PATH)/XFKconfig\"\n'
    return xfkconfig


def save_kconfig(path, kconfig):
    with open(Path(path) / "XFKconfig", "w", encoding="utf-8") as f:
        f.write(kconfig)


def generate_port_xfkconfig():
    """
    自动扫描port下面的文件夹，生成XFKconfig配置文件
    """

    root = os.environ["XF_ROOT"]
    path = Path(root) / PORTS_DIR

    folder_name = collect_folders(path, PORTS_DIR)
    folder_name = [i.upper() for i in folder_name]

    xfkconfig = gen_kconfig(path, folder_name)
    save_kconfig(path, xfkconfig)


if __name__ == "__main__":
    generate_port_xfkconfig()
