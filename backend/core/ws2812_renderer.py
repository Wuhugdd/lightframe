"""
ws2812_renderer.py
渲染成 8x32 RGB 帧，输出二进制格式
"""

from typing import List, Tuple
from PIL import Image, ImageDraw, ImageFont


class WS2812Renderer:
    """8x32 LED 矩阵渲染器"""

    def __init__(self, width: int = 8, height: int = 32):
        self.width = width
        self.height = height

    def create_frame(self) -> List[Tuple[int, int, int]]:
        """创建空帧"""
        return [(0, 0, 0) for _ in range(self.width * self.height)]

    def set_pixel(self, frame: List[Tuple[int, int, int]], x: int, y: int, color: Tuple[int, int, int]) -> None:
        """设置像素颜色"""
        if 0 <= x < self.width and 0 <= y < self.height:
            idx = y * self.width + x
            frame[idx] = color

    def hsv_to_rgb(self, h: float, s: float, v: float) -> Tuple[int, int, int]:
        """HSV 转 RGB"""
        import colorsys
        r, g, b = colorsys.hsv_to_rgb(h, s, v)
        return (int(r * 255), int(g * 255), int(b * 255))

    def get_rainbow_color(self, position: float) -> Tuple[int, int, int]:
        """获取彩虹色"""
        return self.hsv_to_rgb(position % 1.0, 1.0, 1.0)

    def spectrum_color_scheme(self, scheme_name: str, band: int, position: float) -> Tuple[int, int, int]:
        """根据配色方案获取频谱颜色"""
        schemes = {
            "rainbow": lambda b, p: self.get_rainbow_color(b / 8 + p * 0.2),
            "fire": lambda b, p: (
                int(255 * (position)),
                int(128 * (position) + 64),
                int(32 * position),
            ),
            "ocean": lambda b, p: (
                int(32 * position),
                int(100 * position + 64),
                int(200 * position),
            ),
            "purple": lambda b, p: (
                int(180 + 75 * p),
                int(30 + 90 * p),
                int(180 + 75 * p),
            ),
            "green": lambda b, p: (
                int(50 + 50 * p),
                int(150 + 100 * p),
                int(50 + 50 * p),
            )
        }

        scheme = schemes.get(scheme_name, schemes["rainbow"])
        return scheme(band, position)

    def render_spectrum(self, bands: List[float], color_scheme: str = "rainbow") -> bytes:
        """渲染频谱帧"""
        frame = self.create_frame()
        band_width = self.width // len(bands)

        for band_idx, band_height in enumerate(bands):
            pixels_high = int((band_height / 100) * self.height)

            for y in range(pixels_high):
                pos = y / self.height
                color = self.spectrum_color_scheme(color_scheme, band_idx, pos)

                x_start = band_idx * band_width
                for dx in range(band_width):
                    self.set_pixel(frame, x_start + dx, y, color)

        return self.frame_to_bytes(frame)

    def render_text(self, text: str, color: Tuple[int, int, int] = (255, 255, 255)) -> bytes:
        """渲染文字到帧"""
        # 使用 PIL 渲染文字然后转成 LED 帧
        image = Image.new('RGB', (len(text) * 8, self.height), (0, 0, 0))
        draw = ImageDraw.Draw(image)

        try:
            font = ImageFont.load_default()
            draw.text((0, 0), text, fill=color, font=font)
        except Exception:
            pass

        frame = self.create_frame()

        # 这里简化处理，实际需要根据实际字体像素采样
        for y in range(self.height):
            for x in range(self.width):
                if x < image.width:
                    pixel = image.getpixel((x, y))
                    self.set_pixel(frame, x, y, pixel)

        return self.frame_to_bytes(frame)

    def frame_to_bytes(self, frame: List[Tuple[int, int, int]]) -> bytes:
        """将帧转换为二进制 RGB 格式"""
        # 顺序: R -> G -> B，每个像素 3 字节
        # 总大小: 8 * 32 * 3 = 768 字节
        data = bytearray()
        for r, g, b in frame:
            data.append(r)
            data.append(g)
            data.append(b)
        return bytes(data)
