"""
context.py
获取上下文信息：时间、天气等
"""

from datetime import datetime
from typing import Dict, Any, Optional
import os
import requests
from zoneinfo import ZoneInfo


class Context:
    """提供渲染上下文"""

    def __init__(self):
        self.timezone = ZoneInfo(os.getenv("TIMEZONE", "Asia/Shanghai"))
        self.openweather_key = os.getenv("OPENWEATHER_API_KEY")

    def get_current(self) -> Dict[str, Any]:
        """获取当前上下文"""
        now = datetime.now(self.timezone)

        ctx = {
            "year": now.year,
            "month": now.month,
            "day": now.day,
            "hour": now.hour,
            "minute": now.minute,
            "second": now.second,
            "datetime": now,
        }

        return ctx

    def get_weather(self, city_id: str = "1809878") -> Optional[Dict[str, Any]]:
        """获取当前天气（需要配置 OpenWeather API Key）"""
        if not self.openweather_key:
            return None

        try:
            url = f"https://api.openweathermap.org/data/2.5/weather?id={city_id}&appid={self.openweather_key}&units=metric"
            resp = requests.get(url, timeout=5)
            data = resp.json()

            return {
                "temp": int(data["main"]["temp"]),
                "description": data["weather"][0]["main"],
                "icon": data["weather"][0]["icon"]
            }
        except Exception:
            return None
