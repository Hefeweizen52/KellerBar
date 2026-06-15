#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>
#include <vector> 

void mqttInit(const char* broker, uint16_t port, std::vector<uint16_t> *pwm_dutys_ptr);
void mqttHandle(); // Muss in der loop() aufgerufen werden

#endif