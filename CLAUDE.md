# CLAUDE 开发指引 - LightFrame

## 项目概述

LightFrame 是一个基于 ESP32-S3 + 8×32 WS2812 的智能炫彩桌面音乐时钟，采用前后端分离架构：

- **firmware/** - ESP32-S3 固件，C++ Arduino 框架，PlatformIO 项目
- **backend/** - Python FastAPI 后端，处理模式管理和帧渲染
- **webconfig/** - 静态网页，配置界面和预览
- **docs/** - 文档

## 架构设计

职责分离：
- ESP32 固件：WiFi连接、按键处理、LED驱动、麦克风采样、FFT计算、光敏调光、配网
- Python 后端：模式管理、内容生成（时钟/文字/天气）、RGB帧渲染、配置存储
- 网页：用户配置、在线预览

通信协议：
- ESP32 请求 GET `/api/frame?mac={mac}&mode={mode}`
- 后端返回 8×32×3 = 768 字节二进制 RGB 数据
- ESP32 直接填充缓冲区刷新 LED

## 代码规范

- 固件：C++17，按模块拆分，每个模块一对 .h .cpp
- 后端：Python 3.10+，遵循 PEP8
- 文件头要有注释说明文件用途
- 函数要有简短注释说明功能

## 开发顺序

1. 硬件调试：先单独调通每个硬件
2. 固件基础：WiFi配网、NVS存储
3. 后端基础：FastAPI 框架、API端点
4. 基础模式：时钟、频谱
5. 进阶功能：天气、滚动文字

## 修改指南

- 添加新模式：在 `backend/modes/builtin/` 添加 JSON 文件，或通过网页编辑器添加自定义
- 修改硬件引脚：编辑 `firmware/include/config.h`
- 修改配色方案：在 `backend/core/ws2812_renderer.py` 添加新方案
