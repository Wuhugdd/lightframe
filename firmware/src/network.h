/**
 * @file network.h
 * @brief WiFi + HTTP 请求
 */

#ifndef LIGHTFRAME_NETWORK_H
#define LIGHTFRAME_NETWORK_H

#include "config.h"

#define FRAME_INTERVAL_MS  33  // ~30 FPS

bool network_connect();
void network_disconnect();
bool network_is_connected();
bool network_fetch_frame();

#endif // LIGHTFRAME_NETWORK_H
