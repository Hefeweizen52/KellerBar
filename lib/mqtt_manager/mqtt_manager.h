#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>
#include <vector> 

void mqttInit(const char* broker, uint16_t port);
void mqttHandle();

#endif