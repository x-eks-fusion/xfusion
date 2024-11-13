from fastapi import FastAPI, WebSocket, WebSocketDisconnect
import json
from rich.panel import Panel
from rich.syntax import Syntax
from rich.text import Text
from rich.console import Console
import struct

app = FastAPI()

device = {}


def pack_tlv(type_value, data):
    # 确保 type_value 是 4 个字节，data 是字节对象
    if not isinstance(type_value, int) or not (0 <= type_value < (1 << 32)):
        raise ValueError("Type must be an integer within 4 bytes range.")
    if isinstance(data, str):
        data_bytes = data.encode()  # 如果输入是字符串，则编码成字节
    elif isinstance(data, int):
        data_bytes = data.to_bytes(
            (data.bit_length() + 7) // 8, byteorder='big', signed=False)

    length = len(data_bytes)

    # 构建 TLV 格式的二进制流
    tlv = struct.pack('>I Q', type_value, length) + data_bytes

    return tlv


def get_io_level(id):
    id = str(id)
    if not device.get(id):
        return
    res = 0
    if device[id].get("data"):
        res = int.from_bytes(device[id]["data"], byteorder='big')
    elif device[id].get("config") and device[id]["config"].get("pull") != None:
        if device[id]["config"]["pull"] == 1:
            res = 1
        elif device[id]["config"]["pull"] == 0:
            res = 0
    return res


@app.websocket("/ws/send")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    console = Console()
    try:
        while True:
            data = await websocket.receive_bytes()
            type = struct.unpack('<I', data[:4])[0]
            size = struct.unpack('<Q', data[4:12])[0]
            context = data[12:12+size]
            if type == 0:
                context = context.decode()
                json_object = json.loads(context)
                json_str = json.dumps(json_object, indent=4)
                content = Syntax(json_str, "json", theme="monokai",
                                 line_numbers=False)
                if not device.get(json_object['id']):
                    device[f"{json_object['id']}"] = {}
                device[f"{json_object['id']}"]["config"] = json_object
            elif type == 1:
                context = context.decode()
                json_object = json.loads(context)
                json_str = json.dumps(json_object, indent=4)
                content = Syntax(json_str, "json", theme="monokai",
                                 line_numbers=False)
                await websocket.send_bytes(pack_tlv(type, get_io_level(json_object["id"])))
            else:
                device[f"{type}"]["data"] = context
                content = Text(f"data:{context}")
            panel = Panel(content,
                          title=f"type:{type}, size:{size}",
                          border_style="blue",
                          padding=(1, 1, 1, 1))
            console.print(panel)
    except WebSocketDisconnect:
        # 捕获断链异常
        console.print(Panel(Text("Client disconnected"), border_style="red"))

if __name__ == "__main__":
    # You can run the server with the following command
    # uvicorn websocket_server:app --reload
    import uvicorn
    uvicorn.run("FastAPI_Server2:app", host="127.0.0.1",
                port=8000, log_level="error", reload=True)
