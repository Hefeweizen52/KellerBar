#include "mqtt_manager.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "wifi_manager.h" // Nutzt das 'isConnected' Flag von vorhin
// #include "control_state.h"
#include "vector"

std::vector<uint16_t> *p_pwm_dutys;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

const char *mqttBroker = nullptr;
uint16_t mqttPort = 1883;
unsigned long lastMqttReconnectAttempt = 0;

// Hier landen alle eingehenden MQTT-Nachrichten
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    std::string payloadStd((char *)payload, length);
    std::string topicStd(topic);

    Serial.printf("[MQTT] Nachricht empfangen auf Topic: %s -> %s\n", topic, payloadStd);

    std::size_t pos{};

    if (topicStd == "Bar/PWM/1/SetDuty")
        p_pwm_dutys->at(0) = std::stoul(payloadStd, &pos);

    if (topicStd == "Bar/PWM/2/SetDuty")
        p_pwm_dutys->at(1) = std::stoul(payloadStd, &pos);

    if (topicStd == "Bar/PWM/3/SetDuty")
        p_pwm_dutys->at(2) = std::stoul(payloadStd, &pos);

    if (topicStd == "Bar/PWM/4/SetDuty")
        p_pwm_dutys->at(3) = std::stoul(payloadStd, &pos);

    if (topicStd == "Bar/PWM/5/SetDuty")
        p_pwm_dutys->at(4) = std::stoul(payloadStd, &pos);

    if (topicStd == "Bar/PWM/6/SetDuty")
        p_pwm_dutys->at(5) = std::stoul(payloadStd, &pos);
}

void mqttInit(const char *broker, uint16_t port, std::vector<uint16_t> *pwm_dutys_ptr)
{
    p_pwm_dutys = pwm_dutys_ptr;
    mqttBroker = broker;
    mqttPort = port;
    mqttClient.setServer(mqttBroker, mqttPort);
    mqttClient.setCallback(mqttCallback);
    mqttClient.setBufferSize(512); // Puffer leicht erhöhen für JSON oder längere Befehle
}

void mqttReconnectNonBlocking()
{
    // Nur versuchen zu verbinden, wenn WiFi steht
    if (!WifiManager::isConnected)
        return;

    unsigned long now = millis();
    // Wenn wir nicht verbunden sind, probieren wir es maximal alle 5 Sekunden (nicht-blockierend!)
    if (now - lastMqttReconnectAttempt > 5000)
    {
        lastMqttReconnectAttempt = now;
        Serial.println("[MQTT] Versuche Verbindung zum Broker...");

        // Eindeutige Client-ID generieren
        String clientId = "ESP32S3-Client-" + String(random(0xffff), HEX);

        if (mqttClient.connect(clientId.c_str()))
        {
            Serial.println("[MQTT] Erfolgreich verbunden!");

            // Sofort alle benötigten Command-Topics abonnieren
            mqttClient.subscribe("Bar/#");
        }
        else
        {
            Serial.printf("[MQTT] Fehlgeschlagen, RC=%d. Nächster Versuch in 5s.\n", mqttClient.state());
        }
    }
}

void mqttHandle()
{
    if (!mqttClient.connected())
    {
        mqttReconnectNonBlocking();
    }
    else
    {
        mqttClient.loop(); // Verarbeitet eingehende Pakete und hält die Verbindung per Ping stabil
    }
}