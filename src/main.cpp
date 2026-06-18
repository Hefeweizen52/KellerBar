#include <Arduino.h>
#include "vector"

#include "pins.h"
#include "config.h"
#include "esp32_rmt.hpp"
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "control_state.h"

#include <PubSubClient.h>

volatile bool update_animation = true;

std::vector<uint16_t> pwm_dutys(6, 0);

std::vector<esp32_rmt<WS2812, GRB>> strips = {
    esp32_rmt<WS2812, GRB>(PIN_PIXEL_1, RMT_CHANNEL_0, STRIP_PIXEL_COUNT[0]),
    esp32_rmt<WS2812, GRB>(PIN_PIXEL_2, RMT_CHANNEL_1, STRIP_PIXEL_COUNT[1]),
    esp32_rmt<WS2812, GRB>(PIN_PIXEL_3, RMT_CHANNEL_2, STRIP_PIXEL_COUNT[2]),
    esp32_rmt<WS2812, GRB>(PIN_PIXEL_4, RMT_CHANNEL_3, STRIP_PIXEL_COUNT[3])};

void init_pwm();
void init_pixels();
void init_wifi();
void init_mqtt();
void init_timer();
void IRAM_ATTR onTimerCallback(void *arg);

void init_pwm()
{
    for (uint8_t i = 0; i < 6; i++)
    {
        ledcSetup(i, PWM_FREQ, PWM_RES);
        ledcAttachPin(PIN_PWM_1 + i, i);
    }
}

void init_pixels()
{
    pinMode(PIN_OE, OUTPUT);

    for (auto &strip : strips)
        strip.begin();

    for(uint8_t i = 0; i < NUM_STRIPS; i++)
        pixel_handler::register_strip(&strips[i], STRIP_PIXEL_COUNT[i]); 

    digitalWrite(PIN_OE, LOW); // Output Enable aktivieren
}

void init_wifi()
{
    WifiManager::init(MY_WIFI_SSID, MY_WIFI_PASS);
}

void init_mqtt()
{
    mqttInit(MY_MQTT_BROKER, 1883);
}

void init_timer()
{
    esp_timer_create_args_t timer_args = {
        .callback = &onTimerCallback,      // Verweis auf unsere Funktion oben
        .arg = NULL,                       // Optionale Argumente, die übergeben werden können
        .dispatch_method = ESP_TIMER_TASK, // Ausführung im High-Priority Timer-Task
        .name = "animations-timer"         // Name des Timers (hilfreich beim Debugging)
    };

    esp_timer_handle_t periodic_timer;
    esp_timer_create(&timer_args, &periodic_timer);
    esp_timer_start_periodic(periodic_timer, 10000);
}

void IRAM_ATTR onTimerCallback(void *arg)
{
    update_animation = true;
}

void setup()
{
    init_pwm();
    init_pixels();
    init_wifi();
    init_mqtt();
    init_timer();
}

void loop()
{
    if (update_animation)
    {
        pixel_handler::tick();
        update_animation = false;
    }

    mqttHandle();
}
