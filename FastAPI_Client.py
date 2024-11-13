# client.py
import asyncio
from fastapi import FastAPI
from contextlib import asynccontextmanager
from websockets import connect

app = FastAPI()


@asynccontextmanager
async def lifespan(app: FastAPI):
    # 在应用启动时启动 WebSocket 客户端
    task = asyncio.create_task(websocket_client())
    yield  # 允许应用启动并运行
    # 在应用关闭时取消任务
    task.cancel()


async def websocket_client():
    uri = "ws://127.0.0.1:8000/ws/send"
    async with connect(uri) as websocket:
        print("Connected to the WebSocket server.")
        message = '{"message":"Hello, server!"}'
        await websocket.send(message)
        while True:
            response = await websocket.recv()
            print(f"Received from server: {response}")

app.router.lifespan_context = lifespan  # 将 lifespan 注册为应用生命周期管理

if __name__ == "__main__":
    import uvicorn
    uvicorn.run("FastAPI_Client:app", host="127.0.0.1",
                port=8001, log_level="error", reload=True)
