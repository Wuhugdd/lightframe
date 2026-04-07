/**
 * @file portal.cpp
 * @brief Captive Portal 配网实现（基于 WiFiManager）
 */

#include <WiFiManager.h>
#include "portal.h"
#include "config.h"
#include "storage.h"
#include "main.h"

static WiFiManager wm;
static WiFiManagerParameter custom_backend_host("backend", "Backend Host", "", 63);
static WiFiManagerParameter custom_backend_port("port", "Backend Port", "8000", 7);
static bool config_received = false;

void portal_start() {
    String ap_name = String(AP_NAME_PREFIX) + "-" + ESP.getChipId();

    wm.addParameter(&custom_backend_host);
    wm.addParameter(&custom_backend_port);

    // 如果配网超时，保持在AP模式
    wm.setTimeout(300);

    // 启动配网门户
    bool connected = wm.autoConnect(ap_name.c_str());

    if (!connected) {
        DEBUG_PRINTLN("Portal timeout didn't get config");
    } else {
        DEBUG_PRINTLN("Portal connected with new config");
        config_received = true;
        portal_save_config();
        current_state = STATE_NORMAL;
    }
}

void portal_loop() {
    // WiFiManager 处理自己的循环
}

bool portal_config_received() {
    return config_received;
}

void portal_save_config() {
    const char *host = custom_backend_host.getValue();
    const char *port = custom_backend_port.getValue();

    strncpy(backend_host, host, sizeof(backend_host) - 1);
    strncpy(backend_port, port, sizeof(backend_port) - 1);

    // 保存WiFi和后端配置到存储
    storage_save_wifi_config(WiFi.SSID(), WiFi.psk());
    storage_save_backend_config(host, port);

    DEBUG_PRINTLN("Config saved to NVS:");
    DEBUG_PRINTLN("SSID: " + WiFi.SSID());
    DEBUG_PRINTLN("Backend: " + String(host) + ":" + String(port));
}
