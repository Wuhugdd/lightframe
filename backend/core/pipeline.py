"""
pipeline.py
统一生成+渲染管线
"""

from typing import Dict, Any, Optional
from .mode_registry import ModeRegistry
from .context import Context
from .ws2812_renderer import WS2812Renderer
from .config_store import ConfigStore


class RenderPipeline:
    """统一渲染管线"""

    def __init__(self, mode_registry: ModeRegistry, context: Context, renderer: WS2812Renderer, config_store: ConfigStore):
        self.mode_registry = mode_registry
        self.context = context
        self.renderer = renderer
        self.config_store = config_store

    def render_frame(self, mac: str, mode_id: str, spectrum_bands: Optional[list] = None) -> bytes:
        """渲染一帧"""

        # 获取设备配置
        config = self.config_store.get_config(mac) or {}
        color_scheme = config.get("color_scheme", "rainbow")

        # 获取模式定义
        mode = self.mode_registry.get_mode(mode_id)
        if not mode:
            # 返回黑色帧
            return self.renderer.frame_to_bytes(self.renderer.create_frame())

        # 获取上下文
        ctx = self.context.get_current()

        # 根据模式类型渲染
        mode_type = mode.get("content", {}).get("type")

        if mode_type == "spectrum_only":
            # 纯频谱模式，需要 ESP32 提供频谱数据
            if spectrum_bands:
                return self.renderer.render_spectrum(spectrum_bands, color_scheme)
            else:
                return self.renderer.render_spectrum([0] * 8, color_scheme)

        elif mode_type == "time":
            # 时钟模式，可能带频谱
            layout = mode.get("layout", {})
            clock_height = layout.get("clock_height", 8)
            spectrum_height = layout.get("spectrum_height", 24)
            clock_color = layout.get("clock_color", "#ffffff")

            # TODO: 实现时钟渲染
            # 这里先返回全黑，后续实现

        elif mode_type == "text":
            # 滚动文字模式
            text = mode.get("content", {}).get("text", "")
            # TODO: 实现滚动文字

        # 默认返回黑帧
        return self.renderer.frame_to_bytes(self.renderer.create_frame())
