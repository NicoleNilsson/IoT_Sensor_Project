#include "MQTTManager.h"
#include "sensors/DHTSensor.h"
#include "LittleFS.h"
#include <ArduinoJson.h>

static String caCert;
static String clientCert;
static String clientKey;

void MQTTManager::setupMQTT() {
    setupCertificates();
    mqttClient.setServer(broker, port);
        mqttClient.setCallback([this](char* topic, uint8_t* payload, unsigned int length) {
        this->callback(topic, payload, length);
    });
    connect();
}

void MQTTManager::connect() {
    if (getTopic == nullptr) {
        Serial.println("Error: getTopic is not initialized");
        return; 
    }

    while (!mqttClient.connected()) {
        Serial.print("Connecting to MQTT broker... ");
        if (mqttClient.connect("ESP32Client")) {
            Serial.print("Connected to broker! ");
            mqttClient.subscribe(getTopic);
            Serial.println("Subscribed to topic " + String(getTopic));
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void MQTTManager::callback(char* topic, uint8_t* payload, unsigned int length) {
    if (topic == nullptr || payload == nullptr) {
        Serial.println("Received NULL topic or payload");
        return;
    }

    String incomingMessage;
    for (int i = 0; i < length; i++) { 
        incomingMessage += (char)payload[i]; 
    }

    if (String(topic) == getTopic && incomingMessage == WiFi.macAddress()) {
        float humidity = 0;
        float temperature = 0;

        if (dht.readDHTData(&humidity, &temperature) == -1) {
            Serial.println(F("Failed to read from DHT sensor"));
            return;
        }

        StaticJsonDocument<200> doc;
        doc["macAddress"] = WiFi.macAddress();
        doc["temperature"] = temperature;
        doc["humidity"] = humidity;

        char buffer[256];
        serializeJson(doc, buffer);
        mqttClient.publish(postTopic, buffer);
        Serial.println("Temperature: " + String(temperature) + "°C " + " Humidity: " + String(humidity) 
                        + "% posted to topic " + String(postTopic));
    }
}

void MQTTManager::setupCertificates() {
    if (!LittleFS.begin()) {
      Serial.println("Failed to mount LittleFS");
      return;
    }

    File caFile = LittleFS.open("/myCA.crt", "r");
    if (!caFile) {
        Serial.println("Failed to open CA certificate file");
        return;
    }
    caCert = caFile.readString();
    secureClient.setCACert(caCert.c_str());
    caFile.close();

    File clientCertFile = LittleFS.open("/esp_client.crt", "r");
    if (!clientCertFile) {
        Serial.println("Failed to open client certificate file");
        return;
    }
    clientCert = clientCertFile.readString();
    secureClient.setCertificate(clientCert.c_str());
    clientCertFile.close();

    File clientKeyFile = LittleFS.open("/esp_client.key", "r");
    if (!clientKeyFile) {
        Serial.println("Failed to open client key file");
        return;
    }
    clientKey = clientKeyFile.readString();
    secureClient.setPrivateKey(clientKey.c_str());
    clientKeyFile.close();
}

void MQTTManager::addSensor(const char* topic){
    StaticJsonDocument<200> doc;
    doc["macAddress"] = WiFi.macAddress();
    doc["name"] = dht.name;

    char buffer[256];
    serializeJson(doc, buffer);
    mqttClient.publish(topic, buffer); //ändra till macro
}