from xf_build import api
import os
import shutil
import glob
import sys
from pathlib import Path
import logging
import subprocess
import serial.tools.list_ports

XF_PROJECT_BUILD_PATH: Path = api.PROJECT_BUILD_PATH
class bs21():

    def get_sdk_default_target(self):
        sdk_default_target = api.get_define("PLATFORM_SDK_DEFAULT_TARGET")
        return sdk_default_target

    def get_sdk_path(self):
        sdk_rel_path = api.get_define("PLATFORM_SDK_RELATIVE_PATH")
        SDK_PATH: Path = api.XF_ROOT / sdk_rel_path
        return SDK_PATH.resolve()

    def build(self, args):
        project_cmake_file: Path = XF_PROJECT_BUILD_PATH / "build_environ.cmake"
        api.apply_template("cmake_project.j2", project_cmake_file)
        os.chdir(self.get_sdk_path())

        target = self.get_sdk_default_target()
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
        SDK_OUTPUT_PATH: Path = self.get_sdk_path() / "output"
        str_path_sdk_fw_list = glob.glob(f"{SDK_OUTPUT_PATH}/*/fwpkg")
        for str_path_fw in str_path_sdk_fw_list:
            try:
                shutil.copytree(
                    str_path_fw, XF_PROJECT_BUILD_PATH / "sdk/fwpkg")
            except:
                print("copy sdk firmware failed!", sys.exc_info())

    def clean(self, args):
        SDK_OUTPUT_PATH: Path = self.get_sdk_path() / "output"
        if SDK_OUTPUT_PATH.exists() is True:
            shutil.rmtree(SDK_OUTPUT_PATH)

    def flash(self, args):
        target = self.get_sdk_default_target()
        PROJECT_FW_PATH: Path = XF_PROJECT_BUILD_PATH / "sdk/fwpkg" / target
        str_path_search = f"{PROJECT_FW_PATH}/*_all.fwpkg"
        str_path_sdk_fw_list = glob.glob(str_path_search)

        if len(str_path_sdk_fw_list) == 0:
            logging.error(f"firmware path {str_path_search} not exists")
            return
        try:
            # 检查命令是否可用
            subprocess.run(['burn', '--help'], check=True,
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        except FileNotFoundError:
            # 尝试安装相应的包
            subprocess.run([sys.executable, '-m', 'pip',
                           'install', "xf_burn_tools"], check=True)
            # 再次检查命令
            try:
                subprocess.run(['burn', '--help'], check=True,
                               stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            except FileNotFoundError:
                logging.error("burn command not found")
        ports = serial.tools.list_ports.comports()
        if len(ports) == 0:
            logging.error("no serial port found")
            return
        for port in ports:
            os.system(f"burn {str_path_sdk_fw_list[0]}  -p {port.device}")

    def export(self, name, args):
        pass

    def update(self, name, args):
        pass

    def menuconfig(self, args):
        # 第一个拓展参数为 sub 时 打开SDK 侧 menuconfig
        if args[0] == "sub":
            target = self.get_sdk_default_target()
            """
            第二个拓展参数为（如果存在）：
                help : 显示 SDK 侧 menuconfig 可选目标
                其他 : 用户指定 menuconfig 目标
            """
            str_menuconfig_sub = f"python build.py menuconfig"
            if len(args) == 1:
                str_menuconfig_sub += f" {target}"
            elif len(args) == 2:
                if args[1] != "help":
                    str_menuconfig_sub += f" {args[1]}"

            os.chdir(self.get_sdk_path())
            os.system(str_menuconfig_sub)
