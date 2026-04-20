#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>
#include <time.h>

#include "config.h"
#include "display_driver.h"
#include "weather.h"
#include "spotify.h"

// Global objects
EpaperDisplay display;
WeatherClient weather(WEATHER_API_KEY, WEATHER_CITY, WEATHER_COUNTRY);
SpotifyClient spotify(SPOTIFY_CLIENT_ID, SPOTIFY_CLIENT_SECRET);

// Timing
unsigned long lastWeatherUpdate = 0;
unsigned long lastSpotifyUpdate = 0;
unsigned long lastClockUpdate = 0;
unsigned long lastFullRefresh = 0;

// State tracking
bool nothingPlaying = false;
const unsigned long FULL_REFRESH_INTERVAL = 300000;  // Full refresh every 5 min

void connectWiFi() {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.println("IP: " + WiFi.localIP().toString());
    } else {
        Serial.println("\nWiFi connection failed!");
    }
}

void syncTime() {
    configTime(0, 0, NTP_SERVER);
    setenv("TZ", TIMEZONE, 1);
    tzset();

    struct tm timeinfo;
    int attempts = 0;
    while (!getLocalTime(&timeinfo) && attempts < 10) {
        delay(500);
        attempts++;
    }
}

String formatTime(time_t now) {
    char buffer[6];
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, 6, "%H:%M", timeinfo);
    return String(buffer);
}

String formatDate(time_t now) {
    char buffer[20];
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, 20, "%A, %b %d", timeinfo);
    return String(buffer);
}

void drawClock(time_t now) {
    auto* g = display.getDisplay();
    // Large clock centered
    g->setFont(&FreeMonoBold24pt7b);
    g->setCursor(50, 100);
    g->print(formatTime(now));

    g->setFont(&FreeMono12pt7b);
    g->setCursor(50, 145);
    g->print(formatDate(now));

    // Separator line
    g->drawLine(50, 160, 750, 160, GxEPD2_3C::BLACK);
}

void drawWeather() {
    auto* g = display.getDisplay();
    g->setFont(&FreeMonoBold12pt7b);
    g->setCursor(50, 200);
    g->print("Weather");

    if (weather.isValid()) {
        WeatherData data = weather.getData();
        g->setFont(&FreeMono18pt7b);
        g->setCursor(50, 250);
        g->print(data.temperature, 0);
        g->print(" C");

        g->setFont(&FreeMono9pt7b);
        g->setCursor(200, 255);
        g->println(data.description);

        g->setCursor(50, 285);
        g->print("Humidity: ");
        g->print(data.humidity);
        g->print("%  |  Wind: ");
        g->println(data.windSpeed);
    } else {
        g->setFont(&FreeMono9pt7b);
        g->setCursor(50, 250);
        g->println("Loading...");
    }
}

void drawSpotify() {
    auto* g = display.getDisplay();
    g->setFont(&FreeMonoBold12pt7b);
    g->setCursor(50, 340);
    g->print("Now Playing");

    if (spotify.isValid()) {
        SpotifyData data = spotify.getData();
        g->setFont(&FreeMono12pt7b);
        g->setCursor(50, 385);
        g->print(data.isPlaying ? "> " : "|| ");
        g->println(data.trackName);

        g->setFont(&FreeMono9pt7b);
        g->setCursor(70, 415);
        g->print("by ");
        g->println(data.artistName);
    } else {
        g->setFont(&FreeMono9pt7b);
        g->setCursor(50, 385);
        g->println("No playback");
    }
}

void drawCalendar() {
    // Placeholder - calendar skipped for now
    // Could add quotes, stock ticker, or other info here
}

void drawFullDisplay() {
    display.clear();

    time_t now = time(nullptr);
    drawClock(now);
    drawWeather();
    drawSpotify();

    display.display();
}

void setup() {
    Serial.begin(115200);
    Serial.println("E-Ink Dashboard Starting...");

    // Initialize display
    Serial.println("Initializing display...");
    display.begin();
    display.setRotation(1);  // Landscape orientation
    display.setFullWindow();

    // Connect to WiFi
    connectWiFi();

    // Sync time
    syncTime();

    // Initial updates
    Serial.println("Performing initial updates...");
    weather.update();
    spotify.update();

    // Draw everything
    drawFullDisplay();

    lastFullRefresh = millis();
    Serial.println("Setup complete!");
}

void loop() {
    unsigned long now = millis();
    bool needsRefresh = false;

    // Weather update (every 15 min)
    if (now - lastWeatherUpdate >= WEATHER_REFRESH_INTERVAL * 1000) {
        if (weather.update()) {
            lastWeatherUpdate = now;
            Serial.println("Weather updated");
            needsRefresh = true;
        }
    }

    // Spotify update - dynamic interval based on playback state
    unsigned long spotifyInterval = spotify.getRefreshInterval();
    if (now - lastSpotifyUpdate >= spotifyInterval) {
        if (spotify.update()) {
            lastSpotifyUpdate = now;
            Serial.println("Spotify state changed - refreshing display");
            needsRefresh = true;
        }
    }

    // Clock update (every minute)
    if (now - lastClockUpdate >= CLOCK_REFRESH_INTERVAL * 1000) {
        lastClockUpdate = now;
        // Time display updates with full refresh
    }

    // Periodic full refresh to prevent ghosting (every 5 min)
    if (now - lastFullRefresh >= FULL_REFRESH_INTERVAL) {
        Serial.println("Periodic full refresh");
        needsRefresh = true;
        lastFullRefresh = now;
    }

    // Refresh display if needed
    if (needsRefresh) {
        drawFullDisplay();
    }

    // Deep sleep if configured
    if (DEEP_SLEEP_DURATION > 0) {
        esp_sleep_enable_timer_wakeup(DEEP_SLEEP_DURATION * 1000000ULL);
        esp_deep_sleep_start();
    }

    delay(1000);
}
