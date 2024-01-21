
// Writes to sabertooth motor controllers as a servo
// Speed accepted between -100 to 100 where 0 is stopped. Sign designates direction
void setMotorSpeed(Servo mServo, int speed) {
  speed = (abs(speed)<DEAD_ZONE)?0:speed;
  speed = map(speed, -100, 100, 0, 180);
  mServo.write(speed);
}

// gets the direction from forward / back and left / right values
Direction getDirection(int FB, int LR) {
  if (FB < -DEAD_ZONE) {
    if (LR < -DEAD_ZONE) return BL;
    else if (LR > DEAD_ZONE) return BR;
    else return BB;
  }
  else if (FB > DEAD_ZONE) {
    if (LR < -DEAD_ZONE) return FL;
    else if (LR > DEAD_ZONE) return FR;
    else return FF;
  }
  else {
    if (LR < -DEAD_ZONE) return LL;
    else if (LR > DEAD_ZONE) return RR;
    else return STOP;
  }
}
