/**
 * @file storage.h
 * @brief NVS 配置存储
 */

#ifndef LIGHTFRAME_STORAGE_H
#define LIGHTFRAME_STORAGE_H

#include <Preferences.h>
#include <String.h>

void storage_init();
bool storage_has_wifi_config();
void storage_load_config();
void storage_save_wifi_config(String ssid, String pass);
void storage_save_backend_config(const char *host, const char *port);
String storage_get_ssid();
String storage_get_password();

#endif // LIGHTFRAME_STORAGE_H
