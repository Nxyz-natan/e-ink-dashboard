#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct SpotifyData {
    String trackName;
    String artistName;
    String albumName;
    String albumArtUrl;
    bool isPlaying;
    unsigned long progressMs;
    unsigned long durationMs;
    unsigned long timestamp;
};

class SpotifyClient {
private:
    String clientId;
    String clientSecret;
    String accessToken;
    unsigned long tokenExpiry;
    unsigned long lastUpdate;

    // Track change detection
    String lastTrackId;
    String lastArtist;
    bool wasPlaying;

    bool refreshToken();
    bool ensureToken();

public:
    SpotifyClient(const String& id, const String& secret);
    bool update();
    SpotifyData getData() const;
    bool isValid() const;

    // Returns true if track changed (needs display refresh)
    bool hasTrackChanged() const;
    bool isNothingPlaying() const;

    // Get appropriate refresh interval based on state
    unsigned long getRefreshInterval() const;
};

#endif
