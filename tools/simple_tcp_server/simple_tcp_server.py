import socket
import struct
from rich.panel import Panel
from rich.syntax import Syntax
from rich.text import Text
from rich.console import Console
import json
import sys
import threading
from PySide6.QtWidgets import QApplication, QWidget
from sim_log_display import sim_log_display

# 配置服务器IP和端口
SERVER_IP = '127.0.0.1'
SERVER_PORT = 8000
BUFFER_SIZE = 1024

device = {}
console = Console()
g_sim_log_disp = None

def pack_tlv(type_value, data):
    # 确保 type_value 是 4 个字节，data 是字节对象
    if not isinstance(type_value, int) or not (0 <= type_value < (1 << 32)):
        raise ValueError("Type must be an integer within 4 bytes range.")
    if isinstance(data, str):
        data_bytes = data.encode()  # 如果输入是字符串，则编码成字节
    elif isinstance(data, int):
        data_bytes = data.to_bytes(
            (data.bit_length() + 7) // 8 or 1, byteorder='little', signed=False
        )

    length = len(data_bytes)

    # 构建 TLV 格式的二进制流
    tlv = struct.pack('<I Q', type_value, length) + data_bytes

    return tlv

def get_io_level(id):
    id = str(id)
    if not device.get(id):
        return
    res = 0
    if device[id].get("data"):
        res = int.from_bytes(device[id]["data"], byteorder='little')
    elif device[id].get("config") and device[id]["config"].get("pull") != None:
        if device[id]["config"]["pull"] == 1:
            res = 1
        elif device[id]["config"]["pull"] == 2:
            res = 0
    return res


def recv_all(sock, length):
    """接收指定长度的完整数据"""
    data = b''
    while len(data) < length:
        more = sock.recv(length - len(data))
        if not more:
            raise ConnectionError("Client disconnected before receiving complete data")
        data += more
    return data


def run_sim_server():
    # 创建TCP服务器套接字
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 设置SO_REUSEADDR选项
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # 绑定服务器地址和端口
    server_socket.bind((SERVER_IP, SERVER_PORT))

    # 开始监听连接请求，最大连接数为5
    server_socket.listen(5)
    print(f"Server listening on {SERVER_IP}:{SERVER_PORT}...")

    # 接受客户端连接
    client_socket, client_address = server_socket.accept()
    print(f"Connection established with {client_address}")

    try:
        # 接收来自客户端的消息
        while True:
            # 先接收TLV中的Type（4字节）和Length（8字节），共12字节
            header = client_socket.recv(12)
            if not header:
                print("Client disconnected")
                break

            # 使用struct解包Type和Length
            msg_type, msg_length = struct.unpack('<IQ', header)

            # 根据Length接收Value数据
            value = client_socket.recv(msg_length)

            if msg_type == 0:
                context = value.decode()
                json_object = json.loads(context)
                json_str = json.dumps(json_object, indent=4)
                content = Syntax(json_str, "json", theme="monokai", line_numbers=False)
                if not device.get(json_object['id']):
                    device[f"{json_object['id']}"] = {}
                device[f"{json_object['id']}"]["config"] = json_object
            elif msg_type == 1:
                context = value.decode()
                json_object = json.loads(context)
                json_str = json.dumps(json_object, indent=4)
                content = Syntax(json_str, "json", theme="monokai", line_numbers=False)
                tlv_data = pack_tlv(msg_type, get_io_level(json_object["id"]))
                client_socket.send(tlv_data)
            elif msg_type == 67108864:
                log_message = value.decode()
                g_sim_log_disp.log(log_message)
            else:
                device[f"{msg_type}"]["data"] = value
                content = Text(f"data:{value}")
            panel = Panel(
                content,
                title=f"type:{msg_type}, size:{msg_length}",
                border_style="blue",
                padding=(1, 1, 1, 1)
            )
            console.print(panel)

    except ConnectionError:
        console.print(Panel(Text("Client disconnected"), border_style="red"))
    finally:
        # 关闭客户端连接和服务器套接字
        client_socket.close()
        server_socket.close()


def run_sim_modules():
    global g_sim_log_disp

    app = QApplication.instance() or QApplication([])

    # 创建 sim_log_display 设置默认字体大小为16，默认主题为深色
    g_sim_log_disp = sim_log_display(16, "dark")
    g_sim_log_disp.show()

    sys.exit(app.exec())

if __name__ == "__main__":
    sim_server_thread = threading.Thread(target=run_sim_server)
    sim_server_thread.start()

    run_sim_modules()
