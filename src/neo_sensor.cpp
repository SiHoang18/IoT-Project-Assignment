#include "neo_sensor.h"

// Initializes the NeoPixel sensor and starts the NeoPixel control task.
void initNeoSensor() {
    xTaskCreate(neoSensorTask, "Neo Sensor Task", 2048, NULL, 1, NULL);
}

// Task function to control the NeoPixel LED color based on the current temperature value.
void neoSensorTask(void *pvParameters) {
    Adafruit_NeoPixel strip(NEO_NUM_LEDS, NEO_GPIO, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.clear();
    strip.show();
    while (true) {
        if (xSemaphoreTake(sensor_mutex, portMAX_DELAY) == pdTRUE) {  
            if (glob_temperature < 25.0) {
                strip.setPixelColor(0, strip.Color(0, 255, 0));
            } else if (glob_temperature >= 25.0 && glob_temperature < 35.0) {
                strip.setPixelColor(0, strip.Color(255, 255, 0)); 
            } else {
                strip.setPixelColor(0, strip.Color(255, 0, 0)); 
            }
            strip.show();
            xSemaphoreGive(sensor_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}