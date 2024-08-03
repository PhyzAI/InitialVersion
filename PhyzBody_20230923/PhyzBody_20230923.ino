// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <SoftwareSerial.h>

#define CMD_PLAY_NEXT 0x01
#define CMD_PLAY_PREV 0x02
#define CMD_PLAY_W_INDEX 0x03
#define CMD_SET_VOLUME 0x06
#define CMD_SEL_DEV 0x09
#define CMD_PLAY_W_VOL 0x22
#define CMD_PLAY 0x0D
#define CMD_PAUSE 0x0E
#define CMD_SINGLE_CYCLE 0x19

#define DEV_TF 0x02
#define SINGLE_CYCLE_ON 0x00
#define SINGLE_CYCLE_OFF 0x01

#define ARDUINO_RX 4  // Arduino Pin connected to the TX of the Serial MP3 Player module
#define ARDUINO_TX 5  // Arduino Pin connected to the RX of the Serial MP3 Player module

SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);


// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN_EYES    6
#define LED_PIN_MOUTH   7
#define LED_PIN_HEART   8
#define LED_PIN_R_ARM   9
#define LED_PIN_L_ARM  10


#define PWM_INPUT 3

// How many NeoPixels are attached to the Arduino?
// wrt Phyz left eye 1-12, right eye 13-24, thinking eye 25-36
#define LED_COUNT_EYES 36
#define LED_COUNT_MOUTH 6
#define LED_COUNT_HEART 6
#define LED_COUNT_R_ARM 6
#define LED_COUNT_L_ARM 6



// how long do you want the system crash to be?
#define CRASH_LENGTH 10

// define a threshold for two successive readings for changing the main state
#define INTERRUPT_THRESHOLD 100

// how slowly Phyz eyes change color - smaller value for slower change
#define num_increments 5

// Declare our NeoPixel strip object controlling the eyes lights:
Adafruit_NeoPixel strip_eyes(LED_COUNT_EYES, LED_PIN_EYES, NEO_GRB + NEO_KHZ800);
// Declare our NeoPixel strip object controlling the mouth lights:
Adafruit_NeoPixel strip_mouth(LED_COUNT_MOUTH, LED_PIN_MOUTH, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, ad.d together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
// Declare our NeoPixel strip object controlling the heart lights:
Adafruit_NeoPixel strip_heart(LED_COUNT_HEART, LED_PIN_HEART, NEO_GRB + NEO_KHZ800);
// Declare our NeoPixel strip object controlling the right arm light:
Adafruit_NeoPixel strip_r_arm(LED_COUNT_R_ARM, LED_PIN_R_ARM, NEO_GRB + NEO_KHZ800);
// Declare our NeoPixel strip object controlling the left arm light:
Adafruit_NeoPixel strip_l_arm(LED_COUNT_L_ARM, LED_PIN_L_ARM, NEO_GRB + NEO_KHZ800);

int sensorPin = A0; //input pin for the potentiometer
int sensorValue = 0; // variable to store the value coming from the sensor
int duration; // PWM pulse width variable

// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip_eyes.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_eyes.show();            // Turn OFF all pixels ASAP
  strip_eyes.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_mouth.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_mouth.show();            // Turn OFF all pixels ASAP
  strip_mouth.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_heart.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_heart.show();            // Turn OFF all pixels ASAP
  strip_heart.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_r_arm.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_r_arm.show();            // Turn OFF all pixels ASAP
  strip_r_arm.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_l_arm.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_l_arm.show();            // Turn OFF all pixels ASAP
  strip_l_arm.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  pinMode(PWM_INPUT, INPUT); // PWM input pin

  Serial.begin(9600);

  mp3.begin(9600);
  delay(50);  // wait chip initialization is complete

  mp3_command(CMD_SEL_DEV, DEV_TF);  // select the TF card
  delay(20);                        // wait for 200ms
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  enum OpState { STARTUP = 0, RUNNING = 1, RAINBOWHAPPY = 2, SYSTEMCRASH = 3, SHUTDOWN = 4 };
  //typedef enum OpState OpState_t ;
  OpState op_state;

  sensorValue = analogRead(sensorPin); // capture sensor input

  duration = pulseIn(PWM_INPUT, HIGH);

// Debug Serial.print("PWM Value: ");
// Debug Serial.println(duration);

// define op_state based on analog inputs

//if (sensorValue > 800) { op_state = OpState::STARTUP;}
//else if (sensorValue > 600) { op_state = OpState::RAINBOWHAPPY;}
//else if (sensorValue > 400) { op_state = OpState::SYSTEMCRASH;}
//else if (sensorValue > 200) { op_state = OpState::SHUTDOWN;}
//else op_state = OpState::RUNNING;
if (duration > 1900) { op_state = OpState::STARTUP;}
else if (duration > 1700) { op_state = OpState::RAINBOWHAPPY;}
else if (duration > 1400) { op_state = OpState::SYSTEMCRASH;}
else if (duration > 1100) { op_state = OpState::SHUTDOWN;}
else op_state = OpState::RUNNING;

// sample mp3 commands
mp3_command(CMD_PLAY, 0x0000);       // Play mp3
//mp3_command(CMD_PAUSE, 0x0000);      // Pause mp3
//mp3_command(CMD_PLAY_NEXT, 0x0000);  // Play next mp3
//mp3_command(CMD_PLAY_PREV, 0x0000);  // Play previous mp3
//mp3_command(CMD_SET_VOLUME, 30);     // Change volume to 30

//  op_state = OpState::STARTUP;

  switch (op_state) {
    case OpState::STARTUP:
      // continuously change between green and blue
      for(int i=0; i<127; i=i+num_increments) { // For each pixel in strip...
        colorSwipe(strip_eyes.Color(  0, 255-i,   i), 10); // Green
        if (abs(analogRead(sensorPin) - sensorValue) > INTERRUPT_THRESHOLD) { break;}

      // Do a theater marquee effect in various colors...
      //theaterChase(strip_eyes.Color(127, 127, 127), 10); // White, half brightness
      //if (abs(analogRead(sensorPin) - sensorValue) > INTERRUPT_THRESHOLD) { break;}
      }
      for(int i=0; i<127; i=i+num_increments) { // For each pixel in strip...
        colorSwipe(strip_eyes.Color(  0, i,   255-i), 10); // Green
        if (abs(analogRead(sensorPin) - sensorValue) > INTERRUPT_THRESHOLD) { break;}

      // Do a theater marquee effect in various colors...
      //theaterChase(strip_eyes.Color(127, 127, 127), 10); // White, half brightness
      //if (abs(analogRead(sensorPin) - sensorValue) > INTERRUPT_THRESHOLD) { break;}
      }
    break;
    case OpState::RAINBOWHAPPY:
      theaterChaseRainbow(2); // Rainbow-enhanced theaterChase variant
      strip_eyes.clear(); // lights off

    break;
    case OpState::SYSTEMCRASH:
    // crash sequence
      for(int i=0; i<CRASH_LENGTH; i++) { // duration defined above
        randomColors(int (50/(i+1))); //individual pixels with random colors
        if (abs(analogRead(sensorPin) - sensorValue)) { break;}

      }
      strip_eyes.clear(); // lights off
    break;
    case OpState::RUNNING:
      rainbow(2);             // Flowing rainbow cycle along the whole strip
      strip_eyes.clear(); // lights off

    break;
    case OpState::SHUTDOWN: // turn everything off
      strip_eyes.clear();  
    break;
    default:
    // statements
    break;
  }

}

