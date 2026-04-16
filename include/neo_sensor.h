#ifndef NEO_SENSOR_H
#define NEO_SENSOR_H

#include "global.h"
#include "Adafruit_NeoPixel.h"

#define NEO_GPIO 45
#define NEO_NUM_LEDS 1
void initNeoSensor();
void neoSensorTask(void *pvParameters);
#endif