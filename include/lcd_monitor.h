#ifndef LCD_MONITOR_H
#define LCD_MONITOR_H

#include <global.h>
#include <Wire.h>
#include "DHT20.h"
#include <LiquidCrystal_I2C.h>

void initLCD();
void sensorTask(void *pvParameters);
void lcdTask(void *pvParameters);

#endif