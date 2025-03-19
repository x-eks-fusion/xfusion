import os
import sys
import xf_build
from xf_build import api

PORT_REL_PATH = os.path.join("..", "port_xf_for_nearlink") 

# 获取 ports/nearlink/port_xf_for_nearlink/port_xf_collect.py 所在目录
RELATIVE_TARGET = api.XF_TARGET_PATH.relative_to(api.XF_ROOT / "boards")
ROOT_PORT = api.XF_ROOT / "ports" / RELATIVE_TARGET
port_dir = (ROOT_PORT / PORT_REL_PATH).resolve().as_posix()
sys.path.insert(0, port_dir)

# 加载获取构建文件列表方法
try:
    from port_xf_collect import get_target_build_lists
except ImportError:
    raise ImportError("无法加载 port_xf_collect 脚本")

# 获取构建文件列表
try:
    current_target = os.path.basename(api.XF_TARGET_PATH)
    lists = get_target_build_lists(target=current_target, port_rel_path=PORT_REL_PATH)
except Exception as e:
    raise RuntimeError(f"配置加载失败: {str(e)}")

xf_build.collect(
    srcs=lists["srcs"],
    inc_dirs=lists["incs"],
    requires=lists["reqs"],
    cflags=lists["cflags"]
)
