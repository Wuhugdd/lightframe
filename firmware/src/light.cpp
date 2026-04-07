/**
 * @file light.cpp
 * @brief 光敏传感器 + 自动调光实现
 */

#include "light.h"

void light_init() {
    analogReadResolution(12);
}

int light_read_raw() {
    return analogRead(LIGHT_PIN);
}

int light_get_brightness() {
    // 多次采样取平均
    int total = 0;
    const int samples = 8;

    for (int i = 0; i < samples; i++) {
        total += light_read_raw();
        delay(1);
    }

    int raw = total / samples;

    // 阈值处理
    if (raw < LIGHT_DARK_THRESHOLD) {
        return 0;  // 关灯
    }

    // 线性映射到亮度范围
    int brightness = map(raw,
        LIGHT_DARK_THRESHOLD,
        LIGHT_BRIGHT_THRESHOLD,
        MIN_BRIGHTNESS + 10,
        MAX_BRIGHTNESS
    );

    return constrain(brightness, 0, MAX_BRIGHTNESS);
}
