@echo off
chcp 65001 >nul
echo ========================================
echo    LightFrame Backend 启动中...
echo ========================================
cd /d "%~dp0\backend"
call activate  REM 如果用conda虚拟环境，替换成你的激活命令
python -m uvicorn api.index:app --host 0.0.0.0 --port 8000
pause
