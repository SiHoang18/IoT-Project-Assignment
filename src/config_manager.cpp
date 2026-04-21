#include "config_manager.h"
#include <Preferences.h>

Preferences prefs;

// Loads system configuration parameters from non-volatile storage (Preferences).
// Retrieves SSID, password, server, user, and token values.
void loadSystemConfig(String &ssid, String &pass, String &server, String &user, String &token) {
    prefs.begin("coreiot_cfg", true);
    ssid = prefs.getString("ssid", "");
    pass = prefs.getString("pass", "");
    server = prefs.getString("server", "spp.coreiot.io");
    user = prefs.getString("user", "");
    token = prefs.getString("token", "");
    prefs.end();
}

// Saves system configuration parameters to non-volatile storage (Preferences).
// Stores SSID, password, server, user, and token values.
void saveSystemConfig(String ssid, String pass, String server, String user, String token) {
    prefs.begin("coreiot_cfg", false);
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    prefs.putString("server", server);
    prefs.putString("user", user);
    prefs.putString("token", token);
    prefs.end();
}

// Clears all system configuration parameters from non-volatile storage (Preferences).
void wipeSystemConfig() {
    prefs.begin("coreiot_cfg", false);
    prefs.clear();
    prefs.end();
}