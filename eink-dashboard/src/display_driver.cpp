#include "display_driver.h"

// Define the display instance for GDEP073E01 (7.3" Spectra 6)
// The GDEY073D46 is compatible with the GDEP073E01 used in Waveshare displays
GxEPD2_3C<GxEPD2_730c_GDEY073D46, GxEPD2_730c_GDEY073D46::HEIGHT> display_3c(
    EPAPER_CS, EPAPER_DC, EPAPER_RST, EPAPER_BUSY
);

bool EpaperDisplay::begin() {
    if (_initialized) return true;

    // Initialize the adapter reset pin if defined
#ifdef EPAPER_ADAPTER_RST
    pinMode(EPAPER_ADAPTER_RST, OUTPUT);
    digitalWrite(EPAPER_ADAPTER_RST, HIGH);
#endif

    display_3c.init(115200);
    _initialized = true;
    return true;
}

void EpaperDisplay::clear() {
    display_3c.fillScreen(GxEPD2_3C::WHITE);
}

void EpaperDisplay::display() {
    display_3c.display();
}

void EpaperDisplay::sleep() {
    display_3c.powerOff();
}

void EpaperDisplay::wake() {
    display_3c.powerOn();
}

void EpaperDisplay::setRotation(uint8_t r) {
    display_3c.setRotation(r);
}

void EpaperDisplay::setFullWindow() {
    display_3c.setFullWindow();
}

void EpaperDisplay::setPartialWindow() {
    // Note: partial window may not be supported on all 3-color displays
    display_3c.setPartialWindow();
}

void EpaperDisplay::fillScreen(uint16_t color) {
    display_3c.fillScreen(color);
}

void EpaperDisplay::drawText(int16_t x, int16_t y, const String& text, uint16_t color, uint8_t size) {
    display_3c.setTextColor(color);
    display_3c.setTextSize(size);
    display_3c.setCursor(x, y);
    display_3c.print(text);
}

void EpaperDisplay::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    display_3c.drawRect(x, y, w, h, color);
}

void EpaperDisplay::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    display_3c.drawLine(x0, y0, x1, y1, color);
}

void EpaperDisplay::setPowerSave(bool save) {
    if (save) {
        sleep();
    } else {
        wake();
    }
}

bool EpaperDisplay::isSleeping() {
    return !display_3c.epaper2.isPowerOn();
}

GxEPD2_3C<GxEPD2_730c_GDEY073D46, GxEPD2_730c_GDEY073D46::HEIGHT>* EpaperDisplay::getDisplay() {
    return &display_3c;
}
