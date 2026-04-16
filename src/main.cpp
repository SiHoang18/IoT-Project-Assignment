#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include "global.h"
#include "config_manager.h"
#include "web_server.h"
#include "mqtt_handler.h"
#include "led_sensor.h"
#include "neo_sensor.h"
#include "lcd_monitor.h"

#define BOOT_BUTTON_PIN 0

String c_ssid, c_pass, c_server, c_user, c_token;
bool isAPModeActive = true;

void hardwareMonitorTask(void *pvParameters) {
    pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
    uint32_t pressStartTime = 0;
    
    while(1) {
        if (digitalRead(BOOT_BUTTON_PIN) == LOW) {
            if (pressStartTime == 0) {
                pressStartTime = millis();
            } else if (millis() - pressStartTime > 3000) {
                wipeSystemConfig();
                Serial.println("FACTORY RESET TRIGGERED. REBOOTING...");
                ESP.restart();
            }
        } else {
            pressStartTime = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void setup() {
    Serial.begin(115200);
    delay(3000);
    Wire.begin(11, 12);
    
    sensor_mutex = xSemaphoreCreateMutex();
    
    loadSystemConfig(c_ssid, c_pass, c_server, c_user, c_token);

    if (c_ssid.length() > 0) {
        Serial.println("STA Configuration found. Attempting connection...");
        WiFi.mode(WIFI_STA);
        WiFi.begin(c_ssid.c_str(), c_pass.c_str());
        
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
            delay(500);
            attempts++;
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            initMQTT(c_server, c_user, c_token);
            if (connectMQTT()) {
                Serial.println("MQTT Connected Successfully.");
                isAPModeActive = false;
                xTaskCreate(mqttTask, "MQTT_Core_Task", 4096, NULL, 1, NULL);
            }
        }
    }

    if (isAPModeActive) {
        Serial.println("Fallback to AP Mode.");
        WiFi.disconnect();
        WiFi.mode(WIFI_AP);
        WiFi.softAP("ESP32_CoreIOT_AP");
        initWebServer();
    }

    if (sensor_mutex != NULL) {
        initLED();
        initNeoSensor();
        initLCD();
    }
    
    xTaskCreate(hardwareMonitorTask, "HW_Monitor", 2048, NULL, 1, NULL);
}

void loop() {
    vTaskDelete(NULL);
}