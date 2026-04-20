#include "spotify.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <base64.h>

static SpotifyData currentData;

SpotifyClient::SpotifyClient(const String& id, const String& secret)
    : clientId(id), clientSecret(secret), tokenExpiry(0), lastUpdate(0),
      wasPlaying(false) {}

bool SpotifyClient::ensureToken() {
    if (millis() < tokenExpiry && accessToken.length() > 0) {
        return true;
    }
    return refreshToken();
}

bool SpotifyClient::refreshToken() {
    if (!WiFi.isConnected()) {
        return false;
    }

    String credentials = base64::encode(clientId + ":" + clientSecret);

    HTTPClient http;
    http.begin("https://accounts.spotify.com/api/token");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Authorization", "Basic " + credentials);

    String body = "grant_type=client_credentials";
    int httpCode = http.POST(body);

    if (httpCode != 200) {
        http.end();
        return false;
    }

    String payload = http.getString();
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, payload);
    http.end();

    if (error) {
        return false;
    }

    accessToken = doc["access_token"].as<String>();
    int expiresIn = doc["expires_in"];
    tokenExpiry = millis() + (expiresIn - 60) * 1000;  // Refresh 1 min early

    Serial.println("Spotify token refreshed");
    return true;
}

bool SpotifyClient::update() {
    if (!ensureToken()) {
        return false;
    }

    HTTPClient http;
    http.begin("https://api.spotify.com/v1/me/player/currently-playing");
    http.addHeader("Authorization", "Bearer " + accessToken);

    int httpCode = http.GET();
    if (httpCode != 200) {
        http.end();
        return false;
    }

    String payload = http.getString();
    StaticJsonDocument<1536> doc;
    DeserializationError error = deserializeJson(doc, payload);
    http.end();

    // "null" response means nothing is playing
    if (error || payload == "null") {
        lastUpdate = millis();
        wasPlaying = false;
        currentData.isPlaying = false;
        currentData.trackName = "";
        currentData.artistName = "";
        return true;
    }

    // Store previous track info for change detection
    String previousTrackId = lastTrackId;
    bool previousPlaying = wasPlaying;

    // Parse current track
    if (doc.containsKey("item") && !doc["item"].isNull()) {
        lastTrackId = doc["item"]["id"].as<String>();

        // Get artist name (first artist)
        lastArtist = doc["item"]["artists"][0]["name"].as<String>();

        // Get track name
        currentData.trackName = doc["item"]["name"].as<String>();
        currentData.artistName = lastArtist;
        currentData.albumName = doc["item"]["album"]["name"].as<String>();

        // Get album art URL
        if (doc["item"]["album"]["images"].size() > 0) {
            currentData.albumArtUrl = doc["item"]["album"]["images"][0]["url"].as<String>();
        }

        // Get playback state
        wasPlaying = doc["is_playing"].as<bool>();
        currentData.isPlaying = wasPlaying;

        // Get progress
        currentData.progressMs = doc["progress_ms"].as<unsigned long>();
        currentData.durationMs = doc["item"]["duration_ms"].as<unsigned long>();
    }

    lastUpdate = millis();

    // Return true if track changed or play state changed (needs refresh)
    bool trackChanged = (lastTrackId != previousTrackId);
    bool stateChanged = (wasPlaying != previousPlaying);

    if (trackChanged) {
        Serial.println("Track changed: " + currentData.trackName);
    }
    if (stateChanged) {
        Serial.println("Play state changed: " + String(wasPlaying ? "playing" : "paused"));
    }

    return trackChanged || stateChanged;
}

SpotifyData SpotifyClient::getData() const {
    return currentData;
}

bool SpotifyClient::isValid() const {
    return lastUpdate > 0;
}

bool SpotifyClient::hasTrackChanged() const {
    return true;  // update() already handles change detection
}

bool SpotifyClient::isNothingPlaying() const {
    return !wasPlaying && lastTrackId.length() == 0;
}

unsigned long SpotifyClient::getRefreshInterval() const {
    // If nothing is playing, use idle refresh (10 min)
    // Otherwise, check every 10 seconds for track changes
    if (!wasPlaying && lastTrackId.length() == 0) {
        return SPOTIFY_IDLE_REFRESH * 1000;
    }
    return SPOTIFY_REFRESH_INTERVAL * 1000;
}
