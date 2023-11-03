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

void moveToPosition(long targetPosX, long targetPosZ) {
  // Calculate the distance to move for each axis
  long distX = targetPosX - currPosX;
  long distZ = targetPosZ - currPosZ;
  bool dirX = distX > 0;
  bool dirZ = distZ > 0;
  
  distX = abs(distX);
  distZ = abs(distZ);

  Serial.print("Moving to (");
  Serial.print(targetPosX);
  Serial.print(",~,");
  Serial.print(targetPosZ);
  Serial.println(")");

  while(distX > 0 || distZ > 0) {
    if(distX > 0) {
      moveStepper('X', dirX);
      distX--;
    }
    if(distZ > 0) {
      moveStepper('Z', dirZ);
      distZ--;
    }
    delayMicroseconds(150); // A delay between steps to ensure motors can keep up
  }

  // Update the current position variables
  currPosX = targetPosX;
  currPosZ = targetPosZ;
}


// Use serial input to handle commands
void checkSerial() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "Shutdown") {
      // Return to origin
      moveToPosition(0, 0);
      Serial.println("Shutdown");
      while (true) {
        //empty
      }
    } else if (command == "Zero") {
      moveToPosition(0, 0);
      delay(2500);
    }
  }
}

void delayCS(int duration) {
  delay(duration);
  checkSerial();
}
