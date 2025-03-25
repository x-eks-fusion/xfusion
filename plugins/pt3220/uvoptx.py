from lxml import etree
import os
from pathlib import Path

class UVOPTX:
    def __init__(self, name) -> None:
        self.DIR_PROJECT = os.path.dirname(os.path.abspath(name))

        self.tree = etree.parse(name)
        self.Target = self.tree.xpath("//Target")[0]

    def save(self, name):
        def disable_tag_self_close(node):
            """关闭标签自闭合"""
            Targets = self.tree.xpath(f"//{node}")[0]
            for node in Targets.iter():
                if node.text is None:
                    node.text = ""
        disable_tag_self_close("Target")

        # 保持缩进
        etree.indent(self.tree, space="  ")

        def keep_space(node):
            """保持 node 下标签空行"""
            elem = self.tree.xpath(f"//{node}")[0]
            for i, child in enumerate(elem):
                child.tail = "\n\n  "
            elem.text = "\n\n  "
            elem[-1].tail = "\n\n"
        keep_space("ProjectOpt")

        with open(name, "wb") as file:
            file.write(b'<?xml version="1.0" encoding="UTF-8" standalone="no" ?>\n')
            self.tree.write(file, encoding="utf-8", pretty_print=True)

    def set_target_name(self, name):
        elemTargetName = self.Target.xpath("./TargetName")[0]
        elemTargetName.text = name

    def set_sw_param(self, flash_algo_start:str, flash_algo_size:str, 
                     device_name:str, flm_path:str,
                     flm_start:str, flm_size:str):
        """
        设置烧录参数
        Params:
            flash_algo_start (str): 烧录算法起始地址
            flash_algo_size (str): 烧录算法 ram 占用大小
            device_name (str): PACK 包中的设备名
            flm_path (str): PACK 包 flm 文件的相对路径
            flm_start (str): flm 文件起始地址
            flm_size (str): flm 文件大小
        """
        text = "-U20090928 -O78 -S2 -ZTIFSpeedSel5000 -A0 -C0 -JU1 -JI127.0.0.1 -JP0 -RST0 -N00(\"ARM CoreSight SW-DP\") "
        text += "-D00(0BC11477) -L00(0) -TO18 -TC10000000 -TP21 -TDS8007 -TDT0 -TDC1F -TIEFFFFFFFF -TIP8 -TB1 -TFE0 -FO11 "

        def hex_prepare(value):
            """ 去除 0x 及 多余的 0 """
            return value.lstrip("0x").lstrip("0") or "0"

        # RAM for Algorithm
        flash_algo_start = hex_prepare(flash_algo_start)
        flash_algo_size = hex_prepare(flash_algo_size)
        text += f"-FD{flash_algo_start} -FC{flash_algo_size} -FN1 "

        # Programming Algorithm
        flm_start = hex_prepare(flm_start)
        flm_size = hex_prepare(flm_size)
        text += f"-FF0{os.path.basename(flm_path)} -FS0{flm_start} -FL0{flm_size} -FP0($$Device:{device_name}${flm_path})"

        elem = self.Target.xpath("//SetRegEntry/Name")[0]
        elem.text = text
