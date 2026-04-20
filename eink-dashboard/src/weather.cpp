#include "weather.h"
#include <HTTPClient.h>
#include <WiFi.h>

static WeatherData currentData;

WeatherClient::WeatherClient(const String& key, const String& c, const String& co)
    : apiKey(key), city(c), country(co), lastUpdate(0) {}

bool WeatherClient::update() {
    if (!WiFi.isConnected()) {
        return false;
    }

    String url = "http://api.openweathermap.org/data/2.5/weather?q=" +
                 city + "," + country +
                 "&appid=" + apiKey +
                 "&units=metric";

    HTTPClient http;
    http.begin(url);

    int httpCode = http.GET();
    if (httpCode != 200) {
        Serial.printf("Weather API error: %d\n", httpCode);
        http.end();
        return false;
    }

    String payload = http.getString();
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        Serial.println("Weather JSON parse error");
        http.end();
        return false;
    }

    // Parse weather data
    currentData.temperature = doc["main"]["temp"].as<float>();
    currentData.humidity = doc["main"]["humidity"].as<float>();
    currentData.feelsLike = doc["main"]["feels_like"].as<float>();
    currentData.pressure = doc["main"]["pressure"].as<int>();
    currentData.windSpeed = doc["wind"]["speed"].as<int>();
    currentData.windDeg = doc["wind"]["deg"].as<int>();
    currentData.description = doc["weather"][0]["description"].as<String>();
    currentData.icon = doc["weather"][0]["icon"].as<String>();
    currentData.city = doc["name"].as<String>();
    currentData.timestamp = millis();

    http.end();
    lastUpdate = millis();

    Serial.printf("Weather updated: %.1fC, %s\n", currentData.temperature, currentData.description.c_str());
    return true;
}

WeatherData WeatherClient::getData() const {
    return currentData;
}

bool WeatherClient::isValid() const {
    return lastUpdate > 0;
}

unsigned long WeatherClient::getTimeUntilNextUpdate() const {
    return WEATHER_REFRESH_INTERVAL * 1000 - (millis() - lastUpdate);
}
