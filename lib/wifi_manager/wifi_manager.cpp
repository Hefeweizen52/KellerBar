#include "wifi_manager.h"

namespace WifiManager {
    volatile bool isConnected = false;
    const char* _ssid = nullptr;
    const char* _password = nullptr;

    void init(const char* ssid, const char* password) {
        _ssid = ssid;
        _password = password;

        // 1. Event-Callback registrieren, BEVOR WiFi gestartet wird
        WiFi.onEvent(onWiFiEvent);

        // 2. WiFi-Modus setzen und Autoconnect aktivieren
        WiFi.mode(WIFI_STA);
        WiFi.setAutoReconnect(true); 

        // 3. Verbindung asynchron anstoßen (kehrt sofort zurück!)
        WiFi.begin(_ssid, _password);
        Serial.println("[WiFi] Verbindung asynchron gestartet...");
    }

    void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
        switch (event) {
            case ARDUINO_EVENT_WIFI_STA_START:
                Serial.println("[WiFi] Station-Modus erfolgreich gestartet.");
                break;

            case ARDUINO_EVENT_WIFI_STA_CONNECTED:
                Serial.println("[WiFi] Mit Access Point verbunden. Warte auf IP...");
                break;

            case ARDUINO_EVENT_WIFI_STA_GOT_IP:
                isConnected = true;
                Serial.print("[WiFi] IP-Adresse erhalten: ");
                Serial.println(WiFi.localIP());
                break;

            case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
                isConnected = false;
                Serial.print("[WiFi] Verbindung verloren. Grund-Code: ");
                // info.wifi_sta_disconnected.reason liefert den exakten Espressif-Fehlercode
                Serial.println(info.wifi_sta_disconnected.reason);
                
                // Der ESP32 versucht dank setAutoReconnect(true) im Hintergrund selbstständig
                // die Verbindung wiederaufzubauen. Ein manuelles WiFi.begin() ist hier meistens nicht nötig.
                break;

            default:
                break;
        }
    }
}