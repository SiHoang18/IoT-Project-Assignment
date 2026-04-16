#include "lcd_monitor.h"
#include "global.h"

DHT20 dht20;
LiquidCrystal_I2C lcd(33, 16, 2);

SemaphoreHandle_t stateNormalSem;
SemaphoreHandle_t stateWarningSem;
SemaphoreHandle_t stateCriticalSem;

void initLCD() {
    lcd.begin();
    lcd.backlight();
    dht20.begin();

    stateNormalSem = xSemaphoreCreateBinary();
    stateWarningSem = xSemaphoreCreateBinary();
    stateCriticalSem = xSemaphoreCreateBinary();

    if (stateNormalSem != NULL && stateWarningSem != NULL && stateCriticalSem != NULL) {
        xTaskCreate(sensorTask, "Sensor Task", 2048, NULL, 1, NULL);
        xTaskCreate(lcdTask, "LCD Task", 2048, NULL, 1, NULL);
    }
}

void sensorTask(void *pvParameters) {
    while (1) {
        dht20.read();
        float temp = dht20.getTemperature();
        float hum = dht20.getHumidity();

        if (xSemaphoreTake(sensor_mutex, portMAX_DELAY) == pdTRUE) {
            glob_temperature = temp;
            glob_humidity = hum;
            xSemaphoreGive(sensor_mutex);
        }

        if (temp >= 35.0 || hum >= 80.0) {
            xSemaphoreGive(stateCriticalSem);
        } else if (temp >= 25.0 || hum >= 60.0) {
            xSemaphoreGive(stateWarningSem);
        } else {
            xSemaphoreGive(stateNormalSem);
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void lcdTask(void *pvParameters) {
    while (1) {
        float currentTemp = 0.0;
        float currentHum = 0.0;

        if (xSemaphoreTake(sensor_mutex, portMAX_DELAY) == pdTRUE) {
            currentTemp = glob_temperature;
            currentHum = glob_humidity;
            xSemaphoreGive(sensor_mutex);
        }

        if (xSemaphoreTake(stateCriticalSem, 0) == pdTRUE) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("CRITICAL STATE!");
            lcd.setCursor(0, 1);
            lcd.print("T:"); lcd.print(currentTemp, 1); lcd.print(" H:"); lcd.print(currentHum, 1);
        } else if (xSemaphoreTake(stateWarningSem, 0) == pdTRUE) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("WARNING STATE");
            lcd.setCursor(0, 1);
            lcd.print("T:"); lcd.print(currentTemp, 1); lcd.print(" H:"); lcd.print(currentHum, 1);
        } else if (xSemaphoreTake(stateNormalSem, 0) == pdTRUE) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("NORMAL STATE");
            lcd.setCursor(0, 1);
            lcd.print("T:"); lcd.print(currentTemp, 1); lcd.print(" H:"); lcd.print(currentHum, 1);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}