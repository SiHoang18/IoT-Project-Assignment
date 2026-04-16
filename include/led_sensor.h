#ifndef LED_SENSOR_H
#define LED_SENSOR_H
#include "global.h"
#define LED_GPIO 48

void initLED();
void ledTask(void *pvParameters);

#endif