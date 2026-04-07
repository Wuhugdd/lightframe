# LightFrame 部署指南

## 环境要求

- ESP32-S3 开发环境（PlatformIO）
- Python 3.8+ 用于后端
- 局域网内一台电脑/树莓派运行后端

## 步骤 1：编译烧录固件

1. 打开 `firmware/` 目录
2.用 PlatformIO 打开项目
3. 修改 `include/config.h` 中引脚定义（如果你的接线不同）
4. 编译并烧录到 ESP32-S3

## 步骤 2：配置网络和后端

### 首次启动配网

1. ESP 首次启动没有配置，会自动开启 AP：`LightFrame-XXXXX`
2. 手机连接这个 AP，会自动弹出配网页面
3. 输入你的 WiFi SSID、密码，以及后端服务的 IP 地址和端口
4. 保存后 ESP 会重启并连接 WiFi

### 启动后端服务

```bash
# 进入后端目录
cd backend

# 创建虚拟环境（可选但推荐）
python -m venv venv
source venv/bin/activate  # Windows: venv\Scripts\activate

# 安装依赖
pip install -r requirements.txt

# 复制环境变量文件
cp .env.example .env
# 编辑 .env，设置你的时区等

# 启动服务
python -m uvicorn api.index:app --host 0.0.0.0 --port 8000
```

## 步骤 3：网页配置

打开浏览器访问 `http://<backend-ip>:8000/config/config.html`

在配置页面：
1. 选择要启用的显示模式
2. 调整默认亮度
3. 设置是否开启自动调光
4. 选择频谱配色
5. 保存配置

## 步骤 4：使用预览

打开 `http://<backend-ip>:8000/config/preview.html` 可以在浏览器中实时预览显示效果。

## 后台运行（可选）

使用 systemd 或者 pm2 让后端在后台运行：

### 使用 systemd

创建 `/etc/systemd/system/lightframe.service`:

```ini
[Unit]
Description=LightFrame Backend
After=network.target

[Service]
Type=simple
User=pi
WorkingDirectory=/home/pi/lightframe/backend
ExecStart=/home/pi/lightframe/venv/bin/python -m uvicorn api.index:app --host 0.0.0.0 --port 8000
Restart=always

[Install]
WantedBy=multi-user.target
```

然后：

```bash
sudo systemctl daemon-reload
sudo systemctl enable lightframe
sudo systemctl start lightframe
```

## 常见问题

**Q: ESP 无法连接 WiFi**
A: 长按按键进入配网模式，重新配置

**Q: 无法获取帧**
A: 检查后端 IP 是否正确，防火墙是否开放 8000 端口

**Q: 频谱跳动不明显**
A: 调整麦克风灵敏度，或者检查 FFT 噪声门阈值