// sending commands to the MP3 player

void mp3_command(int8_t command, int16_t dat) {
  int8_t frame[8] = { 0 };
  frame[0] = 0x7e;                // starting byte
  frame[1] = 0xff;                // version
  frame[2] = 0x06;                // the number of bytes of the command without starting byte and ending byte
  frame[3] = command;             //
  frame[4] = 0x00;                // 0x00 = no feedback, 0x01 = feedback
  frame[5] = (int8_t)(dat >> 8);  // data high byte
  frame[6] = (int8_t)(dat);       // data low byte
  frame[7] = 0xef;                // ending byte
  for (uint8_t i = 0; i < 8; i++) {
    mp3.write(frame[i]);
  }
}
// Some functions of our own for creating animated effects -----------------

// turn on or change the color of individual pixels
// in a randow fashion
void randomColors (int wait) {
  int pixel;
  for(int i=0; i<strip_eyes.numPixels(); i++) { // For each pixel in strip...
    pixel = random(strip_eyes.numPixels());
    strip_eyes.setPixelColor(pixel, strip_eyes.Color(random(0,255),random(0,255),random(0,255)));         //  Set pixel's color (in RAM)
    strip_eyes.show();                          //  Update strip to match
    delay(wait); 
  }
}



// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  int loop_count_limit = strip_eyes.numPixels();
  for(int i=0; i<loop_count_limit; i++) { // For each pixel in strip...
    strip_eyes.setPixelColor(i%strip_eyes.numPixels(), color);         //  Set pixel's color (in RAM)
//    if (i%strip_tail.numPixels() == 0) {strip_tail.clear();} // clear the shorter strip
//    strip_tail.setPixelColor(i%strip_tail.numPixels(), color);         //  Set pixel's color (in RAM)

    strip_eyes.show();                          //  Update strip to match
//    strip_tail.show();                          //  Update strip to match

    delay(wait);                           //  Pause for a moment
  }
}

void colorSwipe(uint32_t color, int wait) {
  int loop_count_limit = strip_eyes.numPixels() - 12;

  strip_eyes.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
  for(int i=0; i<loop_count_limit; i++) { // For each pixel in strip...
    strip_eyes.setPixelColor(i%strip_eyes.numPixels(), color);         //  Set pixel's color (in RAM)

  
    Serial.print("color Value: ");
    Serial.println(color);

  }
  strip_eyes.show();                          //  Update strip to match
  delay(wait);                           //  Pause for a moment

}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip_eyes.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip_eyes.numPixels(); c += 3) {
        strip_eyes.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip_eyes.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip_eyes.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip_eyes.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip_eyes.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip_eyes.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip_eyes.numPixels();
        uint32_t color = strip_eyes.gamma32(strip_eyes.ColorHSV(hue)); // hue -> RGB
        strip_eyes.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip_eyes.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
