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
#define LED_PIN_RSIDE       6
#define LED_PIN_RFRONT      7
#define LED_PIN_LSIDE       8
#define LED_PIN_LFRONT      9
#define LED_PIN_FRONT_TOP  10
#define LED_PIN_REAR       11


#define PWM_INPUT 3

// How many NeoPixels are attached to the Arduino?
// wrt Phyz left eye 1-12, right eye 13-24, thinking eye 25-36
#define LED_COUNT_RSIDE     143
#define LED_COUNT_RFRONT     28
#define LED_COUNT_LSIDE     143
#define LED_COUNT_LFRONT     28
#define LED_COUNT_FRONT_TOP  12
#define LED_COUNT_REAR        8



// how long do you want the system crash to be?
#define CRASH_LENGTH 10

// define a threshold for two successive readings for changing the main state
#define INTERRUPT_THRESHOLD 100

// how slowly Phyz eyes change color - smaller value for slower change
#define num_increments 1

// a measure of how fast the heart is beating; the higher the value, the faster the heartbeat [1,90]
#define heartbeat 10

// Declare our NeoPixel strip object controlling the right side lights:
Adafruit_NeoPixel strip_rside(LED_COUNT_RSIDE, LED_PIN_RSIDE, NEO_GRB + NEO_KHZ800);
// Declare our NeoPixel strip object controlling the mouth lights:
Adafruit_NeoPixel strip_rfront(LED_COUNT_RFRONT, LED_PIN_RFRONT, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, ad.d together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
// Declare our NeoPixel strip object controlling the heart lights:
Adafruit_NeoPixel strip_lside(LED_COUNT_LSIDE, LED_PIN_LSIDE, NEO_GRB + NEO_KHZ800);
// Declare our NeoPixel strip object controlling the right arm light:
Adafruit_NeoPixel strip_lfront(LED_COUNT_LFRONT, LED_PIN_LFRONT, NEO_GRB + NEO_KHZ800);
// Declare our NeoPixel strip object controlling the left arm light:
Adafruit_NeoPixel strip_front_top(LED_COUNT_FRONT_TOP, LED_PIN_FRONT_TOP, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_rear(LED_COUNT_REAR, LED_PIN_REAR, NEO_GRB + NEO_KHZ800);

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

  strip_rside.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_rside.show();            // Turn OFF all pixels ASAP
  strip_rside.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_rfront.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_rfront.show();            // Turn OFF all pixels ASAP
  strip_rfront.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_lside.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_lside.show();            // Turn OFF all pixels ASAP
  strip_lside.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_lfront.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_lfront.show();            // Turn OFF all pixels ASAP
  strip_lfront.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_front_top.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_front_top.show();            // Turn OFF all pixels ASAP
  strip_front_top.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_rear.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_rear.show();            // Turn OFF all pixels ASAP
  strip_rear.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  pinMode(PWM_INPUT, INPUT); // PWM input pin

  Serial.begin(9600);

  mp3.begin(9600);
  delay(50);  // wait chip initialization is complete

  mp3_command(CMD_SEL_DEV, DEV_TF);  // select the TF card
  delay(20);                        // wait for 200ms
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  enum OpState { NORMAL = 0, RUNNING = 1, THINKING = 2, ANGRY = 3, SHUTDOWN = 4 };
  //typedef enum OpState OpState_t ;
  OpState op_state;

  sensorValue = analogRead(sensorPin); // capture sensor input

  duration = pulseIn(PWM_INPUT, HIGH); // capture pwm input

  //op_state = OpState::NORMAL; // define a default state in case there is no PWM signal

  // Debug Serial.print("PWM Value: ");
  // Debug Serial.println(duration);

  // define op_state based on PWM inputs
  if (duration > 1900) { op_state = OpState::NORMAL;}
  else if (duration > 1700) { op_state = OpState::THINKING;}
  else if (duration > 1400) { op_state = OpState::ANGRY;}
  else if (duration > 1100) { op_state = OpState::SHUTDOWN;}
  else op_state = OpState::NORMAL;

  // sample mp3 commands
  mp3_command(CMD_PLAY, 0x0000);       // Play mp3
  //mp3_command(CMD_PAUSE, 0x0000);      // Pause mp3
  //mp3_command(CMD_PLAY_NEXT, 0x0000);  // Play next mp3
  //mp3_command(CMD_PLAY_PREV, 0x0000);  // Play previous mp3
  //mp3_command(CMD_SET_VOLUME, 30);     // Change volume to 30

  // this is the main section that decided what to do with the lights based on the
  // op_state that was decided based on the PWM frequency reading
  switch (op_state) {
    case OpState::NORMAL:
      // continuously change between green and blue
      for(int i=0; i<286; i=i+num_increments) { // For each pixel in strip...
        colorSwipeNormal(i, 10); // Green
        //colorSwipeNormal(random(0,255), 10); // random

      //  if (abs(pulseIn(PWM_INPUT, HIGH) - duration) > INTERRUPT_THRESHOLD) { break;}
      }
      for(int i=0; i<286; i=i+num_increments) { // For each pixel in strip...
        colorSwipeNormal(286-i, 10); // Green
      //  if (abs(pulseIn(PWM_INPUT, HIGH) - duration) > INTERRUPT_THRESHOLD) { break;}
      }
    break;

    case OpState::THINKING:
      // top eye is rotating through the rainbow
      theaterChaseRainbow(30); // Rainbow-enhanced theaterChase variant
      strip_rside.clear(); // lights off
    break;

    case OpState::ANGRY:
      // angry red eyes
      for(int i=0; i<255; i=i+num_increments) { // For each pixel in strip...
        colorSwipeAngry(i, 10); // Green
      //  if (abs(pulseIn(PWM_INPUT, HIGH) - duration) > INTERRUPT_THRESHOLD) { break;}
      }
      for(int i=0; i<255; i=i+num_increments) { // For each pixel in strip...
        colorSwipeAngry(255-i, 10); // Green
      //  if (abs(pulseIn(PWM_INPUT, HIGH) - duration) > INTERRUPT_THRESHOLD) { break;}
      }

      strip_rside.clear(); // lights off
    break;
    case OpState::RUNNING:
      rainbow(2);             // Flowing rainbow cycle along the whole strip
      strip_rside.clear(); // lights off

    break;
    case OpState::SHUTDOWN: // turn everything off
      strip_rside.clear();  
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
  for(int i=0; i<strip_rside.numPixels(); i++) { // For each pixel in strip...
    pixel = random(strip_rside.numPixels());
    strip_rside.setPixelColor(pixel, strip_rside.Color(random(0,255),random(0,255),random(0,255)));         //  Set pixel's color (in RAM)
    strip_rside.show();                          //  Update strip to match
    delay(wait); 
  }
}



// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  int loop_count_limit = strip_rside.numPixels();
  for(int i=0; i<loop_count_limit; i++) { // For each pixel in strip...
    strip_rside.setPixelColor(i%strip_rside.numPixels(), color);         //  Set pixel's color (in RAM)
//    if (i%strip_tail.numPixels() == 0) {strip_tail.clear();} // clear the shorter strip
//    strip_tail.setPixelColor(i%strip_tail.numPixels(), color);         //  Set pixel's color (in RAM)

    strip_rside.show();                          //  Update strip to match
//    strip_tail.show();                          //  Update strip to match

    delay(wait);                           //  Pause for a moment
  }
}

void colorSwipeNormal(int color_val, int wait) {
  int loop_count_limit = strip_rside.numPixels();

  strip_rside.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_rside.clear();
  for (int j=color_val; j<color_val+5; j++) {
    strip_rside.setPixelColor(j%loop_count_limit, strip_rside.Color(  0, 255, color_val));         //  Set pixel's color (in RAM)
    //strip_rside.setPixelColor(j%loop_count_limit, strip_rside.Color(  rand_r, rand_g, rand_b));         //  Set pixel's color (in RAM)

  }

  loop_count_limit = strip_lside.numPixels();

  strip_lside.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_lside.clear();
  for (int j=color_val; j<color_val+5; j++) {
    strip_lside.setPixelColor(j%loop_count_limit, strip_lside.Color(  0, 255, color_val));         //  Set pixel's color (in RAM)
  }


  loop_count_limit = strip_rfront.numPixels();

  strip_rfront.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_rfront.clear();
 // for(int i=0; i<loop_count_limit; i++) { // For each pixel in strip...
   // strip_rfront.setPixelColor(i%loop_count_limit, strip_rfront.Color(  255, 0, 0));         //  Set pixel's color (in RAM)
    for (int j=color_val; j<color_val+5; j++) {
      strip_rfront.setPixelColor(j%loop_count_limit, strip_rfront.Color(  0, 255, color_val));         //  Set pixel's color (in RAM)
    }
   // strip_rfront.show();                     //  Update eyes to match

  //}
  
  loop_count_limit = strip_lfront.numPixels();

  strip_lfront.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_lfront.clear();
  for(int j=color_val; j<color_val+5; j++) { // For each pixel in strip...
    strip_lfront.setPixelColor(j%loop_count_limit, strip_lfront.Color(  0, 255, color_val));         //  Set pixel's color (in RAM)
  }

  loop_count_limit = strip_front_top.numPixels();

  strip_front_top.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_front_top.clear();
  for(int j=color_val; j<color_val+5; j++) { // For each pixel in strip...
    strip_front_top.setPixelColor(j%loop_count_limit, strip_front_top.Color(  0, 255, color_val));         //  Set pixel's color (in RAM)
  }

  loop_count_limit = strip_rear.numPixels();

  strip_rear.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip_rear.clear();
  for (int j=color_val; j<color_val+5; j++) {
    strip_rear.setPixelColor(j%loop_count_limit, strip_rear.Color(  0, 255, color_val));         //  Set pixel's color (in RAM)
  }

  strip_rside.show();                     //  Update eyes to match
  strip_lside.show();                    //  update heart
  strip_rfront.show();                     //  Update eyes to match
  strip_lfront.show();                    //  update heart
  strip_front_top.show();                    //  update heart
  strip_rear.show();                    //  update heart

  delay(wait);                           //  Pause for a moment
}

void colorSwipeAngry(int color_val, int wait) {
  int loop_count_limit = strip_rside.numPixels() - 12;

  strip_rside.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
  for(int i=0; i<loop_count_limit; i++) { // For each pixel in strip...
    strip_rside.setPixelColor(i%strip_rside.numPixels(), strip_rside.Color( 255, 0, 0));         //  Set pixel's color (in RAM)
  }

  // Let's set the heartbeat
  for(int i=0; i<strip_lside.numPixels(); i++) { // For each pixel in strip...
    strip_lside.setPixelColor(i%strip_lside.numPixels(), strip_rside.Color(color_val,0,0));         //  Set pixel's color (in RAM)
  }

  strip_rside.show();                     //  Update eyes to match
  strip_lside.show();                    //  update heart
  delay(wait);                           //  Pause for a moment
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip_rside.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip_rside.numPixels(); c += 3) {
        strip_rside.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip_rside.show(); // Update strip with new contents
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
    strip_rside.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip_rside.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int upwards = 1; // direction of heartbeat intensity

  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<32; a++) {  // Repeat 32 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip_rside.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b+strip_rside.numPixels()-12; c<strip_rside.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip_rside.numPixels();
        uint32_t color = strip_rside.gamma32(strip_rside.ColorHSV(hue)); // hue -> RGB
        strip_rside.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }

      strip_rside.setPixelColor(5,  strip_rside.Color( 128, 200, 32));         //  Set pixel's color (in RAM)
      strip_rside.setPixelColor(6,  strip_rside.Color( 128, 200, 32));         //  Set pixel's color (in RAM)
      strip_rside.setPixelColor(7, strip_rside.Color( 128, 200, 32));         //  Set pixel's color (in RAM)
      strip_rside.setPixelColor(16, strip_rside.Color( 128, 200, 32));         //  Set pixel's color (in RAM)
      strip_rside.setPixelColor(17, strip_rside.Color( 128, 200, 32));         //  Set pixel's color (in RAM)
      strip_rside.setPixelColor(18, strip_rside.Color( 128, 200, 32));         //  Set pixel's color (in RAM)

      strip_rside.show();                // Update strip with new contents

     // if (abs(pulseIn(PWM_INPUT, HIGH) - duration) > INTERRUPT_THRESHOLD) { break;}

      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
      // Let's set the heartbeat
      for(int i=0; i<strip_lside.numPixels(); i++) { // For each pixel in strip...
        if (a<16) {
          strip_lside.setPixelColor(i%strip_lside.numPixels(), strip_rside.Color(a*17,0,0));         //  Set pixel's color (in RAM)
        }
        else {
          strip_lside.setPixelColor(i%strip_lside.numPixels(), strip_rside.Color((31-a)*17,0,0));         //  Set pixel's color (in RAM)      
        }
      }
      strip_lside.show();                // Update strip with new contents
        //  if (abs(pulseIn(PWM_INPUT, HIGH) - duration) > INTERRUPT_THRESHOLD) { break;}

  }
}


