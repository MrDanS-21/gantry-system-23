// Function to move a stepper motor
void moveStepper(char motor, bool dir, int steps, int delayMicros) {
  int dirPin;
  int stepPin;
  static unsigned long lastStepTimeX = 0;
  static unsigned long lastStepTimeZ = 0;
  static int stepCountX = 0;
  static int stepCountZ = 0;
  unsigned long* lastStepTime;
  int* stepCount;

  if (motor == 'X') {
    dirPin = 2;
    stepPin = 3;
    lastStepTime = &lastStepTimeX;
    stepCount = &stepCountX;
  } else if (motor == 'Z') {
    dirPin = 15;
    stepPin = 16;
    lastStepTime = &lastStepTimeZ;
    stepCount = &stepCountZ;
  } else {
    return; // Invalid motor
  }

  if (micros() - *lastStepTime >= delayMicros && *stepCount < steps) {
    digitalWrite(dirPin, dir); 
    digitalWrite(stepPin, HIGH); 
    delayMicroseconds(5);
    digitalWrite(stepPin, LOW);
    *lastStepTime = micros();
    (*stepCount)++;
  }

  if (*stepCount >= steps) {
    *stepCount = 0;  // Reset step count for next movement
  }
}

// Function to move both stepper motors
void moveSteppers(bool dirX, bool dirZ, int stepsX, int stepsZ, int delayMicrosX, int delayMicrosZ) {
  while (stepsX > 0 || stepsZ > 0) {
    if (stepsX > 0) {
      moveStepper('X', dirX, stepsX, delayMicrosX);
      stepsX--;
    }
    if (stepsZ > 0) {
      moveStepper('Z', dirZ, stepsZ, delayMicrosZ);
      stepsZ--;
    }
  }
}
