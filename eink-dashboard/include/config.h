#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID "TELUSWIFI0380"
#define WIFI_PASSWORD "FDMRyPqUV4"

// Display Configuration - Waveshare 7.3" Spectra 6 with DESPI-C73 adapter
// Wired to custom ESP32 PCB per J2 header:
#define EPAPER_CS     15   // J2-8
#define EPAPER_DC     14   // J2-7
#define EPAPER_RST    13   // J2-6
#define EPAPER_BUSY   12   // J2-3
#define EPAPER_MOSI   11   // J2-4 (SDI on adapter)
#define EPAPER_SCLK   10   // J2-5

// Display dimensions
#define EPAPER_WIDTH  800
#define EPAPER_HEIGHT 480

// API Configuration
#define NTP_SERVER    "pool.ntp.org"
#define TIMEZONE      "UTC-5"  // Adjust for your timezone

// OpenWeatherMap API (get free key at openweathermap.org)
#define WEATHER_API_KEY "YOUR_WEATHER_API_KEY"
#define WEATHER_CITY    "New York"
#define WEATHER_COUNTRY "US"

// Spotify API (requires OAuth setup)
#define SPOTIFY_CLIENT_ID "YOUR_SPOTIFY_CLIENT_ID"
#define SPOTIFY_CLIENT_SECRET "YOUR_SPOTIFY_CLIENT_SECRET"

// Refresh settings (seconds)
#define WEATHER_REFRESH_INTERVAL   900    // 15 minutes
#define SPOTIFY_REFRESH_INTERVAL   10     // 10 seconds (check for track changes)
#define SPOTIFY_IDLE_REFRESH       600    // 10 minutes when nothing playing
#define CLOCK_REFRESH_INTERVAL     60     // 1 minute
#define CALENDAR_REFRESH_INTERVAL  300    // 5 minutes

// Power saving
#define DEEP_SLEEP_DURATION 0  // 0 = always on, >0 = seconds to sleep

#endif
