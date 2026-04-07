/**
 * @file network.cpp
 * @brief WiFi + HTTP 请求实现
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include "network.h"
#include "ws2812.h"
#include "main.h"
#include "config.h"
#include "storage.h"

static HTTPClient http;
static uint8_t frame_buffer[LED_WIDTH * LED_HEIGHT * 3];

bool network_connect() {
    DEBUG_PRINTLN("Connecting to WiFi...");

    String ssid = storage_get_ssid();
    String pass = storage_get_password();

    WiFi.begin(ssid.c_str(), pass.c_str());

    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) {
        delay(500);
        timeout++;
        DEBUG_PRINT(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        DEBUG_PRINTLN("");
        DEBUG_PRINTLN("WiFi connected!");
        DEBUG_PRINTLN("IP: " + WiFi.localIP().toString());
        return true;
    } else {
        DEBUG_PRINTLN("");
        DEBUG_PRINTLN("WiFi connect failed!");
        return false;
    }
}

void network_disconnect() {
    WiFi.disconnect();
}

bool network_is_connected() {
    return WiFi.status() == WL_CONNECTED;
}

bool network_fetch_frame() {
    if (!network_is_connected()) {
        return false;
    }

    // 构建API URL
    String url = "http://";
    url += backend_host;
    url += ":";
    url += backend_port;
    url += "/api/frame";

    DEBUG_PRINTLN("Fetching frame from: " + url);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode != HTTP_CODE_OK) {
        DEBUG_PRINTLN("Frame fetch failed, code: " + String(httpCode));
        http.end();
        return false;
    }

    // 检查大小是否正确
    int content_len = http.getSize();
    if (content_len != LED_WIDTH * LED_HEIGHT * 3) {
        DEBUG_PRINTLN("Wrong frame size: " + String(content_len));
        http.end();
        return false;
    }

    // 读取二进制帧数据
    http.getStream().readBytes(frame_buffer, content_len);

    // 加载到LED缓冲区
    ws2812_load_frame(frame_buffer);
    ws2812_show();

    http.end();
    return true;
}
