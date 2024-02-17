#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>

/*
+++++ Sabertooth Motor Controller Setup +++++
Sabertooths will be configured in RC control mode. To setup sabertooth motor controllers, please set switch 1 to the DOWN
position and switch 2 to the UP position. 

SW1 - 6 Config
[DOWN, UP, UP, DOWN, UP, UP]
* SW4 - Mixed Mode off
* SW5 - Linear Response
* SW6 - MCU Mode

*/

/*
+++++ Motor Controller Settings
CH1 - L/R on the right stick
CH2 - U/D on the right stick
CH5 - Right 3 pos switch
CH6 - Right dial

*/

// Controller Settings
#define MAX_SPEED 100 // In range [0,100]
#define DEAD_ZONE 10  // Dead zone for controller noise

// RC input Pins
#define CH_1_PIN A0
#define CH_2_PIN A1
#define CH_5_PIN A2
#define CH_6_PIN A3 // Speed Dial

// Motor Controller Pins
#define BACK_LEFT_PIN   2 // Back Left Wheel
#define FRONT_LEFT_PIN  3 // Front Left Wheel
#define BACK_RIGHT_PIN  4 // Back Right Wheel
#define FRONT_RIGHT_PIN 5 // Front Right Wheel

// Config Settings
#define MIN_STICK_CH_VAL -100 // Minimum value for stick channels
#define MAX_STICK_CH_VAL 100 // Maximum value for stick channels

// Mode Settings
// Modes: 0=motor lock, 1=tank mode, 2=omni wheel mode
uint8_t motorMode = 0;
#define MOTOR_LOCK_MODE 0
#define TANK_MODE 1
#define OMNI_MODE 2

enum Direction {
  FF, // Forward
  FR, // Forward Right
  RR, // Right
  BR, // Back Right
  BB, // Back
  BL, // Back Left
  LL, // Left
  FL,  // Forward Left
  STOP // In dead zone, no direction
};
Direction phyzDirection;

int ch1Value, ch2Value;
int ch5Value, ch6Value;

int motorSpeed = 0;

Servo BLServo, FLServo;
Servo BRServo, FRServo;
#define BLSERVO_INV -1
#define FLSERVO_INV -1
#define BRSERVO_INV 1
#define FRSERVO_INV -1

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  //noInterrupts();
  long ch = pulseIn(channelInput, HIGH, 30000);
  //interrupts();
  if (ch < 10) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Read the channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

// Serial logging for debug
void logToSerial() {
  Serial.print(" CH1:"); Serial.print(ch1Value);
  Serial.print(" CH2:"); Serial.print(ch2Value);
  // Serial.print(" CH5:"); Serial.print(ch5Value);
  // Serial.print(" CH6:"); Serial.print(ch6Value);

  Serial.print(" Motor Speed:"); Serial.print(motorSpeed);
  Serial.print(" Motor Mode:"); Serial.print(motorMode);
  Serial.print(" Direction:"); Serial.print(phyzDirection);
  Serial.println();
}

void setup() {
  Serial.begin(9600);

  // Set pinmodes
  pinMode(CH_1_PIN, INPUT);
  pinMode(CH_2_PIN, INPUT);
  pinMode(CH_5_PIN, INPUT);
  pinMode(CH_6_PIN, INPUT);

  // Setup Motor Control Channels
  BLServo.attach(BACK_LEFT_PIN);
  FLServo.attach(FRONT_LEFT_PIN);
  BRServo.attach(BACK_RIGHT_PIN);
  FRServo.attach(FRONT_RIGHT_PIN);
}

void loop() {

  // Get motor Speed
  motorSpeed = readChannel(CH_6_PIN, 0, 100, 0);
  motorSpeed = (motorSpeed<0)?0:motorSpeed; // Set speed to 0 if less that 0

  // Read motor mode
  ch5Value = readChannel(CH_5_PIN, MIN_STICK_CH_VAL, MAX_STICK_CH_VAL, 0);
  if (ch5Value < -20)     motorMode = TANK_MODE;
  else if (ch5Value > 20) motorMode = OMNI_MODE;
  else                    motorMode = MOTOR_LOCK_MODE;


  // Read Stick Values
  ch1Value = readChannel(CH_1_PIN, MIN_STICK_CH_VAL, MAX_STICK_CH_VAL, 0);
  ch2Value = -readChannel(CH_2_PIN, MIN_STICK_CH_VAL, MAX_STICK_CH_VAL, 0);

  // Get Direction of travel
  phyzDirection = getDirection(ch2Value, ch1Value);


  setMotorSpeed(BLServo, (ch2Value*motorSpeed)/100);
  setMotorSpeed(FLServo, (ch2Value*motorSpeed)/100);
  setMotorSpeed(BRServo, (ch2Value*motorSpeed)/100);
  setMotorSpeed(FRServo, (ch2Value*motorSpeed)/100);


  // Switch Motor Mode
  switch(motorMode) {
    case MOTOR_LOCK_MODE:
      // do nada
    break;
    case TANK_MODE:
    break;
    case OMNI_MODE:

    break;
  }

  logToSerial();
}



