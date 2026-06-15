#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

namespace WifiManager {
    extern volatile bool isConnected;

    // Startet die Verbindung asynchron
    void init(const char* ssid, const char* password);
    
    // Interne Callback-Funktion für WiFi-Events
    void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
}

#endif // WIFI_MANAGER_H