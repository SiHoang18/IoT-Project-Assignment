#include "mqtt_handler.h"
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "global.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

String m_server;
String m_user;
String m_token;

// Callback function for handling incoming MQTT messages (RPC requests).
// Processes device control and state queries, and sends responses.
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (unsigned int i = 0; i < length; i++) {
        msg += (char)payload[i];
    }
    
    Serial.print("RPC Topic: ");
    Serial.println(topic);
    Serial.print("RPC Payload: ");
    Serial.println(msg);

    String topicStr = String(topic);
    if (topicStr.startsWith("v1/devices/me/rpc/request/")) {
        String requestId = topicStr.substring(topicStr.lastIndexOf("/") + 1);
        String responseTopic = "v1/devices/me/rpc/response/" + requestId;

        if (msg.indexOf("setDevice1State") != -1) {
            bool state = (msg.indexOf("true") != -1);
            digitalWrite(8, state ? HIGH : LOW);
            mqttClient.publish(responseTopic.c_str(), state ? "true" : "false");
        } 
        else if (msg.indexOf("setDevice2State") != -1) {
            bool state = (msg.indexOf("true") != -1);
            digitalWrite(10, state ? HIGH : LOW);
            mqttClient.publish(responseTopic.c_str(), state ? "true" : "false");
        }
        else if (msg.indexOf("getDevice1State") != -1) {
            bool state = digitalRead(8);
            mqttClient.publish(responseTopic.c_str(), state ? "true" : "false");
        }
        else if (msg.indexOf("getDevice2State") != -1) {
            bool state = digitalRead(10);
            mqttClient.publish(responseTopic.c_str(), state ? "true" : "false");
        }
    }
}

// Initializes the MQTT client with server, user, and token credentials.
// Sets up the MQTT server and callback function.
void initMQTT(String server, String user, String token) {
    m_server = server;
    m_user = user;
    m_token = token;
    pinMode(8, OUTPUT);
    pinMode(10, OUTPUT);
    mqttClient.setServer(m_server.c_str(), 1883);
    mqttClient.setCallback(mqttCallback);
}

// Attempts to connect to the MQTT broker and subscribes to RPC request topics.
// Returns true if connection is successful, false otherwise.
bool connectMQTT() {
    String clientId = "ESP32CoreIOT-" + String(random(0xffff), HEX);
    
    if (mqttClient.connect(clientId.c_str(), m_user.c_str(), m_token.c_str())) {
        mqttClient.subscribe("v1/devices/me/rpc/request/+");
        return true;
    }
    return false;
}

// Task function to maintain MQTT connection, publish telemetry data (temperature and humidity),
// and handle reconnection if disconnected.
void mqttTask(void *pvParameters) {
    while(1) {
        if (!mqttClient.connected()) {
            Serial.println("MQTT Disconnected. Attempting reconnection...");
            if (connectMQTT()) {
                Serial.println("MQTT Reconnected Successfully");
            } else {
                vTaskDelay(pdMS_TO_TICKS(5000));
                continue;
            }
        }
        
        mqttClient.loop();

        float currentTemp = 0.0;
        float currentHum = 0.0;

        if (xSemaphoreTake(sensor_mutex, portMAX_DELAY) == pdTRUE) {
            currentTemp = glob_temperature;
            currentHum = glob_humidity;
            xSemaphoreGive(sensor_mutex);
        }

        String payload = "{\"temperature\":";
        payload += String(currentTemp, 1);
        payload += ",\"humidity\":";
        payload += String(currentHum, 1);
        payload += "}";
        
        mqttClient.publish("v1/devices/me/telemetry", payload.c_str());
        
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}