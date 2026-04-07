# LightFrame 开发会话总结

## 项目概述

LightFrame 是一个基于 **ESP32-S3 + 8×32 WS2812** 的智能炫彩桌面音乐时钟项目，采用**前后端分离架构**设计，灵感来自 InkSight。

**架构设计：**
- **ESP32 固件**：只负责硬件驱动、WiFi连接、麦克风FFT、按键、配网 → 实时性任务放本地
- **Python FastAPI 后端**：负责模式管理、内容生成（时钟/天气/文字）、RGB帧渲染、配置存储 → 复杂内容生成放后端
- **网页配置**：提供用户界面，配置设备、在线预览、自定义模式编辑

---

## 已完成工作 (2026-04-07)

### ✅ 项目结构创建

按照 `PROJECT_PLAN.md` 完整创建了目录结构：

```
lightframe/
├── firmware/                    # ESP32-S3 固件 (PlatformIO)
│   ├── include/config.h         # 硬件引脚、常量定义
│   ├── src/
│   │   ├── main.cpp/h          # 主状态机
│   │   ├── ws2812.cpp/h        # WS2812 LED驱动
│   │   ├── mic_fft.cpp/h       # 麦克风采样 + FFT计算
│   │   ├── light.cpp/h         # 光敏自动调光
│   │   ├── network.cpp/h       # WiFi + HTTP请求
│   │   ├── portal.cpp/h        # Captive Portal配网（WiFiManager）
│   │   ├── storage.cpp/h       # NVS配置存储
│   │   └── renderer.cpp/h      # 本地频谱渲染
│   └── platformio.ini
├── backend/                     # Python FastAPI 后端
│   ├── api/index.py            # FastAPI入口 + API端点
│   ├── core/
│   │   ├── mode_registry.py    # 模式注册表，加载JSON模式
│   │   ├── pipeline.py         # 统一渲染管线
│   │   ├── config_store.py     # SQLite配置存储
│   │   ├── context.py          # 上下文（时间、天气）
│   │   └── ws2812_renderer.py  # RGB帧渲染，输出二进制
│   ├── modes/
│   │   ├── builtin/
│   │   │   ├── clock_only.json
│   │   │   ├── spectrum_only.json
│   │   │   └── clock_spectrum.json
│   │   └── custom/            # 用户自定义模式
│   ├── requirements.txt
│   └── .env.example
├── webconfig/                   # 静态网页
│   ├── config.html             # 设备配置页
│   ├── preview.html            # 在线预览
│   ├── editor.html             # 自定义模式编辑器
│   └── style.css               # 现代美化样式（支持深色模式）
├── docs/                        # 文档
│   ├── hardware.md             # 硬件清单和接线
│   └── setup.md                # 部署指南
├── start_backend_conda.bat      # Windows双击启动后端（conda环境）
├── SESSION_SUMMARY.md          # 本文件 - 开发会话总结
├── PROJECT_PLAN.md             # 原始开发计划
├── CLAUDE.md                   # AI开发指引
└── .gitignore
```

### ✅ 问题修复

1. **依赖问题**：移除了 `sqlalchemy`，直接使用 Python 内置 `sqlite3`，避免 `greenlet` 编译错误
2. **Python 版本兼容**：把 `| None` 新语法改成 `Optional[]`，兼容 Python 3.9
3. **模块路径问题**：修复了导入路径，后端能正常启动
4. **预览方向问题**：修复为 **32列横向 × 8行纵向**，符合实际LED矩阵摆放
5. **样式美化**：重写 CSS，改成现代渐变圆角阴影设计，支持深色模式

### ✅ Git 初始化

- 已完成 `git init`
- 已完成首次提交：包含完整项目骨架

### ✅ 启动脚本

创建了 `start_backend_conda.bat`，Windows 下双击就能启动后端，不用每次输命令。

---

## 当前状态

- ✅ 后端能够正常启动运行
- ✅ 网页能够正常访问
- ✅ 项目骨架完整，所有文件模板已创建
- ⚠️  固件还未编译烧录
- ⚠️  硬件还未接线调试
- ⚠️  后端时钟渲染逻辑还需要补全

---

## 下一步任务

按顺序：

1. **固件编译烧录**：用 PlatformIO 打开 `firmware/`，编译烧录到 ESP32-S3
2. **硬件接线调试**：按照 `docs/hardware.md` 接线，逐个模块调试
3. **补全渲染逻辑**：在 `backend/core/pipeline.py` 和 `backend/core/ws2812_renderer.py` 中实现时钟文字渲染
4. **整体联调**：ESP32 连接后端，测试获取帧并显示
5. **部署**：部署到树莓派/实验室服务器/云服务器，让后端 24 小时运行

---

## 关键设计要点

### 前后端职责分工

| ESP32 固件 | Python 后端 |
|-----------|-------------|
| WS2812 LED驱动 | JSON模式加载管理 |
| 麦克风采样 + FFT计算 | SQLite设备配置存储 |
| 光敏自动亮度调节 | 获取时间/天气上下文 |
| 按键处理 | RGB帧渲染 |
| Captive Portal配网 | 提供API给ESP32和网页 |
| NVS存储配置 |  |

### 通信协议

- ESP32 请求：`GET /api/frame?mac={device_mac}&mode={mode_id}`
- 后端响应：`8×32×3 = 768 字节` 二进制 RGB 数据
- ESP32 直接填充缓冲区刷新，非常简单高效

### JSON 模式扩展

- 添加新模式只需要在 `backend/modes/` 添加 JSON 文件
- 不需要修改核心代码，不需要重新刷 ESP32 固件
- 架构借鉴自 [InkSight](https://github.com/datascale-ai/inksight)

---

## 开发者知识点

- 后端使用 **FastAPI** 框架，当前主流Python API框架
- 前端就是静态HTML+CSS+JavaScript，不需要打包构建，直接访问
- Git 已初始化，每次完成一个功能记得 `git add .` + `git commit -m "描述"`

---

## 常见问题

**Q: 没有公网IP，ESP32能连接到后端吗？**
A: 如果在同一个局域网，可以用内网IP。如果要在外网访问，需要内网穿透或者放有公网IP的服务器上。

**Q: 树莓派能当后端服务器吗？**
A: 完全可以，低功耗，一直开着不费电，性能足够。

**Q: 实验室服务器没有管理员权限能部署吗？**
A: 可以，pip --user 或者 conda 装依赖都不需要权限，用高位端口就行。

---

## 最后修改

- 最后修改时间：2026-04-08
- 状态：项目骨架完成，等待硬件调试和功能补全
