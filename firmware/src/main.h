/**
 * @file main.h
 * @brief 主状态机头文件
 */

#ifndef LIGHTFRAME_MAIN_H
#define LIGHTFRAME_MAIN_H

// 设备运行状态
enum DeviceState {
    STATE_BOOT,           // 启动中
    STATE_WIFI_CONNECT,   // 连接WiFi
    STATE_NORMAL,         // 正常运行
    STATE_PORTAL,         // 配网模式
    STATE_SLEEP           // 休眠（省电）
};

// 显示模式
enum DisplayMode {
    MODE_CLOCK,           // 纯时钟
    MODE_SPECTRUM,        // 纯频谱
    MODE_CLOCK_SPECTRUM,  // 时钟+频谱组合
    MODE_CUSTOM           // 自定义模式
};

// 全局状态
extern DeviceState current_state;
extern DisplayMode current_mode;
extern int current_brightness;
extern bool auto_dimming_enabled;

// 主循环函数
void loop_normal();

#endif // LIGHTFRAME_MAIN_H
