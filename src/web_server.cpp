#include "web_server.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "config_manager.h"

#define LED1_PIN 8
#define LED2_PIN 10

AsyncWebServer server(80);

void rebootTask(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(1500));
    ESP.restart();
}

void initWebServer() {
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    
    if (!LittleFS.begin(true)) return;

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Cache-Control", "no-cache, no-store, must-revalidate");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(LittleFS, "/index.html", "text/html"); });
    server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(LittleFS, "/styles.css", "text/css"); });
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(LittleFS, "/script.js", "text/javascript"); });

    server.on("/api/control", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("device") && request->hasParam("action")) {
            String deviceId = request->getParam("device")->value();
            String action = request->getParam("action")->value();
            int targetPin = (deviceId == "1") ? LED1_PIN : LED2_PIN;
            int state = (action == "on") ? HIGH : LOW;
            digitalWrite(targetPin, state);
            request->send(200, "text/plain", "Success");
        } else {
            request->send(400, "text/plain", "Bad Request");
        }
    });

    server.on("/api/config", HTTP_POST, [](AsyncWebServerRequest *request){
        String ssid = request->hasParam("ssid", true) ? request->getParam("ssid", true)->value() : "";
        String pass = request->hasParam("pass", true) ? request->getParam("pass", true)->value() : "";
        String srv = request->hasParam("server", true) ? request->getParam("server", true)->value() : "";
        String usr = request->hasParam("user", true) ? request->getParam("user", true)->value() : "";
        String tkn = request->hasParam("token", true) ? request->getParam("token", true)->value() : "";
        
        saveSystemConfig(ssid, pass, srv, usr, tkn);
        request->send(200, "text/plain", "Config Saved");
        xTaskCreate(rebootTask, "Reboot_Task", 2048, NULL, 5, NULL);
    });

    server.begin();
}