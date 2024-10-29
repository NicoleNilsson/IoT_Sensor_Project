#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

class WiFiManager{
public:
    WiFiManager(const char* ssid, const char* password)
                :ssid(ssid), password(password){}

    void connectToWiFi();
private:
    const char* ssid;
    const char* password;
};

#endif
