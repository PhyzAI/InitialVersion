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
    
 
  // Mode 1 Omni Mode
  if(ch3Value < -50) {  
    int speed = map(ch4Value, -100, 91, 0, 127); 
    int DV = ch2Value; if(DV < 50 && DV > -50){DV=0;}
    int DH = ch1Value; if(DH < 50 && DH > -50){DH=0;}
    
    if(speed < 10) {
       speed=0;
        ST1[1].motor(1, 0);
        ST1[1].motor(2, 0);
        ST1[0].motor(1, 0);
        ST1[0].motor(2, 0);
        }
    
    else {

    int nm11 = 0;
    int nm12 = 0;
    int nm01 = 0;
    int nm02 = 0;

    //Fwd left
    if(DV>10 && DH > 10){ 
    nm11 = 0;
    nm12 = speed;
    nm01 = speed;
    nm02 = 0;
    }
    
    //Fwd 
    if(DV>10 && DH == 0){ 
    nm11 = speed;
    nm12 = speed;
    nm01 = speed;
    nm02 = speed;
    }
    
    //Fwd Right
    if(DV>10 && DH < -10){ 
    nm11 = speed;
    nm12 = 0;
    nm01 = 0;
    nm02 = speed;
    }

    //left slide
    if(DV == 0 && DH > 10){
    nm11 = -speed;
    nm12 = speed;
    nm01 = speed;
    nm02 = -speed;
    }

    // Right left slide
    if(DV == 0 && DH < -10){
    nm11 = speed;
    nm12 = -speed;
    nm01 = -speed;
    nm02 = speed;
    }   
    //back Right 
    if(DV < -10 && DH < -10){ 
    nm11 = 0;
    nm12 = -speed;
    nm01 = -speed;
    nm02 = -0;
    }
    
    //Back 
    if(DV < -10 && DH == 0){
    nm11 = -speed;
    nm12 = -speed;
    nm01 = -speed;
    nm02 = -speed;
    }

    //Back Left 
    if(DV < -10 && DH > 10){
    nm11 = -speed;
    nm12 = 0;
    nm01 = 0;
    nm02 = -speed;
    }
    
   ST1[1].motor(1, nm11);
   ST1[1].motor(2, nm12);
   ST1[0].motor(1, nm01);
   ST1[0].motor(2, nm02);

    }    
    
  }

  // Tank Mode  
  if(ch3Value < 50 && ch3Value > -10) {
    int speed = map(ch2Value, -100, 100, -127, 127);
    int speedOffset = map(ch4Value, -100, 100, 0, 127);
   
    if(speed < 15 && speed > -15) {speed = 0;}
    else {  
    if(speed > 10) {speed = map(speed + speedOffset, 10, 254, 0, 127);}
    if(speed < 10) {speed = map(speed - speedOffset, -254, -10, -127, 0);}
    }

    int turn = map(ch1Value, -100, 100, -127, 127);
    if(turn < 10 && turn > -10) {turn = 0;}
       
    int nm11 = speed - turn;
    int nm12 = speed - turn;
    int nm01 = speed + turn;
    int nm02 = speed + turn;
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