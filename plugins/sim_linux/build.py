import xf_build
from xf_build import api
import logging
import os
import shutil
import glob
import sys
from pathlib import Path

# xf 构建相关的描述文件输出的路径
XF_BUILD_DESC_FILE_PATH: Path = api.XF_TARGET_PATH / ".xfusion"
SDK_OUTPUT_PATH: Path = api.XF_TARGET_PATH / "build"
XF_PROJECT_BUILD_PATH: Path = api.PROJECT_BUILD_PATH
XF_PROJECT_NAME = os.environ.get("XF_PROJECT")

hookimpl = xf_build.get_hookimpl()


class sim_linux():
    @hookimpl
    def build(self, args):
        if XF_BUILD_DESC_FILE_PATH.exists() is False:
            os.mkdir(XF_BUILD_DESC_FILE_PATH)

        project_xmake_file: Path = XF_BUILD_DESC_FILE_PATH / "xmake.lua"
        api.apply_template("xmake_project.j2", project_xmake_file)
        api.cd_to_target()

        api.exec_cmd(["xmake", "b"])

        # 尝试将构建输出的文件复制到工程目录下的 build/sdk 目录（如无则创建）
        list_path_sdk_exec_file = glob.glob(
            f"{SDK_OUTPUT_PATH}/**/{XF_PROJECT_NAME}", recursive=True
        )
        for path_sdk_exec in list_path_sdk_exec_file:
            _out_path = Path(path_sdk_exec).parent  # 获取父目录
            try:
                shutil.copytree(
                    _out_path,
                    XF_PROJECT_BUILD_PATH / "sdk",
                    dirs_exist_ok=True,
                )
            except:
                print("copy sdk firmware failed!", sys.exc_info())

    @hookimpl
    def clean(self, args):
        if SDK_OUTPUT_PATH.exists() is True:
            shutil.rmtree(SDK_OUTPUT_PATH)
        api.cd_to_target()
        os.system("xmake c")

    @hookimpl
    def flash(self, args):
        api.cd_to_target()
        os.system("xmake r")

    @hookimpl
    def export(self, args):
        pass

    @hookimpl
    def update(self, args):
        pass

    @hookimpl
    def menuconfig(self, args):
        pass
