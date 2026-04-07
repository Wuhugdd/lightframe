"""
index.py
FastAPI 入口 + API 端点
"""

from fastapi import FastAPI, HTTPException
from fastapi.staticfiles import StaticFiles
from fastapi.responses import Response
from dotenv import load_dotenv
import os
import sys

# 添加上级目录到路径
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from core.mode_registry import ModeRegistry
from core.pipeline import RenderPipeline
from core.context import Context
from core.ws2812_renderer import WS2812Renderer
from core.config_store import ConfigStore

# 加载环境变量
load_dotenv()

# 初始化组件
app = FastAPI(title="LightFrame Backend", version="1.0")

db_path = os.getenv("DATABASE_PATH", "./lightframe.db")
config_store = ConfigStore(db_path)
mode_registry = ModeRegistry(modes_dir="./modes")
context = Context()
renderer = WS2812Renderer(width=8, height=32)
pipeline = RenderPipeline(mode_registry, context, renderer, config_store)


@app.get("/")
async def root():
    return {
        "name": "LightFrame Backend",
        "version": "1.0",
        "modes_count": len(mode_registry.list_modes()),
        "status": "running"
    }


@app.get("/api/modes")
async def list_modes():
    """列出所有可用模式"""
    return {
        "modes": mode_registry.list_modes()
    }


@app.get("/api/config/{mac_address}")
async def get_config(mac_address: str):
    """获取设备配置"""
    config = config_store.get_config(mac_address)
    if not config:
        return {
            "active_modes": ["CLOCK", "SPECTRUM"],
            "default_brightness": 80,
            "spectrum_enabled": True,
            "auto_dimming": True,
            "color_scheme": "rainbow"
        }
    return config


@app.post("/api/config/{mac_address}")
async def set_config(mac_address: str, config: dict):
    """保存设备配置"""
    success = config_store.set_config(mac_address, config)
    return {"success": success}


@app.get("/api/frame")
async def get_frame(mac: str, mode: str):
    """获取新一帧 RGB 数据

    返回 8*32*3 = 768 字节二进制数据
    """
    frame_bytes = pipeline.render_frame(mac, mode)
    return Response(content=frame_bytes, media_type="application/octet-stream")


# 挂载静态网页文件
web_dir = os.path.join(os.path.dirname(__file__), "..", "..", "webconfig")
if os.path.exists(web_dir):
    app.mount("/config", StaticFiles(directory=web_dir, html=True), name="webconfig")


if __name__ == "__main__":
    import uvicorn

    port = int(os.getenv("PORT", "8000"))
    host = os.getenv("HOST", "0.0.0.0")
    uvicorn.run(app, host=host, port=port)
