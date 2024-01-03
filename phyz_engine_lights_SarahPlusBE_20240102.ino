// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN1        6 // On Trinket or Gemma, suggest changing this to 1
#define PIN2        4 // On Trinket or Gemma, suggest changing this to 1
#define PIN3       2 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS1 16 // Popular NeoPixel ring size
#define NUMPIXELS2 149

// divider to determine the number of lights lit at a time
#define lightskip 2

int curpin2_1 = 37;
int curpin2_2 = 74;
int curpin2_3 = 111;
int curpin2_4 = 150;
int num = 50; // BE orig 50

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels2(NUMPIXELS2, PIN2, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels2.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {

  pixels2.clear(); // BE

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.

  // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  // Here we're using a moderately bright green color:

  if(curpin2_1 == 0) {
    curpin2_1 = 37;
  }
  if(curpin2_2 == 38) {
    curpin2_2 = 74;
  }
  if(curpin2_3 == 75) {
    curpin2_3 = 111;
  }
  if(curpin2_4 == 112) {
    curpin2_1 = 37;
    curpin2_2 = 74;
    curpin2_3 = 111;
    curpin2_4 = 150;
    pixels2.clear();
    if(num <= 1) { // BE orig 1
      num = 1; // BE orig 1
    }
    else {
      num = num - 2;
    }
  }
  //pixels2.setBrightness(10); // max 255 - BE

  pixels2.setPixelColor(curpin2_1, pixels2.Color(255, 255, 255));
  pixels2.setPixelColor(curpin2_1-1, pixels2.Color(255, 255, 255));
  pixels2.setPixelColor(curpin2_1-2, pixels2.Color(255, 255, 255));

  pixels2.setPixelColor(curpin2_2, pixels2.Color(255, 255, 255));
  pixels2.setPixelColor(curpin2_2-1, pixels2.Color(255, 255, 255));
  pixels2.setPixelColor(curpin2_2-2, pixels2.Color(255, 255, 255));
  
  pixels2.setPixelColor(curpin2_3, pixels2.Color(255, 255, 255));
  pixels2.setPixelColor(curpin2_3-1, pixels2.Color(255, 255, 255));
  pixels2.setPixelColor(curpin2_3-2, pixels2.Color(255, 255, 255));

  pixels2.setPixelColor(curpin2_4, pixels2.Color(255, 255, 255));
  pixels2.setPixelColor(curpin2_4-1, pixels2.Color(255, 255, 255));
  pixels2.setPixelColor(curpin2_4-2, pixels2.Color(255, 255, 255));

  curpin2_1--;
  curpin2_2--;
  curpin2_3--;
  curpin2_4--;

  //curpin2_1 = curpin2_1 - lightskip; // BE
  //curpin2_2 = curpin2_2 - lightskip; // BE
  //curpin2_3 = curpin2_3 - lightskip; // BE
  //curpin2_4 = curpin2_4 - lightskip; // BE

  pixels2.show();   // Send the updated pixel colors to the hardware.

  delay(num); // Pause before next pass through loop
}
