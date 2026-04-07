"""
config_store.py
SQLite 配置存储，保存每个设备的配置
"""

import sqlite3
from typing import Dict, Any, Optional
import os


class ConfigStore:
    def __init__(self, db_path: str):
        self.db_path = db_path
        self._init_db()

    def _init_db(self):
        """Initialize database schema"""
        conn = sqlite3.connect(self.db_path)
        cursor = conn.cursor()

        cursor.execute("""
            CREATE TABLE IF NOT EXISTS devices (
                mac_address TEXT PRIMARY KEY,
                active_modes TEXT,
                default_brightness INTEGER,
                spectrum_enabled BOOLEAN,
                auto_dimming BOOLEAN,
                color_scheme TEXT,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        """)

        conn.commit()
        conn.close()

    def get_config(self, mac_address: str) -> Optional[Dict[str, Any]]:
        """Get device configuration by MAC address"""
        conn = sqlite3.connect(self.db_path)
        cursor = conn.cursor()

        cursor.execute("""
            SELECT active_modes, default_brightness, spectrum_enabled,
                   auto_dimming, color_scheme
            FROM devices WHERE mac_address = ?
        """, (mac_address,))

        row = cursor.fetchone()
        conn.close()

        if not row:
            return None

        return {
            "active_modes": row[0].split(",") if row[0] else [],
            "default_brightness": row[1],
            "spectrum_enabled": bool(row[2]),
            "auto_dimming": bool(row[3]),
            "color_scheme": row[4]
        }

    def set_config(self, mac_address: str, config: Dict[str, Any]) -> bool:
        """Save or update device configuration"""
        conn = sqlite3.connect(self.db_path)
        cursor = conn.cursor()

        active_modes = ",".join(config.get("active_modes", []))

        cursor.execute("""
            INSERT OR REPLACE INTO devices
            (mac_address, active_modes, default_brightness,
             spectrum_enabled, auto_dimming, color_scheme, updated_at)
            VALUES (?, ?, ?, ?, ?, ?, CURRENT_TIMESTAMP)
        """, (
            mac_address,
            active_modes,
            config.get("default_brightness", 80),
            config.get("spectrum_enabled", True),
            config.get("auto_dimming", True),
            config.get("color_scheme", "rainbow")
        ))

        conn.commit()
        conn.close()
        return True
