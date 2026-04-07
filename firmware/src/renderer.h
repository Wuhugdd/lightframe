/**
 * @file renderer.h
 * @brief RGB 帧渲染到 WS2812
 * 实际上这里只是占位，帧渲染主要在后端完成。
 * 本地只处理频谱模式下的直接渲染
 */

#ifndef LIGHTFRAME_RENDERER_H
#define LIGHTFRAME_RENDERER_H

#include "ws2812.h"

void renderer_render_spectrum(float *bands);

#endif // LIGHTFRAME_RENDERER_H
