void moveStepper(char motor, bool dir, long steps, int speed, int duration) {
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

  Serial.print("Moving Stepper ");
  Serial.print(motor);
  Serial.print(" ");
  Serial.print(steps);
  Serial.print(" Microsteps in the ");
  Serial.print(dir);
  Serial.println(" direction");
  
  digitalWrite(dirPin, dir);
  for(long x = 0; x < steps; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
  delay(duration);
}

void moveToPosition(long targetPosX, long targetPosZ) {
  // Calculate the distance to move for each axis
  long distX = targetPosX - currPosX;
  long distZ = targetPosZ - currPosZ;
  bool dirX;
  bool dirZ;

  if(distX<0) {
    dirX=0;
  } else {
    dirX=1;
  }
  if (distZ<0) {
    dirZ=0;
  } else {
    dirZ=1;
  }

  Serial.print("Moving to (");
  Serial.print(targetPosX);
  Serial.print(",~,");
  Serial.print(targetPosZ);
  Serial.println(")");
  
  // Use the existing function to move each axis
  moveStepper('X', dirX, abs(distX), 150, 100);
  delayMicroseconds(100);
  moveStepper('Z', dirZ, abs(distZ), 100, 100);

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
