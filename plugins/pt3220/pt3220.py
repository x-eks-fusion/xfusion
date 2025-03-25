from lxml import etree
import os
from pathlib import Path
from .uvprojx import UVPROJX

class PT3220(UVPROJX):
    def __init__(self, path_project):
        super().__init__(path_project)

    def set_blelib(self, path_lib=None):
        group_name = "ble"
        result = self.Groups.xpath(f".//Group[GroupName='{group_name}']")

        # 没有 ble 这个 Group
        if len(result) == 0:
            self.update_files(group_name, path_lib)
            return

        elemGroup = result[0]
        if path_lib == None:
            # 清空
            for elem in elemGroup.xpath("./*"):
                if elem.tag != "GroupName":
                    elemGroup.remove(elem)
        else:
            path_lib = self.to_project_relpaths(path_lib)
            elemFiles = elemGroup.xpath(f".//Files")

            # 为了不让 ble group 位置发生变化
            if len(elemFiles) == 0:
                elemFiles = etree.SubElement(elemGroup, "Files")
                elemFile = etree.SubElement(elemFiles, "File")
                elemFileName = etree.SubElement(elemFile, "FileName")
                elemFileName.text = os.path.basename(path_lib)
                elemFileType = etree.SubElement(elemFile, "FileType")
                elemFileType.text = "4"
                elemFilePath = etree.SubElement(elemFile, "FilePath")
                elemFilePath.text = path_lib
            else:
                elemFileName = elemGroup.xpath(f".//Files/File/FileName")[0]
                elemFileName.text = os.path.basename(path_lib)
                elemFilePath = elemGroup.xpath(f".//Files/File/FilePath")[0]
                elemFilePath.text = path_lib
