import os
import argparse
from pathlib import Path

PORTS_DIR = "boards"
CHARACTERISTIC_FILE = "target.json"


def find_matching_dir(port):
    xf_root = os.environ.get("XF_ROOT")
    if not xf_root:
        return None
    root_dir = Path(xf_root) / PORTS_DIR
    for port_dir in root_dir.rglob(port):
        if not port_dir.is_dir():
            continue
        if (port_dir / CHARACTERISTIC_FILE).exists():
            print(port_dir)
            return port_dir
    return None


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Find matching directory")
    parser.add_argument("port", type=str, help="The port to search for")
    args = parser.parse_args()

    find_matching_dir(args.port)
