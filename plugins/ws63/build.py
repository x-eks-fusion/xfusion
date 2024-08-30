import xf_build
from xf_build import api
import logging
import os
import shutil
from pathlib import Path


hookimpl = xf_build.get_hookimpl()
SDK_PATH: Path = api.XF_ROOT / "sdks/fbb_ws63/src"; 
class ws63():
    @hookimpl
    def build(self, args):
        project_cmake_file: Path = api.PROJECT_BUILD_PATH / "build_environ.cmake"
        api.apply_template("cmake_project.j2", project_cmake_file)
        os.chdir(SDK_PATH.resolve())
        api.exec_cmd(["python", "build.py", "-c -nhso -release ws63-liteos-app"])
        return
                
    @hookimpl
    def clean(self, args):
        ws63_output_path: Path = SDK_PATH / "output"
        if ws63_output_path.exists() == True:
            shutil.rmtree(ws63_output_path)

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
        if args[0] == "sub":
            os.chdir(SDK_PATH)
            os.system("python build.py ws63-liteos-app menuconfig")
