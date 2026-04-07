/**
 * @file light.h
 * @brief 光敏传感器 + 自动调光
 */

#ifndef LIGHTFRAME_LIGHT_H
#define LIGHTFRAME_LIGHT_H

#include "config.h"

void light_init();
int light_read_raw();
int light_get_brightness();

#endif // LIGHTFRAME_LIGHT_H
