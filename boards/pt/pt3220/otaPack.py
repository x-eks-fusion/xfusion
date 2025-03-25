import os, sys
import glob
import binascii
from pathlib import Path
import shutil
import json
from zipfile import ZipFile
import argparse

# NOTE: 请指定 Keil armcc 编译器 bin 目录路径(xxx/ARM/ARMCC/bin)
PATH_CMD = Path('C:/Keil_v5/ARM/ARMCC/bin')

# NOTE: PT3220 XFusion SDK 目前不支持 256k OTA
BASE_ADDR = {
    "256" : { "A": 16384, "B": 131072 }, # 0x4000, 0x20000
    "512" : { "A": 16384, "B": 262144 }, # 0x4000, 0x40000
}

PATH_ARMLINK = PATH_CMD / "armlink.exe"
PATH_FROMELF = PATH_CMD / "fromelf.exe"
if not PATH_ARMLINK.exists() or not PATH_FROMELF.exists():
    raise FileNotFoundError("fromelf.exe or armcc.exe not founded! Please specify the compiler path.")

def compute_crc32(file_path):
    """计算文件的 CRC32 校验和"""
    with open(file_path, 'rb') as f:
        file_bytes = f.read()
    crc32 = binascii.crc32(file_bytes) & 0xFFFFFFFF
    return f"0x{crc32:08X}"

# 配置部分
BANK_B = '_bank_b'
EXT_LNP = '.lnp'
EXT_AXF = '.axf'
EXT_HEX = '.hex'
EXT_BIN = '.bin'
EXT_ASM = '.asm'
PATH_LNP = './output/'

# 获取最新的 .lnp 文件
lnp_files = glob.glob(os.path.join(PATH_LNP, f'*{EXT_LNP}'))
lnp_files = [(file, os.path.getmtime(file)) for file in lnp_files]
if not lnp_files:
    raise FileNotFoundError("No .lnp files found in the specified directory.")
full_path = max(lnp_files, key=lambda x: x[1])[0]

# 去掉后缀名
pname = os.path.splitext(full_path)[0]

# 拼接文件名
bank_a_name = pname
bank_b_name = pname + BANK_B
bank_a_file_name = os.path.basename(bank_a_name)
bank_b_file_name = bank_a_file_name + BANK_B

bankA_lnp = bank_a_name + EXT_LNP
bankA_axf = bank_a_name + EXT_AXF
bankA_hex = bank_a_name + EXT_HEX
bankA_bin = bank_a_name + EXT_BIN
bankA_asm = bank_a_name + EXT_ASM

bankB_lnp = bank_b_name + EXT_LNP
bankB_axf = bank_b_name + EXT_AXF
bankB_hex = bank_b_name + EXT_HEX
bankB_bin = bank_b_name + EXT_BIN
bankB_asm = bank_b_name + EXT_ASM

def relink():
    # 替换并生成目标 .lnp 文件
    with open(bankA_lnp, 'r') as f:
        content = f.read()
    # 输出文件
    content = content.replace(bank_a_file_name, bank_b_file_name)
    with open(bankB_lnp, 'w') as f:
        f.write(content)

    print("**************** Linking High Bank ... ****************")
    os.system(f"{PATH_ARMLINK} --Via {bankB_lnp} --predefine=\"-DSEL_BANK_B\"")

    # 删除目标 .lnp 文件
    os.remove(bankB_lnp)

    # 生成 hex 文件
    # os.system(f"{PATH_FROMELF} {bankA_axf} --i32combined --output {bankA_hex}")
    os.system(f"{PATH_FROMELF} {bankB_axf} --i32combined --output {bankB_hex}")

    # 生成 bin 文件
    # os.system(f"{PATH_FROMELF} {bankA_axf} --bin --output {bankA_bin}")
    os.system(f"{PATH_FROMELF} {bankB_axf} --bin --output {bankB_bin}")

    # 生成 asm 文件
    # os.system(f"{PATH_FROMELF} {bankA_axf} --text -a -c --output {bankA_asm}")
    os.system(f"{PATH_FROMELF} {bankB_axf} --text -a -c --output {bankB_asm}")

    # 计算 CRC32
    print("=================================")
    print(f"File : {os.path.basename(bankA_bin)}")
    crc32 = compute_crc32(bankA_bin)
    print(f"CRC32: {crc32}")
    print("=================================")
    print(f"File : {os.path.basename(bankB_bin)}")
    crc32 = compute_crc32(bankB_bin)
    print(f"CRC32: {crc32}")
    print("=================================")

def zip_files(name, *files):
    with ZipFile(name, 'w') as z:
        for f in files:
            z.write(f, arcname=os.path.basename(f))

def pack(name:str="ota_pack", version:int=1, flash_size:str="256"):
    DIR_PACK = Path("./pack")
    DIR_PACK.mkdir(parents=True, exist_ok=True)

    path_ota_binA = DIR_PACK / "binA.bin"
    path_ota_binB = DIR_PACK / "binB.bin"
    path_ota_info = DIR_PACK / "ota_info.json"
    path_pack = DIR_PACK / (name + ".zip")

    shutil.copy(bankA_bin, path_ota_binA)
    shutil.copy(bankB_bin, path_ota_binB)

    # 生成 ota_info.json 文件, 512k
    ota_info = {
        "version":version, 
        "binA":{
            "path":"./binA.bin", 
            "base":BASE_ADDR[flash_size]["A"],
            },
        "binB":{
            "path":"./binB.bin", 
            "base":BASE_ADDR[flash_size]["B"],
            }
        }
    with open(path_ota_info, 'w') as file:
        json.dump(ota_info, file, indent=4)

    # 打包
    zip_files(path_pack, path_ota_binA, path_ota_binB, path_ota_info)
    print(f"Generate ota pack {path_pack} success!")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="鹏天微 ble 芯片 OTA bankB 固件和 OTA pack 包生成脚本工具")
    parser.add_argument("-v", "--version", type=int, 
                        default=1,
                        help="固件版本号, 1~999")

    parser.add_argument("-n", "--name", type=str, 
                        default="ota_pack", # -n 不存在，使用此值
                        help="ota 压缩包工具名")

    parser.add_argument("-s", "--size", type=str, 
                        default="512",
                        choices=["256", "512"],
                        metavar="[256|512]",
                        help="芯片 FLASH 大小(Hxxx:256, Ixxx:512)")

    args = parser.parse_args(sys.argv[1:])

    if args.size == "256":
        print("XFusion SDK 目前暂不支持 256k (PT3220-Hxxx) OTA")
        sys.exit(0)

    relink()
    pack(args.name, args.version, args.size)
