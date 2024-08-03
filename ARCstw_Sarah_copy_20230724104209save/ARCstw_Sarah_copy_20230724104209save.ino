#include <Sabertooth.h>
Sabertooth ST1[2] = { Sabertooth(128), Sabertooth(129) };

#define CH1 3
#define CH2 5
#define CH3 6
#define CH4 9
int speed =0;
int turn =0;

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Red the channel and return a boolean value
bool redSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void setup(){
  Serial.begin(9600);
  SabertoothTXPinSerial.begin(9600);
  Sabertooth::autobaud(SabertoothTXPinSerial); 
  ST1[1].motor(1, 0);
  ST1[1].motor(2, 0);
  ST1[0].motor(1, 0);
  ST1[0].motor(2, 0);

  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
}

int ch1Value, ch2Value, ch3Value, ch4Value;

void loop() {
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch3Value = readChannel(CH3, -100, 100, 0);
  ch4Value = readChannel(CH4, -100, 100, 0);


  // Tank Mode  
  if(ch3Value < 50 && ch3Value > -10) {
    int speed = map(ch1Value, -100, 100, -127, 127);
    int speedOffset = map(ch4Value, -100, 100, 0, 127);
   
    if(speed < 15 && speed > -15) {speed = 0;}
    else {  
    if(speed > 10) {speed = map(speed + speedOffset, 10, 254, 0, 127);}
    if(speed < 10) {speed = map(speed - speedOffset, -254, -10, -127, 0);}
    }
   
    int turn = map(ch2Value, 100, -100, -127, 127);
    if(turn < 10 && turn > -10) {turn = 0;}
       
    int nm11 = speed - turn;
    int nm12 = speed - turn;
    int nm01 = speed + turn;
    int nm02 = speed + turn;
  
  Serial.print(nm11);
  Serial.print(":");
  Serial.print(nm12);
  Serial.print(":");
  Serial.print(nm01);
  Serial.print(":");
  Serial.print(nm02);
  Serial.println(":");

   3

    ST1[1].motor(1, nm11);
    ST1[1].motor(2, nm12);
    ST1[0].motor(1, nm01);
    ST1[0].motor(2, nm02);
  }
  
  // Do Not Move
  if(ch3Value >50) {
    Serial.print("Mode 3: ");
    ST1[1].motor(1, 00);
    ST1[1].motor(2, 00);
    ST1[0].motor(1, 00);
    ST1[0].motor(2, 00);
  }
}