#include <Sabertooth.h>
Sabertooth ST1[2] = { Sabertooth(128), Sabertooth(129) };

#define CH1 3
#define CH2 5
#define CH3 6
#define CH4 9

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
  
  
  Serial.print("Ch1: ");
  Serial.print(ch1Value);
  Serial.print(" Ch2: ");
  Serial.print(ch2Value);
  Serial.print(" Ch3: ");
  Serial.print(ch3Value);
  Serial.print(" Ch4: ");
  Serial.println(ch4Value);

  int speedFB = 127/100 * ch2Value;
  int speedLR = 127/100 * ch1Value;
/*
  ST1[1].motor(1, 100);
  ST1[1].motor(2, 100);
  ST1[0].motor(1, 100);
  ST1[0].motor(2, 100);
  delay(1000);

  ST1[1].motor(1, 00);
  ST1[1].motor(2, 00);
  ST1[0].motor(1, 00);
  ST1[0].motor(2, 00);
  delay(1000);
*/
  if(ch3Value < 10 && ch3Value > -10){
    if(ch2Value > 20){
      ST1[1].motor(1, 100);
      ST1[1].motor(2, 100);
      ST1[0].motor(1, 100);
      ST1[0].motor(2, 100);
    }else if(ch2Value < -20){
      ST1[1].motor(1, -100);
      ST1[1].motor(2, -100);
      ST1[0].motor(1, -100);
      ST1[0].motor(2, -100);
    }else if(ch1Value > 20){
      ST1[1].motor(1, -100);
      ST1[1].motor(2, -100);
      ST1[0].motor(1, 100);
      ST1[0].motor(2, 100);
    }else if(ch1Value < -20){
      ST1[1].motor(1, 100);
      ST1[1].motor(2, 100);
      ST1[0].motor(1, -100);
      ST1[0].motor(2, -100);
    }else{
      ST1[1].motor(1, 00);
      ST1[1].motor(2, 00);
      ST1[0].motor(1, 00);
      ST1[0].motor(2, 00);
    }  
  }else if(ch3Value < -10){

  }else{
    ST1[1].motor(1, 00);
    ST1[1].motor(2, 00);
    ST1[0].motor(1, 00);
    ST1[0].motor(2, 00);
  } 

}