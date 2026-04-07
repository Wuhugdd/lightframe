/**
 * @file renderer.cpp
 * @brief 本地频谱渲染（当使用纯频谱模式时）
 */

#include "renderer.h"
#include "config.h"

// 简易彩虹渐变
static CRGB rainbow_at(float position) {
    position = fmod(position, 1.0f);
    float h = position * 360.0f;
    float s = 1.0f;
    float v = 1.0f;

    float c = v * s;
    float x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
    float m = v - c;

    float r, g, b;
    if (h < 60) { r = c + m; g = x + m; b = 0 + m; }
    else if (h < 120) { r = x + m; g = c + m; b = 0 + m; }
    else if (h < 180) { r = 0 + m; g = c + m; b = x + m; }
    else if (h < 240) { r = 0 + m; g = x + m; b = c + m; }
    else if (h < 300) { r = x + m; g = 0 + m; b = c + m; }
    else { r = c + m; g = 0 + m; b = x + m; }

    return CRGB((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255));
}

void renderer_render_spectrum(float *bands) {
    ws2812_clear();

    for (int band = 0; band < FFT_BANDS; band++) {
        float height = bands[band];
        // 映射到高度（总共 LED_HEIGHT 行）
        int pixel_height = (int)(height * LED_HEIGHT / 100.0f);
        pixel_height = constrain(pixel_height, 0, LED_HEIGHT);

        // 每个频段占 x 方向 4 像素（8频段 x 4 = 32）
        int x_start = band * 4;

        for (int h = 0; h < pixel_height && h < LED_HEIGHT; h++) {
            int y = h;
            float rainbow_pos = (float)band / FFT_BANDS + (float)h / LED_HEIGHT * 0.3f;
            CRGB color = rainbow_at(rainbow_pos);

            for (int dx = 0; dx < 4; dx++) {
                ws2812_set_pixel(x_start + dx, y, color);
            }
        }
    }

    ws2812_show();
}
