#!/usr/bin/env python3

import click
import subprocess
from pathlib import Path
import sys
from rich.console import Console
from rich.progress import Progress, SpinnerColumn, BarColumn, TextColumn

console = Console()


@click.command()
@click.argument("path", type=click.Path(exists=True))
def format_code(path):
    """格式化文件或文件夹中的代码使用 AStyle 工具."""
    path = Path(path).resolve()
    if sys.platform == "linux":
        cmd = Path(__file__).resolve().parent / \
            "astyle" / "astyle-3.4.12-linux-x64"
    elif sys.platform == "win32":
        cmd = Path(__file__).resolve().parent / \
            "astyle" / "astyle-3.4.12-win-x64.exe"
    else:
        console.print("Unsupported platform", style="bold red")
        return

    args = "--style=1tbs "\
        "--indent=spaces=4 "\
        "--attach-namespaces "\
        "--attach-classes "\
        "--pad-oper "\
        "--pad-header "\
        "--unpad-paren "\
        "--unpad-brackets "\
        "--squeeze-lines=1 "\
        "--align-pointer=name "\
        "--align-reference=name "\
        "--keep-one-line-statements "\
        "--convert-tabs "\
        "--max-code-length=120 "\
        "--suffix=none "

    def format_file(file_path):
        try:
            result = subprocess.run(
                [str(cmd)] + args.split() + [file_path],
                check=True,
                capture_output=True,
                text=True
            )
            # if result.stdout:
            #     console.print(result.stdout, style="blue")
            # if result.stderr:
            #     console.print(result.stderr, style="red")
        except subprocess.CalledProcessError:
            console.print(
                f"格式化文件时出错: {file_path}, 可能是因为你没安装astyle，请尝试："
                "sudo apt install astyle -y",
                style="bold red",
            )

    if path.is_dir():
        # 处理文件夹
        with Progress() as progress:
            task = progress.add_task("格式化文件夹中...", total=sum(
                1 for _ in path.rglob('*') if _.suffix in (".h", ".c", ".cpp")))
            for file_path in path.rglob('*'):
                if file_path.suffix in (".h", ".c", ".cpp"):
                    format_file(file_path)
                progress.update(task, advance=1)
        console.print(f"{path} 格式化成功！", style="bold green")
    elif path.is_file():
        # 处理单个文件
        if path.suffix in (".h", ".c", ".cpp"):
            format_file(path)
            console.print(f"{path.name} 格式化成功！", style="bold green")
    else:
        console.print("输入的路径既不是文件也不是文件夹。", style="bold red")


if __name__ == "__main__":
    format_code()
