void moveStepper(char motor, bool dir) {
  int dirPin;
  int stepPin;
  
  if (motor == 'Z') {
    dirPin = 15;
    stepPin = 16;
  } else if (motor == 'X') {
    dirPin = 2;
    stepPin = 3;
  } else {
    return;  //Invalid stepper motor
  }
  
  digitalWrite(dirPin, dir);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(150);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(150);
}
