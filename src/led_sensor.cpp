#include "led_sensor.h"

void initLED() {
  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, LOW);
  xTaskCreate(ledTask, "LED Task", 2048, NULL, 1, NULL);
}

void ledTask(void *pvParameters) {
    int blinkDelay = 1000;
    while (true) {
        if (xSemaphoreTake(sensor_mutex, portMAX_DELAY) == pdTRUE) {  
            if (glob_temperature < 25.0) {
                blinkDelay = 1000;
            } else if (glob_temperature >= 25.0 && glob_temperature < 35.0) {
                blinkDelay = 500;
            } else {
                blinkDelay = 100;
            }
            xSemaphoreGive(sensor_mutex);
        }
        digitalWrite(LED_GPIO, !digitalRead(LED_GPIO));
        vTaskDelay(pdMS_TO_TICKS(blinkDelay));
    }
}