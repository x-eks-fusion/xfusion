from pathlib import Path
import sys
import os
import subprocess


def is_in_virtual_environment():
    return hasattr(sys, 'real_prefix') or (hasattr(sys, 'base_prefix') and sys.base_prefix != sys.prefix)


venv_dir = Path.home() / f".xfusion/{os.environ['XF_VERSION']}"
venv_dir = venv_dir.resolve()

venv_dir.mkdir(parents=True, exist_ok=True)

if is_in_virtual_environment():
    sys_prefix = Path(sys.prefix)
    if sys_prefix.resolve() == venv_dir:
        print("In the virtual environment created by ourselves")
        exit(0)
    else:
        print("In a virtual environment created by others")
        exit(3)
else:
    venv_config_file = venv_dir / "pyvenv.cfg"
    if venv_config_file.is_file():
        print("Virtual environment is created but not activated")
        exit(2)
    else:
        try:
            # 尝试导入virtualenv来检查是否已安装
            import tools.export_script.check_virtualenv as check_virtualenv
        except ImportError:
            # 如果未安装，使用pip安装virtualenv
            print("virtualenv is not installed, installing...")
            subprocess.run([sys.executable, "-m", "pip",
                           "install", "virtualenv"], check=True)
            print("virtualenv installed successfully")
        subprocess.run(
            [sys.executable, "-m", "virtualenv", str(venv_dir)], check=True)
        print("Virtual environment created successfully")
        exit(1)
