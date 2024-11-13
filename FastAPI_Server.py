from fastapi import FastAPI, WebSocket
import json
from rich.panel import Panel
from rich.syntax import Syntax
from rich.console import Console

app = FastAPI()


@app.websocket("/ws/send")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    console = Console()
    while True:
        try:
            data = await websocket.receive_text()
            json_object = json.loads(data)
            json_str = json.dumps(json_object, indent=4)
            syntax = Syntax(json_str, "json", theme="monokai",
                            line_numbers=False)
            panel = Panel(syntax, title="Received Data",
                          border_style="blue", padding=(1, 1, 1, 1))
            console.print(panel)
        except (json.JSONDecodeError, TypeError):
            panel = Panel(data, title="Received Data",
                          border_style="blue", padding=(1, 1, 1, 1))
            console.print(panel)

if __name__ == "__main__":
    # You can run the server with the following command
    # uvicorn websocket_server:app --reload
    import uvicorn
    uvicorn.run("FastAPI_Server:app", host="127.0.0.1",
                port=8000, log_level="error", reload=True)
