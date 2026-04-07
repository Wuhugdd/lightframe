/**
 * @file ws2812.h
 * @brief WS2812 LED 点阵驱动
 */

#ifndef LIGHTFRAME_WS2812_H
#define LIGHTFRAME_WS2812_H

#include <FastLED.h>
#include "config.h"

// 帧缓冲区 - 8x32 = 256 像素，每个像素 3 字节 RGB
extern CRGB leds[LED_COUNT];

void ws2812_init();
void ws2812_set_brightness(int brightness);
void ws2812_show();
void ws2812_clear();

// 设置某个像素颜色（使用矩阵坐标）
void ws2812_set_pixel(int x, int y, CRGB color);

// 从后端接收的二进制帧填充缓冲区
void ws2812_load_frame(const uint8_t *frame_buffer);

#endif // LIGHTFRAME_WS2812_H
