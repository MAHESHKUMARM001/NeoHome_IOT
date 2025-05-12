#ifndef NEOHOME_H
#define NEOHOME_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <map>

class NeoHome {
public:
    NeoHome();
    void begin(const char* wifiSSID, const char* wifiPassword, const char* deviceId);
    String read(const char* deviceId, int virtualPin);
    void write(const char* deviceId, int virtualPin, const String& value);

private:
    WiFiClient espClient;
    PubSubClient mqttClient;
    const char* mqttServer = "13.42.32.58";
    const int mqttPort = 1883;
    String clientId;
    std::map<String, String> pinValues; // Store values per topic
    void callback(char* topic, byte* payload, unsigned int length);
    void reconnectMQTT(const char* deviceId);
    void subscribeToTopics(const char* deviceId, int virtualPin);
};

#endif