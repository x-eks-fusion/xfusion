import xf_build
from xf_build import api
import os
import shutil
import glob
import sys
from pathlib import Path

# 默认目标
DEFAULT_TARGET = "bs21e-sle-ble-slp-central-peripheral"
# SDK 路径
SDK_PATH: Path = api.XF_ROOT / "sdks/bs2x_sdk/"
# SDK 编译输出的路径
SDK_BUILD_OUTPUT_PATH: Path = SDK_PATH / "output"
XF_PROJECT_BUILD_PATH: Path = api.PROJECT_BUILD_PATH

hookimpl = xf_build.get_hookimpl()


class bs21():
    @hookimpl
    def build(self, args):
        project_cmake_file: Path = XF_PROJECT_BUILD_PATH / "build_environ.cmake"
        api.apply_template("cmake_project.j2", project_cmake_file)
        os.chdir(SDK_PATH.resolve())

        target = DEFAULT_TARGET
        # 第一个拓展参数为 help 时 显示 SDK 侧 编译时的可选目标
        """
        第一个拓展参数为（如果存在）：
            help : 显示 SDK 侧 编译时的可选目标
            其他 : 用户指定编译目标
        """
        if len(args) != 0:
            if args[0] == "help":
                target = ""
            else:
                target = args[0]
        api.exec_cmd(["python", "build.py", "-c -nhso -release", target])

        # 尝试将 output 下 fwpkg 目录复制到工程目录下的 build 目录下的 sdk 目录（如无则创建）
        str_path_sdk_fw_list = glob.glob(f"{SDK_BUILD_OUTPUT_PATH}/*/fwpkg")
        for str_path_fw in str_path_sdk_fw_list:
            try:
                shutil.copytree(
                    str_path_fw, XF_PROJECT_BUILD_PATH / "sdk/fwpkg")
            except:
                print("copy sdk firmware failed!", sys.exc_info())

    @hookimpl
    def clean(self, args):
        if SDK_BUILD_OUTPUT_PATH.exists() is True:
            shutil.rmtree(SDK_BUILD_OUTPUT_PATH)

    @hookimpl
    def flash(self, args):
        pass

    @hookimpl
    def export(self, args):
        pass

    @hookimpl
    def update(self, args):
        pass

    @hookimpl
    def menuconfig(self, args):
        # 第一个拓展参数为 sub 时 打开SDK 侧 menuconfig
        if args[0] == "sub":
            target = DEFAULT_TARGET
            """
            第二个拓展参数为（如果存在）：
                help : 显示 SDK 侧 menuconfig 可选目标
                其他 : 用户指定 menuconfig 目标
            """
            if len(args) == 2:
                if args[1] == "help":
                    target = ""
                else:
                    target = args[1]
            os.chdir(SDK_PATH)
            str_menuconfig_sub = f"python build.py menuconfig ${target}"
            os.system(str_menuconfig_sub)
