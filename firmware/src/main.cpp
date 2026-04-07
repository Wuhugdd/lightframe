/**
 * @file main.cpp
 * @brief LightFrame 主程序入口和状态机
 */

#include <Arduino.h>
#include "config.h"
#include "main.h"
#include "ws2812.h"
#include "mic_fft.h"
#include "light.h"
#include "network.h"
#include "portal.h"
#include "storage.h"
#include "renderer.h"

// 全局变量定义
char backend_host[64];
char backend_port[8];
DeviceState current_state = STATE_BOOT;
DisplayMode current_mode = MODE_CLOCK;
int current_brightness = DEFAULT_BRIGHTNESS;
bool auto_dimming_enabled = true;

// 前向声明
void handle_button_shortpress(int button);
void handle_button_longpress(int button);

void setup() {
    Serial.begin(115200);
    delay(1000);

    DEBUG_PRINTLN("\n=== LightFrame Starting...");

    // 初始化存储
    storage_init();

    // 加载配置
    storage_load_config();

    // 初始化WS2812
    ws2812_init();

    // 初始化麦克风FFT
    mic_fft_init();

    // 初始化光敏电阻
    light_init();

    // 初始化按键
    pinMode(BUTTON_MODE, INPUT_PULLUP);
    pinMode(BUTTON_BRIGHT, INPUT_PULLUP);
    pinMode(BUTTON_CONFIRM, INPUT_PULLUP);

    // 初始化蜂鸣器
    pinMode(BUZZER_PIN, OUTPUT);

    // 检查是否有WiFi配置
    if (!storage_has_wifi_config()) {
        DEBUG_PRINTLN("No WiFi config, starting portal...");
        current_state = STATE_PORTAL;
        portal_start();
        return;
    }

    // 尝试连接WiFi
    current_state = STATE_WIFI_CONNECT;
    if (!network_connect()) {
        DEBUG_PRINTLN("WiFi connect failed, starting portal...");
        current_state = STATE_PORTAL;
        portal_start();
        return;
    }

    current_state = STATE_NORMAL;
    DEBUG_PRINTLN("=== LightFrame Ready ===");
}

void loop() {
    switch (current_state) {
        case STATE_BOOT:
            // 启动中，不会停留在这里
            break;

        case STATE_WIFI_CONNECT:
            // 等待连接完成
            break;

        case STATE_PORTAL:
            portal_loop();
            break;

        case STATE_NORMAL:
            loop_normal();
            break;

        case STATE_SLEEP:
            // 低功耗休眠，定时唤醒拉取新帧
            delay(1000);
            break;
    }
}

void loop_normal() {
    static unsigned long last_frame = 0;
    static unsigned long last_buttons = 0;

    // 按键检测
    if (millis() - last_buttons > 20) {
        last_buttons = millis();
        // TODO: 实现按键去抖和长短按检测
    }

    // 更新亮度（自动调光）
    if (auto_dimming_enabled) {
        int target = light_get_brightness();
        ws2812_set_brightness(target);
    } else {
        ws2812_set_brightness(current_brightness);
    }

    // 定时从后端拉取新一帧
    if (millis() - last_frame > FRAME_INTERVAL_MS) {
        last_frame = millis();

        // 从后端请求新帧
        if (network_fetch_frame();
    }
}

void beep(int duration_ms) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration_ms);
    digitalWrite(BUZZER_PIN, LOW);
}
