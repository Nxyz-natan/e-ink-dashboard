#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GfxFont.h>
#include <SPI.h>
#include "config.h"

// Fonts
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono18pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

// GDEP073E01 is a 3-color ePaper display (Spectra 6)
// Using GxEPD2_3C driver

extern GxEPD2_3C<GxEPD2_730c_GDEY073D46, GxEPD2_730c_GDEY073D46::HEIGHT> display_3c;

class EpaperDisplay {
private:
    bool _initialized = false;

public:
    EpaperDisplay() = default;

    bool begin();
    void clear();
    void display();
    void sleep();
    void wake();
    void setRotation(uint8_t r);
    void setFullWindow();
    void setPartialWindow();

    // Drawing helpers (delegate to GxEPD2)
    void fillScreen(uint16_t color);
    void drawText(int16_t x, int16_t y, const String& text, uint16_t color, uint8_t size = 1);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

    // Power management
    void setPowerSave(bool save);
    bool isSleeping();

    // Direct access to underlying display for advanced features
    GxEPD2_3C<GxEPD2_730c_GDEY073D46, GxEPD2_730c_GDEY073D46::HEIGHT>* getDisplay();
};

// Colors
#define GxEPD_WHITE     GxEPD2_3C::WHITE
#define GxEPD_BLACK     GxEPD2_3C::BLACK
#define GxEPD_YELLOW    GxEPD2_3C::YELLOW
#define GxEPD_RED       GxEPD2_3C::RED  // Actually red/orange on Spectra

#endif
