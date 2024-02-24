#include "SerialMP3Player.h"

#define TX 11
#define RX 10
SerialMP3Player mp3(RX,TX);
int plcount = -1;

#define PWMRIGHTVERTICAL 2
#define PWMRIGHTHORIZONTAL 3
#define PWMRIGHTSWITCH 4

int duration_vertical; // PWM pulse width variable
int duration_horizontal; // PWM pulse width variable
int duration_switch; // PWM pulse width variable

#define HORIZONTAL_HIGH_THRESHOLD   1600
#define HORIZONTAL_MIDDLE_THRESHOLD 1400
#define HORIZONTAL_LOW_THRESHOLD    1100
#define VERTICAL_HIGH_THRESHOLD     1600
#define VERTICAL_MIDDLE_THRESHOLD   1400
#define VERTICAL_LOW_THRESHOLD      1100
#define SWITCH_HIGH_THRESHOLD       1600
#define SWITCH_MIDDLE_THRESHOLD     1400
#define SWITCH_LOW_THRESHOLD        1100

#include <Adafruit_NeoPixel.h>
#define LED_PIN    6
#define LED_COUNT 8
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
Serial.begin(9600);     // start serial interface
mp3.begin(9600);        // start mp3-communication
delay(500);             // wait for init
  
mp3.sendCommand(CMD_SEL_DEV, 0, 2);   //select sd-card
delay(500);             // wait for init

strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
strip.show();            // Turn OFF all pixels ASA
strip.setBrightness(75); // Set BRIGHTNESS to about 1/5 (max = 255)


//Play Startup Sound
mp3.play(03,100); 
delay(7000);
mp3.stop(); 

//Light Test
rainbow(10);

}

// the loop function runs over and over again forever
void loop() {

  enum SongList { StartupEngine = 03, GetOutOfTheWay = 07, HappyDance = 05, MotorSound = 02, BreakingDown = 04, Horn = 01, Alarm = 10, DanceMusic = 06 };
  SongList play_song;

  duration_vertical   = pulseIn(PWMRIGHTVERTICAL, HIGH); // capture pwm input
  duration_horizontal = pulseIn(PWMRIGHTHORIZONTAL, HIGH); // capture pwm input
  duration_switch     = pulseIn(PWMRIGHTSWITCH, HIGH); // capture pwm input


// PWM frequency to sound mapping
  if ((duration_vertical > VERTICAL_HIGH_THRESHOLD) && (duration_horizontal < HORIZONTAL_LOW_THRESHOLD)) { play_song = SongList::StartupEngine;}
  else if ((duration_vertical > VERTICAL_HIGH_THRESHOLD) && (duration_horizontal > HORIZONTAL_HIGH_THRESHOLD)) { play_song = SongList::GetOutOfTheWay;}
  else if ((duration_vertical < VERTICAL_LOW_THRESHOLD) && (duration_horizontal > HORIZONTAL_HIGH_THRESHOLD)) { play_song = SongList::HappyDance;}
  else if ((duration_vertical < VERTICAL_LOW_THRESHOLD) && (duration_horizontal < HORIZONTAL_LOW_THRESHOLD)) { play_song = SongList::MotorSound;}
  else if (duration_vertical > VERTICAL_HIGH_THRESHOLD) { play_song = SongList::BreakingDown;}
  else if (duration_vertical < VERTICAL_LOW_THRESHOLD) { play_song = SongList::Horn;}
  else if (duration_vertical > HORIZONTAL_HIGH_THRESHOLD) { play_song = SongList::Alarm;}
  else if (duration_vertical < HORIZONTAL_LOW_THRESHOLD) { play_song = SongList::DanceMusic;}
  else play_song = SongList::MotorSound;

playthesong(StartupEngine);
  switch (play_song) {
    case SongList::StartupEngine:
      //playthesong(StartupEngine, 7000, 50); 
    break;


    default:
    // statements
    break;
  }


  rainbow (10000);

}
void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
  strip.rainbow(firstPixelHue);
  strip.show();
  delay(wait);
  }
}

void playthesong(int this_song){
  //void playthesong(int this_song, int duration=2000, int volume=50){

  mp3.play(this_song); 
  delay(4000);
  mp3.stop();  
}
void playhorn(){
  mp3.play(4,100); 
  delay(4000);
  mp3.stop();  
}

void playalarm(){
  mp3.play(1,100); 
  delay(9000);
  mp3.stop();  
}

