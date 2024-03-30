

/** @brief Sets entire strip to specified color value and updates
 *  @param strip The strip to change color on
 *  @param color uint_32 color value to update color to
 *  @return Void
 */
void setStripToColor(Adafruit_NeoPixel& strip, uint32_t color) {
    for (int n = 0; n < strip.numPixels(); n++) {
        strip.setPixelColor(n, color);
    }
    strip.show();
}

/** @brief Sets entire strip to specified color value and updatess
 *  @param strip The strip to change color on
 *  @param r color red value
 *  @param g color red value
 *  @param b color red value
 *  @return Void
 */
void setStripToColor(Adafruit_NeoPixel& strip, uint8_t r, uint8_t g, uint8_t b) {
    for (int n = 0; n < strip.numPixels(); n++) {
        strip.setPixelColor(n, r, g, b);
    }
    strip.show();
}

class LedStripLightShow {
    private:
        Adafruit_NeoPixel& strip;
        unsigned long millisAtStart;
        int millisToDelay;

    public:
        enum ShowState {NONE = 0, THEATER_CHASE = 1, RAINBOW_THEATER_CHASE = 2, RAINBOW = 3, SOLID = 4};
        ShowState currentState = NONE;
        uint32_t color = 0xFF0000; // Red by default

        LedStripLightShow(Adafruit_NeoPixel& s) {
            strip = s;
            millisAtStart = millis();
            millisToDelay = 10;
        }

        LedStripLightShow(Adafruit_NeoPixel& s, int delay) {
            strip = s;
            millisAtStart = millis();
            millisToDelay = delay;
        }

        // Call me to start a new type of show
        void setShow(int showState) {
            currentState = showState; // Update which show we are playing rn

            millisAtStart = millis();

            // Initial Setup for a show
            switch (currentState) {
                // case THEATER_CHASE:
                //     // generate
                //     // output led
                //     break;
                case RAINBOW_THEATER_CHASE:
                case RAINBOW:
                    color = 0xFF0000; // Start the rainbow at red
                    break;

            }
        }

        // function to check for what type of show needed
        // if (function = rainbow) --> state = whatever number rainbow is
        
        // Updates this strand
        uint8_t updateLEDs() {
            
            switch (currentState) {
                case NONE:
                    setStripToColor(strip, 0x000000);
                    break;
                case THEATER_CHASE:
                    // generate
                    // output led
                    break;
                case RAINBOW_THEATER_CHASE:
                    // generate
                    // output led
                    break;
                case RAINBOW: // Generates a solid rainbow on all leds in the strip
                    for (int i = 0; i < strip.numPixels(); i++) {
                        strip.getPixelColor(i)
                    }
                    // output led
                    break;
                case SOLID: // Applies color to entire strip
                    setStripToColor(strip, color);
                    // output led
                    break;
            }
            
            strip.show();
        }
};



// void I2Ccommand (int command){
//  switch (command){
//   case x:
//   // do x command
//   break;
//   case x:
//   // do x command
//   break;
//   case x:
//   // do x command
//   break;
//  } 
//  }
// }
