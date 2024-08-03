#include "SerialMP3Player.h"

#define TX 11
#define RX 10
SerialMP3Player mp3(RX,TX);
int plcount = -1;

#define PWMRIGHTVERTICAL   2 
#define PWMRIGHTHORIZONTAL 3
#define PWMRIGHTSWITCH     4

#define HEADLIGHTS    5
#define BLINKER_LEFT  7
#define BLINKER_RIGHT 6

unsigned long duration_vertical; // PWM pulse width variable
unsigned long duration_horizontal; // PWM pulse width variable
unsigned long duration_switch; // PWM pulse width variable

#define HORIZONTAL_HIGH_THRESHOLD   1650
#define HORIZONTAL_MIDDLE_THRESHOLD 1400
#define HORIZONTAL_LOW_THRESHOLD    1280
#define VERTICAL_HIGH_THRESHOLD     1650
#define VERTICAL_MIDDLE_THRESHOLD   1400
#define VERTICAL_LOW_THRESHOLD      1280
#define SWITCH_HIGH_THRESHOLD       1650
#define SWITCH_MIDDLE_THRESHOLD     1400
#define SWITCH_LOW_THRESHOLD        1280

#include <Adafruit_NeoPixel.h>
#define LED_PIN   9
#define LED_COUNT 8
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void playthesong(int this_song=2, int this_duration=2000, int this_volume=50);
enum SongList { StartupEngine = 03, GetOutOfTheWay = 07, HappyDance = 05, MotorSound = 02, BreakingDown = 04, Horn = 01, Alarm = 10, DanceMusic = 06, NoSong=00 };

void setup() {
  pinMode(HEADLIGHTS, OUTPUT);
  pinMode(BLINKER_LEFT, OUTPUT);
  pinMode(BLINKER_RIGHT, OUTPUT);

  digitalWrite(HEADLIGHTS, LOW);
  digitalWrite(BLINKER_LEFT, HIGH);
  digitalWrite(BLINKER_RIGHT, HIGH);

  Serial.begin(9600);     // start serial interface
  mp3.begin(9600);        // start mp3-communication
  delay(500);             // wait for init
    
  mp3.sendCommand(CMD_SEL_DEV, 0, 2);   //select sd-card
  delay(500);             // wait for init

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASA
  strip.setBrightness(75); // Set BRIGHTNESS to about 1/5 (max = 255)

  //Light Test
  // rainbow(1);

  //Play Startup Sound
  //mp3.play(03,100); 
  //delay(7000);
  mp3.stop(); 

  playthesong(StartupEngine, 7000, 50); 


}


// the loop function runs over and over again forever
void loop() {

  SongList play_song;

  duration_vertical   = pulseIn(PWMRIGHTVERTICAL, HIGH, 100000); // capture pwm input
  duration_horizontal = pulseIn(PWMRIGHTHORIZONTAL, HIGH, 100000); // capture pwm input
  duration_switch     = pulseIn(PWMRIGHTSWITCH, HIGH, 100000); // capture pwm input
  Serial.println(duration_vertical);
  Serial.println(duration_horizontal);
  Serial.println(duration_switch);
  Serial.println("hai");

// lights mapping
  if (duration_switch > SWITCH_HIGH_THRESHOLD) { digitalWrite(HEADLIGHTS, LOW); } // headlights on
  else if (duration_switch < SWITCH_LOW_THRESHOLD) {
    digitalWrite(BLINKER_LEFT, LOW);
    digitalWrite(BLINKER_RIGHT, LOW);}
  else {  
    digitalWrite(HEADLIGHTS, HIGH);
    digitalWrite(BLINKER_LEFT, HIGH);
    digitalWrite(BLINKER_RIGHT, HIGH);}

// PWM frequency to sound mapping
  if ((duration_vertical > VERTICAL_HIGH_THRESHOLD) && (duration_horizontal < HORIZONTAL_LOW_THRESHOLD)) { play_song = SongList::MotorSound;} 
  else if ((duration_vertical > VERTICAL_HIGH_THRESHOLD) && (duration_horizontal > HORIZONTAL_HIGH_THRESHOLD)) { play_song = SongList::GetOutOfTheWay;}
  else if ((duration_vertical < VERTICAL_LOW_THRESHOLD) && (duration_horizontal > HORIZONTAL_HIGH_THRESHOLD)) { play_song = SongList::HappyDance;}
  else if ((duration_vertical < VERTICAL_LOW_THRESHOLD) && (duration_horizontal < HORIZONTAL_LOW_THRESHOLD)) { play_song = SongList::StartupEngine;}
  else if (duration_vertical > VERTICAL_HIGH_THRESHOLD) { play_song = SongList::BreakingDown;} // up
  else if (duration_vertical < VERTICAL_LOW_THRESHOLD) { play_song = SongList::Horn;}   // down
  else if (duration_horizontal > HORIZONTAL_HIGH_THRESHOLD) { play_song = SongList::Alarm;}  // right
  else if (duration_horizontal < HORIZONTAL_LOW_THRESHOLD) { play_song = SongList::DanceMusic;} // left
  else play_song = SongList::NoSong;

// playthesong(StartupEngine,7000,50);
  switch (play_song) {
    case SongList::NoSong:
      mp3.stop(); 
      //playthesong(StartupEngine, 7000, 50); 
    break;
    case SongList::GetOutOfTheWay:
      playthesong(GetOutOfTheWay, 2000, 50); 
    break;
    case SongList::HappyDance:
      playthesong(HappyDance, 7000, 50); 
    break;
    case SongList::StartupEngine:
      playthesong(StartupEngine, 7000, 50); 
    break;
    case SongList::BreakingDown:
      playthesong(BreakingDown, 7000, 50); 
    break;
    case SongList::Horn:
      playthesong(Horn, 7000, 50); 
    break;
    case SongList::Alarm:
      playthesong(Alarm, 7000, 50); 
    break;
    case SongList::DanceMusic:
      playthesong(DanceMusic, 7000, 50); 
    break;
    default:
      playthesong(MotorSound,10900,50);

    break;
  }


//  rainbow (1);

}
void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
  strip.rainbow(firstPixelHue);
  strip.show();
  delay(wait);
  }
}

void playthesong(int this_song, int this_duration, int this_volume){
  //void playthesong(int this_song, int duration=2000, int volume=50){

  mp3.play(this_song, this_volume); 
  delay(this_duration);
 // mp3.stop();  
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

