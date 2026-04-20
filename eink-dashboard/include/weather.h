#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct WeatherData {
    float temperature;
    float humidity;
    float feelsLike;
    int pressure;
    int windSpeed;
    int windDeg;
    String description;
    String icon;
    String city;
    unsigned long timestamp;
};

class WeatherClient {
private:
    String apiKey;
    String city;
    String country;
    unsigned long lastUpdate;

public:
    WeatherClient(const String& key, const String& c, const String& co);
    bool update();
    WeatherData getData() const;
    bool isValid() const;
    unsigned long getTimeUntilNextUpdate() const;
};

#endif
