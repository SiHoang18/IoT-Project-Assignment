#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>

void loadSystemConfig(String &ssid, String &pass, String &server, String &user, String &token);
void saveSystemConfig(String ssid, String pass, String server, String user, String token);
void wipeSystemConfig();

#endif