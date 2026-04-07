/**
 * @file ws2812.cpp
 * @brief WS2812 LED 点阵驱动实现
 */

#include "ws2812.h"

CRGB leds[LED_COUNT];
static int current_brightness = DEFAULT_BRIGHTNESS;

void ws2812_init() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
    FastLED.clear();
    FastLED.setBrightness(current_brightness);
    FastLED.show();
}

void ws2812_set_brightness(int brightness) {
    current_brightness = constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    FastLED.setBrightness(current_brightness);
}

void ws2812_show() {
    FastLED.show();
}

void ws2812_clear() {
    FastLED.clear();
}

void ws2812_set_pixel(int x, int y, CRGB color) {
    if (x < 0 || x >= LED_WIDTH || y < 0 || y >= LED_HEIGHT) {
        return;
    }

    // 处理之字形布线
    int index;
    if (y % 2 == 0) {
        index = y * LED_WIDTH + x;
    } else {
        index = (y + 1) * LED_WIDTH - 1 - x;
    }

    if (index >= 0 && index < LED_COUNT) {
        leds[index] = color;
    }
}

void ws2812_load_frame(const uint8_t *frame_buffer) {
    // 帧格式：每个像素 3 字节，顺序 R -> G -> B
    // 总大小: 8 * 32 * 3 = 768 字节

    for (int y = 0; y < LED_HEIGHT; y++) {
        for (int x = 0; x < LED_WIDTH; x++) {
            int buf_idx = (y * LED_WIDTH + x) * 3;
            uint8_t r = frame_buffer[buf_idx];
            uint8_t g = frame_buffer[buf_idx + 1];
            uint8_t b = frame_buffer[buf_idx + 2];
            ws2812_set_pixel(x, y, CRGB(r, g, b));
        }
    }
}
