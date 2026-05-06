# Light Frame · 光帧

A smart WS2812 music clock that truly belongs on your desk — watch the spectrum dance to your music during the day, check the time at a glance, and enjoy the vibe at night.

[**中文介绍**](README_ZH.md) | [**Hardware Guide**](docs/hardware.md) | [**Setup Tutorial**](docs/setup.md)

---

## ✨ Features

> On your desk: a clock, a music visualizer, and a programmer's dream desk accessory.

| Feature | Description |
|---------|-------------|
| 🕒 **Precision Clock** | Multiple clock styles, crisp and clear digits |
| 🎵 **Real-time Spectrum** | PDM mic captures audio, FFT computes waterfall spectrum — moves with the music |
| 🌈 **Rich Effects** | Rainbow gradient, solid themes, breathing light — switch on the fly |
| 🌓 **Auto Dimming** | Photoresistor senses ambient light, auto-adjusts brightness, turns off late at night |
| 🔧 **Beginner-Friendly** | Captive Portal one-click WiFi config, web-based visual setup — no code changes needed |
| 🔄 **Multi-Mode Switching** | Press button to cycle through modes for different scenarios |
| 🔌 **Extensible** | JSON-driven mode system — write a JSON file to add a new mode |
| ☁️ **Client-Server Architecture** | Heavy computation offloaded to backend, device only displays and does real-time processing |

---

## 🎯 Available Modes

- **Digital Clock** — Clean and focused time display
- **Clock + Spectrum** — Time on bottom, music spectrum on top
- **Spectrum Visualizer** — Waterfall effect with color-coded frequency bands
- **Weather Clock** — Temperature + weather icons
- **Scrolling Text** — Quotes, poems, countdowns scrolling right to left
- **Starfield** — Twinkling stars for bedtime ambiance
- **Holiday Themes** — New Year, Christmas, Birthday specials

---

## 🔩 Hardware BOM

| Component | Recommended | Qty | Est. Cost |
|-----------|-------------|-----|-----------|
| MCU | ESP32-S3 Dev Board | 1 | ~$5-8 |
| LED Matrix | 8×32 WS2812B | 1 | ~$3-5 |
| Microphone | PDM mic module / MAX9814 | 1 | ~$1-2 |
| Light Sensor | Photoresistor module / GL5528 | 1 | ~$0.5 |
| Button | Tactile switch | 1 | ~$0.1 |
| Power | Micro USB / Type-C direct | - | - |

> Total: roughly **$10-15** for a stunning desk companion!

---

## 🧩 Architecture

```
┌─────────────────────────────────────────────────────────┐
│              Browser / Mobile Phone                      │
│  ┌─────────────┐    HTTP    ┌──────────────────┐         │
│  │ Web Config  │ ←=======→ │ FastAPI Backend   │         │
│  └─────────────┘            └──────────────────┘         │
└─────────────────────────────────────────────────────────┘
                          ↑
                      HTTP (GET /api/frame)
                          ↓
┌─────────────────────────────────────────────────────────┐
│                    ESP32-S3 (Device)                      │
│  ┌─────────┐ ┌───────────┐ ┌──────────────┐              │
│  │ WS2812  │ │ Mic + FFT │ │ Auto Dimming │              │
│  └─────────┘ └───────────┘ └──────────────┘              │
│  ┌─────────┐ ┌───────────┐ ┌──────────────┐              │
│  │ Buttons │ │ Captive   │ │ NVS Storage  │              │
│  │         │ │ Portal    │ │              │              │
│  └─────────┘ └───────────┘ └──────────────┘              │
└─────────────────────────────────────────────────────────┘
```

### Why Client-Server?

| Aspect | All-in-One (ESP32) | LightFrame (Client-Server) |
|--------|-------------------|---------------------------|
| Memory | High — parsing JSON, rendering text | Low — stores only one RGB frame |
| Power | WiFi always on, CPU loaded | Downloads frame, then sleeps |
| Extensibility | Reflash firmware for every change | Edit backend config, instant update |
| Dev Experience | Compile & flash for every tweak | Refresh browser to see changes |

---

## 🚀 Quick Start

### 1. Hardware Assembly

Connect components per the [Hardware Guide](docs/hardware.md).

### 2. Flash Firmware

```bash
cd firmware
pio run --target upload
```

### 3. Deploy Backend

```bash
cd backend
pip install -r requirements.txt
cp .env.example .env
# Fill in your weather API key (optional)
python -m uvicorn api.index:app --host 0.0.0.0 --port 8080
```

### 4. Configure

1. Power on the device. First time? Connect to the `LightFrame-XXXX` AP
2. Captive Portal pops up — enter WiFi credentials and backend URL
3. Save, device auto-reboots and connects
4. Open the web config panel, pick your favorite mode and parameters
5. Done! Enjoy your Light Frame!

---

## 📁 Project Structure

```
lightframe/
├── README.md              English intro
├── README_ZH.md           Chinese intro
├── .gitignore
├── firmware/              ESP32-S3 firmware (PlatformIO)
│   ├── src/               Source code by module
│   └── platformio.ini
├── backend/               Python FastAPI backend
│   ├── api/index.py       Entry + API endpoints
│   ├── core/              Renderer, config, registry
│   ├── modes/             Builtin + custom modes
│   └── requirements.txt
├── webconfig/             Static web config pages
│   ├── config.html
│   ├── preview.html
│   ├── editor.html
│   └── style.css
└── docs/
    ├── hardware.md        BOM + wiring
    └── setup.md           Deployment tutorial
```

---

## 🎨 Adding a Custom Mode

JSON-driven mode system — **no core code changes needed**:

```json
{
  "mode_id": "MY_CUSTOM",
  "display_name": "My Custom Mode",
  "cacheable": false,
  "content": {
    "type": "static",
    "text": "Hello World!"
  },
  "layout": {
    "color": "#FF00FF",
    "align": "center"
  }
}
```

Drop the file into `backend/modes/custom/` and restart the backend!

---

## 🤝 Acknowledgments

- Architecture inspired by [InkSight](https://github.com/datascale-ai/inksight)
- FFT spectrum processing via [arduinoFFT](https://github.com/kosme/arduinoFFT)
- Backend powered by [FastAPI](https://fastapi.tiangolo.com/)

---

## 📄 License

MIT License — free to use, modify, and distribute for personal and commercial projects.

---

## 💖 Support

If you like this project, give it a ⭐ and share it with your hardware-loving friends!

Built something cool? We'd love to see it!
