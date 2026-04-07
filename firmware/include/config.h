/**
 * @file config.h
 * @brief LightFrame 硬件配置和全局常量定义
 */

#ifndef LIGHTFRAME_CONFIG_H
#define LIGHTFRAME_CONFIG_H

// ========== 硬件引脚配置 ==========

// WS2812 LED 点阵
#define LED_PIN         5
#define LED_WIDTH       8
#define LED_HEIGHT      32
#define LED_COUNT       (LED_WIDTH * LED_HEIGHT)

// 麦克风（ADC引脚）
#define MIC_PIN         32
#define SAMPLES         256  // FFT采样点数

// 光敏电阻
#define LIGHT_PIN       33

// 按键
#define BUTTON_MODE     25  // 模式切换 - 短按切换模式，长按进入配网
#define BUTTON_BRIGHT   26  // 亮度调节
#define BUTTON_CONFIRM  27  // 确认

// 蜂鸣器
#define BUZZER_PIN      22

// ========== 系统配置 ==========

// 亮度配置
#define MIN_BRIGHTNESS  0
#define MAX_BRIGHTNESS  100
#define DEFAULT_BRIGHTNESS 80

// 自动调光阈值
#define LIGHT_DARK_THRESHOLD  100    // 低于此值关灯
#define LIGHT_BRIGHT_THRESHOLD 4095   // 高于此值满亮度

// FFT配置
#define FFT_BANDS       8    // 分成8个频段
#define SAMPLE_FREQ     10000 // 采样率

// ========== 网络配置 ==========

#define AP_NAME_PREFIX  "LightFrame"

// 后端API端点
// 后端地址（运行时从NVS读取）
extern char backend_host[64];
extern char backend_port[8];

// ========== 调试配置 ==========

#define DEBUG_MODE      1

#if DEBUG_MODE
#define DEBUG_PRINT(x)  Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#endif // LIGHTFRAME_CONFIG_H
