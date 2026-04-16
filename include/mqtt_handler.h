#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <Arduino.h>

void initMQTT(String server, String user, String token);
bool connectMQTT();
void mqttTask(void *pvParameters);

#endif