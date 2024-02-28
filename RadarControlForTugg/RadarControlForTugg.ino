#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  delay(random(15000, 90000));
  myservo.write(random(10,170));              
  delay(random(500, 3000));        
  myservo.write(90); 
}
