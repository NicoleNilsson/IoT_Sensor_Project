#include "WiFiManager.h"
#include <WiFi.h>

void WiFiManager::connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print("Trying to connect to Wi-Fi... ");
    }
    Serial.println("Connected to Wi-Fi!");
}
