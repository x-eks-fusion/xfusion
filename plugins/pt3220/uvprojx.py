from lxml import etree
import os
from pathlib import Path

class UVPROJX:
    def __init__(self, name) -> None:
        self.DIR_PROJECT = os.path.dirname(os.path.abspath(name))

        self.tree = etree.parse(name)
        self.IncludePath = self.tree.xpath("//Cads/VariousControls/IncludePath")[0]
        if self.IncludePath.text:
            self.ctxIncludePath = list(self.IncludePath.text.split(";"))
        else:
            self.ctxIncludePath = []

        self.MiscControls = self.tree.xpath("//Cads/VariousControls/MiscControls")[0]
        self.ctxMiscControls = set()

        self.Groups = self.tree.xpath("//Groups")[0]

    def to_project_relpaths(self, path):
        """
        将 path 转换为基于当前路径工程路径的相对路径
        """
        if isinstance(path, Path):
            path = str(path)

        if isinstance(path, str):
            path = os.path.abspath(path)
            result = os.path.relpath(path, self.DIR_PROJECT)
        elif isinstance(path, list):
            result = []
            for i in path:
                i = os.path.abspath(i)
                result.append(os.path.relpath(i, self.DIR_PROJECT))
        return result

    def save(self, name):
        # MiscControls
        self.ctxMiscControls = sorted(list(self.ctxMiscControls))
        self.MiscControls.text += " " + " ".join(self.ctxMiscControls)
        if self.MiscControls.text[-1] == " ":
            self.MiscControls.text = self.MiscControls.text[:-1]

        # IncludePath
        self.ctxIncludePath = set(self.ctxIncludePath)
        self.ctxIncludePath = sorted(list(self.ctxIncludePath))
        self.IncludePath.text = ";".join(self.ctxIncludePath)

        # 关闭标签自闭合
        Targets = self.tree.xpath("//Targets")[0]
        for node in Targets.iter():
            if node.text is None:
                node.text = ""

        # 保持缩进
        etree.indent(self.tree, space="  ")

        # 保持 Project 下标签空行
        Project = self.tree.xpath("//Project")[0]
        for i, child in enumerate(Project):
            child.tail = "\n\n  "
        Project.text = "\n\n  "
        Project[-1].tail = "\n\n"

        with open(name, "wb") as file:
            file.write(b'<?xml version="1.0" encoding="UTF-8" standalone="no" ?>\n')
            self.tree.write(file, encoding="utf-8", pretty_print=True)

    def set_target(self, name):
        element = self.tree.xpath("//TargetName")[0]
        element.text = name

        element = self.tree.xpath("//OutputName")[0]
        element.text = name

    def set_device(self, device, vendor, packid):
        elemDevice = self.tree.xpath("//Device")[0]
        elemDevice.text = device
        elemVendor = self.tree.xpath("//Vendor")[0]
        elemVendor.text = vendor
        elemPackId = self.tree.xpath("//PackID")[0]
        elemPackId.text = packid

    def set_startup(self, name):
        element = self.tree.xpath("//Group[1]/Files/File/FileName")[0]
        element.text = name
        element = self.tree.xpath("//Group[1]/Files/File/FilePath")[0]
        element.text = name

    def set_cpu(
        self,
        IRAM_end,
        IROM_end,
        IRAM_start="0x20000000",
        IROM_start="0x8000000",
        CLOCK="8000000",
        CPUTYPE="Cortex-M3",
        other="TZ",
    ):
        text = f'IRAM({IRAM_start}-{IRAM_end}) IROM({IROM_start}-{IROM_end}) CLOCK({CLOCK}) CPUTYPE("{CPUTYPE}") {other}'
        element = self.tree.xpath("//Cpu")[0]
        element.text = text

    def set_define(
        self,
        define,
        base="USE_FULL_LL_DRIVER,HSE_VALUE=8000000,HSE_STARTUP_TIMEOUT=100,LSE_STARTUP_TIMEOUT=5000,LSE_VALUE=32768,HSI_VALUE=8000000,LSI_VALUE=40000,VDD_VALUE=3300,PREFETCH_ENABLE=1",
    ):
        element = self.tree.xpath("//Cads/VariousControls/Define")[0]
        element.text = base + ", " + define

    def add_group(self, group):
        elements = self.tree.xpath("//GroupName")
        if group in [i.text for i in elements]:
            return

        element = etree.Element("Group")
        group_name = etree.SubElement(element, "GroupName")
        group_name.text = group
        etree.SubElement(element, "Files")

        self.Groups.append(element)

        return element

    def remove_group(self, group):
        result = self.Groups.xpath(f".//Group[GroupName='{group}']")
        if len(result) > 0:
            Group = result[0]
            self.Groups.remove(Group)

    def __add_file(self, file):
        element = etree.Element("File")
        file_name = etree.SubElement(element, "FileName")
        file_type = etree.SubElement(element, "FileType")
        file_path = etree.SubElement(element, "FilePath")
        file_name.text = os.path.basename(file)
        file_path.text = file
        _, file_extension = os.path.splitext(file_name.text)
        if file_extension == ".c":
            file_type.text = "1"
        elif file_extension == ".s" or file_extension == ".S":
            file_type.text = "2"
        elif file_extension == ".o":
            file_type.text = "3"
        elif file_extension == ".lib":
            file_type.text = "4"
        elif file_extension == ".h":
            file_type.text = "5"
        elif file_extension == ".cpp" or file_extension == ".cxx":
            file_type.text = "8"
        else:
            file_type.text = "9"
        return element

    def add_include_path(self, paths):
        if not paths:
            return

        paths = self.to_project_relpaths(paths)
        if isinstance(paths, str):
            self.ctxIncludePath.append(paths)
        elif isinstance(paths, list):
            self.ctxIncludePath += paths

    def remove_include_path(self, paths):
        if not paths:
            return

        paths = self.to_project_relpaths(paths)
        if isinstance(paths, str):
            if paths in self.ctxIncludePath:
                self.ctxIncludePath.remove(paths)
        elif isinstance(paths, list):
            for v in paths:
                if v in self.ctxIncludePath:
                    self.ctxIncludePath.remove(v)

    def add_files(self, group, files):
        if not files:
            return

        result = self.Groups.xpath(f".//Group[GroupName='{group}']")
        if len(result) == 0:
            return
        Group = result[0]
        Files = Group.xpath(f".//Files")[0]

        FileNames = Group.xpath(f".//FilePath")
        for i in FileNames:
            if i.text in files:
                return

        if isinstance(files, str):
            Files.append(self.__add_file(files))
        elif isinstance(files, list):
            for i in files:
                Files.append(self.__add_file(i))

    def update_files(self, group, files):
        """
        将 group 中原来的文件列表更新为 files
        """
        if not files:
            return

        files = self.to_project_relpaths(files)

        result = self.Groups.xpath(f".//Group[GroupName='{group}']")
        if len(result) == 0:
            elemGroup = self.add_group(group)
        else:
            elemGroup = result[0]

        elemFiles = elemGroup.xpath(f".//Files")[0]
        elemFilePaths = elemGroup.xpath(f".//FilePath")
        lastfiles = set([i.text for i in elemFilePaths])

        if isinstance(files, str):
            currfiles = set([files])
        elif isinstance(files, list):
            currfiles = set(files)

        added = currfiles - lastfiles
        removed = lastfiles - currfiles

        for i in added:
            elemFiles.append(self.__add_file(i))
        for i in removed:
            elem = elemFiles.xpath(f".//FilePath[text()='{i}']")[0].getparent()
            elemFiles.remove(elem)

    def set_c99(self, enable):
        result = self.tree.xpath(f"//uC99")[0]
        if enable:
            result.text = "1"
        else:
            result.text = "0"

    def set_gnu(self, enable):
        result = self.tree.xpath(f"//uGnu")[0]
        if enable:
            result.text = "1"
        else:
            result.text = "0"

    def add_MiscControls(self, params):
        if isinstance(params, str):
            self.ctxMiscControls.add(params)
        elif isinstance(params, list):
            self.ctxMiscControls |= set(params)

    def remove_MiscControls(self, params):
        if isinstance(params, str):
            self.ctxMiscControls.remove(params)
        elif isinstance(params, list):
            self.ctxMiscControls -= set(params)

    def set_preinclude(self, path):
        path = self.to_project_relpaths(path)
        text = f"--preinclude={path}"
        self.add_MiscControls(text)

    def set_ScatterFile(self, path:str):
        path = self.to_project_relpaths(path)
        ScatterFile = self.tree.xpath("//LDads/ScatterFile")[0]
        ScatterFile.text = path

    def set_AfterMake(
        self,
        run_user_prog1=0,
        run_user_prog2=1,
        user_prog1_name="",
        user_prog2_name="",
        user_prog1_dos16_mode=0,
        user_prog2_dos16_mode=0,
        n_stop_a1x=0,
        n_stop_a2x=0,
    ):
        """
        设置 AfterMake 节点的内容。

        :param run_user_prog1: 是否启用用户程序 1（0 或 1）
        :param run_user_prog2: 是否启用用户程序 2（0 或 1）
        :param user_prog1_name: 用户程序 1 的命令字符串
        :param user_prog2_name: 用户程序 2 的命令字符串
        :param user_prog1_dos16_mode: 用户程序 1 的 DOS16 模式（0 或 1）
        :param user_prog2_dos16_mode: 用户程序 2 的 DOS16 模式（0 或 1）
        :param n_stop_a1x: 停止标志 A1X（0 或 1）
        :param n_stop_a2x: 停止标志 A2X（0 或 1）
        """
        after_make = self.tree.xpath("//AfterMake")

        if len(after_make) == 0:
            # 如果 AfterMake 节点不存在，则创建
            targets = self.tree.xpath("//Targets")[0]
            after_make = etree.SubElement(targets, "AfterMake")
        else:
            after_make = after_make[0]

        # 设置 AfterMake 子节点的值
        def set_or_create(node_name, value):
            element = after_make.find(node_name)
            if element is None:
                element = etree.SubElement(after_make, node_name)
            element.text = str(value)

        set_or_create("RunUserProg1", run_user_prog1)
        set_or_create("RunUserProg2", run_user_prog2)
        set_or_create("UserProg1Name", user_prog1_name)
        set_or_create("UserProg2Name", user_prog2_name)
        set_or_create("UserProg1Dos16Mode", user_prog1_dos16_mode)
        set_or_create("UserProg2Dos16Mode", user_prog2_dos16_mode)
        set_or_create("nStopA1X", n_stop_a1x)
        set_or_create("nStopA2X", n_stop_a2x)
