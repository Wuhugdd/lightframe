@echo off
chcp 65001 >nul
echo ========================================
echo    LightFrame Backend 启动中...
echo ========================================
call E:\Users\Clemence\miniconda3\Scripts\activate base
cd /d "%~dp0\backend"
python -m uvicorn api.index:app --host 0.0.0.0 --port 8000
pause
