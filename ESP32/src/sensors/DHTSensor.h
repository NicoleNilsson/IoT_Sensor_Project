#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <DHT.h>
#include <PubSubClient.h>

class DHTSensor{
public:
    DHTSensor(const u_int8_t pin, const uint8_t type, const char* name)
            :pin(pin), type(type), name(name){ }
    
    void initiateSensor();
    int readDHTData(float *humidity, float *temperature);
    const char* name;

private:
    const u_int8_t pin;
    const uint8_t type;

    static int16_t waitForResponse(gpio_num_t gpio, int16_t microsToWait, uint8_t level);
    int wakeUpSensor();
    
};

#endif
