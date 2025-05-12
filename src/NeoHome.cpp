#include "NeoHome.h"

NeoHome::NeoHome() : mqttClient(espClient) {
    clientId = "esp_" + String(random(0xffff), HEX);
}

void NeoHome::begin(const char* wifiSSID, const char* wifiPassword, const char* deviceId) {
    // Connect to WiFi
    WiFi.begin(wifiSSID, wifiPassword);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");

    // Set MQTT server and callback
    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.setCallback([this](char* topic, byte* payload, unsigned int length) {
        this->callback(topic, payload, length);
    });

    // Connect to MQTT and subscribe to topics
    reconnectMQTT(deviceId);
}

void NeoHome::reconnectMQTT(const char* deviceId) {
    while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (mqttClient.connect(clientId.c_str())) {
            Serial.println("connected");
            // Subscribe to state response topic
            String stateResponseTopic = String(deviceId) + "/state/response";
            mqttClient.subscribe(stateResponseTopic.c_str());

            // Request initial state
            String stateRequestTopic = String(deviceId) + "/state/request";
            mqttClient.publish(stateRequestTopic.c_str(), "get_state");

            // Subscribe to virtual pin 4 (or others as needed)
            subscribeToTopics(deviceId, 4);
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 2 seconds");
            delay(2000);
        }
    }
}

void NeoHome::subscribeToTopics(const char* deviceId, int virtualPin) {
    String dataTopic = String(deviceId) + "/" + String(virtualPin);
    String commandTopic = String(deviceId) + "/" + String(virtualPin) + "/command";
    mqttClient.subscribe(dataTopic.c_str());
    mqttClient.subscribe(commandTopic.c_str());
    Serial.println("Subscribed to: " + dataTopic + " and " + commandTopic);
}

void NeoHome::callback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    pinValues[topic] = message;
    Serial.println("Received on " + String(topic) + ": " + message);
}

String NeoHome::read(const char* deviceId, int virtualPin) {
    if (!mqttClient.connected()) {
        reconnectMQTT(deviceId);
    }
    mqttClient.loop();

    // Check both data and command topics for the latest value
    String dataTopic = String(deviceId) + "/" + String(virtualPin);
    String commandTopic = String(deviceId) + "/" + String(virtualPin) + "/command";
    
    if (pinValues.find(commandTopic) != pinValues.end()) {
        return pinValues[commandTopic];
    } else if (pinValues.find(dataTopic) != pinValues.end()) {
        return pinValues[dataTopic];
    }
    return ""; // Return empty string if no value received yet
}

void NeoHome::write(const char* deviceId, int virtualPin, const String& value) {
    if (!mqttClient.connected()) {
        reconnectMQTT(deviceId);
    }
    String commandTopic = String(deviceId) + "/" + String(virtualPin);
    mqttClient.publish(commandTopic.c_str(), value.c_str(), true);
    mqttClient.loop();
    Serial.println("Published to " + commandTopic + ": " + value);
}