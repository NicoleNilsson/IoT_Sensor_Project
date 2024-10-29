#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include "sensors/DHTSensor.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class MQTTManager{
public:
    MQTTManager(const char* broker, const int port, DHTSensor &dht, const char* getTopic, const char* postTopic)
                :broker(broker), port(port), dht(dht), getTopic(getTopic), postTopic(postTopic), mqttClient(secureClient){
    }
    void addSensor(const char* addSensorTopic);
    void setupMQTT();
    PubSubClient mqttClient;

private:
    const char* broker;
    const int port;
    DHTSensor &dht;
    const char* postTopic;
    const char* getTopic;
    WiFiClientSecure secureClient;
    
    void setupCertificates();
    void connect();
    void callback(char* topic, uint8_t* payload, unsigned int length);
};

#endif
