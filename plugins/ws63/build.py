import xf_build
from xf_build import api
import logging
import os
import shutil
import glob
import sys
from pathlib import Path
import subprocess
import serial.tools.list_ports

# 默认目标
DEFAULT_TARGET = "ws63-liteos-app"
# SDK 路径
SDK_PATH: Path = api.XF_ROOT / "sdks/fbb_ws63/src"
# SDK 编译输出的路径
SDK_OUTPUT_PATH: Path = SDK_PATH / "output"
XF_PROJECT_BUILD_PATH: Path = api.PROJECT_BUILD_PATH




class ws63():

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
        str_path_sdk_fw_list = glob.glob(f"{SDK_OUTPUT_PATH}/*/fwpkg")
        for str_path_fw in str_path_sdk_fw_list:
            try:
                shutil.copytree(
                    str_path_fw, XF_PROJECT_BUILD_PATH / "sdk/fwpkg", dirs_exist_ok=True)
            except:
                print(f"copy sdk firmware failed!", sys.exc_info())


    def clean(self, args):
        if SDK_OUTPUT_PATH.exists() == True:
            shutil.rmtree(SDK_OUTPUT_PATH)


    def flash(self, args):
        framware_path = "./build/sdk/fwpkg/ws63-liteos-app/ws63-liteos-app_all.fwpkg"
        if not os.path.exists(framware_path):
            logging.error(f"firmware path {framware_path} not exists")
            return
        try:
            # 检查命令是否可用
            subprocess.run(['burn', '--help'], check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        except FileNotFoundError:
            # 尝试安装相应的包
            subprocess.run([sys.executable, '-m', 'pip', 'install', "xf_burn_tools"], check=True)
            # 再次检查命令
            try:
                subprocess.run(['burn', '--help'], check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            except FileNotFoundError:
                logging.error("burn command not found")
        ports = serial.tools.list_ports.comports()
        if len(ports) == 0:
            logging.error("no serial port found")
            return
        for port in ports:
            os.system(f"burn {framware_path}  -p {port.device}")


    def export(self, args):
        pass


    def update(self, args):
        pass


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
