/**
 * @file storage.cpp
 * @brief NVS 配置存储实现
 */

#include "storage.h"
#include "config.h"

static Preferences prefs;
static String saved_ssid;
static String saved_password;

#define NVS_NAMESPACE "lightframe"
#define KEY_SSID "ssid"
#define KEY_PASS "pass"
#define KEY_BACKEND_HOST "backend_h"
#define KEY_BACKEND_PORT "backend_p"

void storage_init() {
    prefs.begin(NVS_NAMESPACE, false);
}

bool storage_has_wifi_config() {
    String ssid = prefs.getString(KEY_SSID, "");
    return ssid.length() > 0;
}

void storage_load_config() {
    saved_ssid = prefs.getString(KEY_SSID, "");
    saved_password = prefs.getString(KEY_PASS, "");

    String host = prefs.getString(KEY_BACKEND_HOST, "192.168.1.100");
    String port = prefs.getString(KEY_BACKEND_PORT, "8000");

    strncpy(backend_host, host.c_str(), sizeof(backend_host) - 1);
    strncpy(backend_port, port.c_str(), sizeof(backend_port) - 1);

    DEBUG_PRINTLN("Config loaded from NVS");
    DEBUG_PRINTLN("Backend: " + String(backend_host) + ":" + String(backend_port));
}

void storage_save_wifi_config(String ssid, String pass) {
    prefs.putString(KEY_SSID, ssid);
    prefs.putString(KEY_PASS, pass);
    saved_ssid = ssid;
    saved_password = pass;
}

void storage_save_backend_config(const char *host, const char *port) {
    prefs.putString(KEY_BACKEND_HOST, String(host));
    prefs.putString(KEY_BACKEND_PORT, String(port));

    strncpy(backend_host, host, sizeof(backend_host) - 1);
    strncpy(backend_port, port, sizeof(backend_port) - 1);
}

String storage_get_ssid() {
    return saved_ssid;
}

String storage_get_password() {
    return saved_password;
}
