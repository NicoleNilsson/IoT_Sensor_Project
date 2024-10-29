#include "wifi/wifi_credentials.h"
#include "wifi/WiFiManager.h"
#include "sensors/DHTSensor.h"
#include "mqtt/MQTTManager.h"
#include <ArduinoJson.h>

#define dht_pin 25
#define dht_type 11
const char* broker = "192.168.1.128";
const int port = 8883;
const char* postTopic = "sensor_reading";
const char* getTopic = "request_measurement";
const char* addSensorTopic = "add_sensor";

DHTSensor dht(dht_pin, dht_type, "livingroom");
MQTTManager mqttManager(broker, port, dht, getTopic, postTopic);

void setup() {
  Serial.begin(9600);
  WiFiManager wifiManager(ssid, password);
  wifiManager.connectToWiFi();
  dht.initiateSensor();
  mqttManager.setupMQTT();

  //mqttManager.addSensor(addSensorTopic);
}

void loop() {
  mqttManager.mqttClient.loop();
}
