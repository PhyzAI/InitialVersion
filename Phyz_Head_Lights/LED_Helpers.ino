

/** @brief Sets entire strip to specified color value and updates
 *  @param strip The strip to change color on
 *  @param color uint_32 color value to update color to
 *  @return Void.
 */
void setStripToColor(Adafruit_NeoPixel& strip, uint32_t color) {
    for (int n = 0; n < strip.numPixels(); n++) {
        strip.setPixelColor(n, color);
    }
    strip.show();
}

/** @brief Sets entire strip to specified color value and updates
 *  @param strip The strip to change color on
 *  @param r color red value
 *  @param g color red value
 *  @param b color red value
 *  @return Void.
 */
void setStripToColor(Adafruit_NeoPixel& strip, uint8_t r, uint8_t g, uint8_t b) {
    for (int n = 0; n < strip.numPixels(); n++) {
        strip.setPixelColor(n, r, g, b);
    }
    strip.show();
}

class 

