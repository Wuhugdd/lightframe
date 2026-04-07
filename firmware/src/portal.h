/**
 * @file portal.h
 * @brief Captive Portal 配网
 */

#ifndef LIGHTFRAME_PORTAL_H
#define LIGHTFRAME_PORTAL_H

void portal_start();
void portal_loop();
bool portal_config_received();
void portal_save_config();

#endif // LIGHTFRAME_PORTAL_H
