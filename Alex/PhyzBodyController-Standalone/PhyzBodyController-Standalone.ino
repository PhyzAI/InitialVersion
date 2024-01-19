#include <Adafruit_PWMServoDriver.h>
// #include <Sabertooth.h>

// Pin Settings
#define CH_3_PIN A0
#define CH_4_PIN A1
#define CH_5_PIN 5
#define CH_6_PIN 6

// Config Settings
#define MIN_STICK_CH_VAL -1000 // Minimum value for stick channels
#define MAX_STICK_CH_VAL 1000 // Maximum value for stick channels

int ch3Value, ch4Value;
bool ch5Value, ch6Value;

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  noInterrupts();
  long ch = pulseIn(channelInput, HIGH, 300000);
  interrupts();
  if (ch < 10) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Read the channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void setup() {
  Serial.begin(9600);

  // Set pinmodes
  pinMode(CH_3_PIN, INPUT);
  pinMode(CH_4_PIN, INPUT);
  pinMode(CH_5_PIN, INPUT);
  pinMode(CH_6_PIN, INPUT);

}

void loop() {
  ch3Value = readChannel(CH_3_PIN, MIN_STICK_CH_VAL, MAX_STICK_CH_VAL, 0);
  ch4Value = readChannel(CH_4_PIN, MIN_STICK_CH_VAL, MAX_STICK_CH_VAL, 0);
  ch5Value = readChannel(CH_5_PIN, MIN_STICK_CH_VAL, MAX_STICK_CH_VAL, 0);
  ch6Value = readChannel(CH_6_PIN, MIN_STICK_CH_VAL, MAX_STICK_CH_VAL, 0);

  Serial.print(" CH3:"); Serial.print(ch3Value);
  Serial.print(" CH4:"); Serial.print(ch4Value);
  Serial.print(" CH5:"); Serial.print(ch5Value);
  Serial.print(" CH6:"); Serial.print(ch6Value);
  Serial.println();
  delay(0);
}



