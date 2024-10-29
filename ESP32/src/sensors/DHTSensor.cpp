#include "DHTSensor.h"
#include <ArduinoJson.h>
#include <WiFi.h>

#define DHTPIN GPIO_NUM_25
#define DHTTYPE DHT11
#define MIN_INTERVAL 2000
#define MAX_ATTEMPTS 5

unsigned long _lastreadtime = 0;

void DHTSensor::initiateSensor() {
    gpio_set_direction(DHTPIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DHTPIN, 1);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    Serial.println("Sensor initiated.");
}

int16_t DHTSensor::waitForResponse(gpio_num_t gpio, int16_t microsToWait, uint8_t level) {
  int16_t microsWaited = 0;
  while (gpio_get_level(gpio) == level) {
    if (microsWaited++ > microsToWait) {
      return -1;
    }
    ets_delay_us(1);
  }
  return microsWaited;
}

int DHTSensor::wakeUpSensor(){
  gpio_set_direction(DHTPIN, GPIO_MODE_OUTPUT);
  gpio_set_level(DHTPIN, 0);
  ets_delay_us(20 * 1000);
  gpio_set_level(DHTPIN, 1);
  ets_delay_us(40);
  gpio_set_direction(DHTPIN, GPIO_MODE_INPUT);

  if (waitForResponse(DHTPIN, 80, 0) == -1) {
    Serial.println("Timeout waiting for first response.");
    return -1;
  }
  if (waitForResponse(DHTPIN, 80, 1) == -1) {
    Serial.println("Timeout waiting for second response.");
    return -1;
  }

  return 0;
}

int DHTSensor::readDHTData(float *humidity, float *temperature) {
  uint8_t counter = 0;

  while(counter++ < MAX_ATTEMPTS){
    if(counter > 1){
      Serial.println(F("Retrying... "));
    }

    unsigned long currenttime = millis();
    if ((currenttime - _lastreadtime) < MIN_INTERVAL) {
      vTaskDelay(MIN_INTERVAL / portTICK_PERIOD_MS);
    }
    _lastreadtime = millis();

    if(wakeUpSensor() == -1){
      continue;
    }

    int8_t data[5] = {0};
    Serial.print(F("Reading data from sensor... "));

    for (int i = 0; i < 40; i++) {
      if (waitForResponse(DHTPIN, 50, 0) == -1) {
          Serial.println(F("Timeout while reading data."));
          continue;
      }
      if (waitForResponse(DHTPIN, 70, 1) > 28) {
        data[i / 8] |= (1 << (7 - (i % 8)));
      }
    }

    Serial.print(F("Finished reading data. Validating checksum... "));

    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
      *humidity = data[0] + data[1] * 0.1;
      *temperature = data[2] + data[3] * 0.1;
      Serial.println(F("Reading succeeded!"));
      return 0;
    } else {
      Serial.println(F("Wrong checksum."));
    }
  }

  return -1;
}
