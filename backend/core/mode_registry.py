"""
mode_registry.py
模式注册表，加载 JSON 模式定义
"""

import json
import os
from typing import Dict, List, Dict
from pathlib import Path


class ModeRegistry:
    """管理所有显示模式"""

    def __init__(self, modes_dir: str = "./modes"):
        self.modes_dir = Path(modes_dir)
        self.modes: Dict[str, Dict] = {}
        self.reload_all()

    def reload_all(self):
        """重新加载所有模式文件"""
        self.modes.clear()

        # 加载内置模式
        builtin_dir = self.modes_dir / "builtin"
        if builtin_dir.exists():
            for file in builtin_dir.glob("*.json"):
                self._load_mode(file)

        # 加载用户自定义模式
        custom_dir = self.modes_dir / "custom"
        if custom_dir.exists():
            for file in custom_dir.glob("*.json"):
                self._load_mode(file)

    def _load_mode(self, file_path: Path):
        """加载单个模式文件"""
        try:
            with open(file_path, "r", encoding="utf-8") as f:
                mode = json.load(f)

            mode_id = mode.get("mode_id")
            if not mode_id:
                print(f"Warning: {file_path} missing mode_id, skipping")
                return

            self.modes[mode_id] = mode
            print(f"Loaded mode: {mode_id} ({mode.get('display_name', mode_id)})")

        except Exception as e:
            print(f"Error loading {file_path}: {e}")

    def get_mode(self, mode_id: str) -> Dict | None:
        """获取模式定义"""
        return self.modes.get(mode_id)

    def list_modes(self) -> List[Dict]:
        """列出所有可用模式"""
        return [
            {
                "mode_id": mode["mode_id"],
                "display_name": mode.get("display_name", mode["mode_id"]),
                "cacheable": mode.get("cacheable", False)
            }
            for mode in self.modes.values()
        ]

    def add_custom_mode(self, mode_id: str, mode_def: Dict) -> bool:
        """添加用户自定义模式"""
        try:
            custom_file = self.modes_dir / "custom" / f"{mode_id}.json"
            with open(custom_file, "w", encoding="utf-8") as f:
                json.dump(mode_def, f, indent=2, ensure_ascii=False)
            self.reload_all()
            return True
        except Exception as e:
            print(f"Error adding custom mode: {e}")
            return False
