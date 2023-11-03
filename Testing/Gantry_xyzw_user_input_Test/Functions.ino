void moveStepper(char motor, bool dir, long steps, int speed) {
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
  delayMicroseconds(100);
}

void moveServo(Servo &servo, int position, int duration) {
  servo.writeMicroseconds(position);
  delay(duration);
}

void clamp(bool x) {
  int position;
  if (x == true) {
    position = 1000;
  } else {
    position = 1450;
  }
  moveServo(rot, position, 2000);
  currPosW = x;
}

void pp(bool x) {
  moveServo(lin, 2000, 5000);
  currPosY = 2000;
  clamp(x);
  moveServo(lin, 1000, 5000);
  currPosY = 1000;
}

void move(long targetPosX, long targetPosZ) {
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
  moveStepper('X', dirX, abs(distX), 150);
  delayMicroseconds(100);
  moveStepper('Z', dirZ, abs(distZ), 100);

  // Update the current position variables
  currPosX = targetPosX;
  currPosZ = targetPosZ;
}

void moveAbsolute(long targetPosX, int targetPosY, long targetPosZ, bool targetPosW) {
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
  Serial.print(",");
  Serial.print(targetPosY);
  Serial.print(",");
  Serial.print(targetPosZ);
  Serial.print(",");
  Serial.print(targetPosW);
  Serial.println(")");

  //if (
  
  // Use the existing function to move each axis
  moveStepper('X', dirX, abs(distX), 150);
  delayMicroseconds(100);
  moveStepper('Z', dirZ, abs(distZ), 100);

  // Update the current position variables
  currPosX = targetPosX;
  currPosY = targetPosY;
  currPosZ = targetPosZ;
  currPosW = targetPosW;
}

// Use serial input to handle commands
void checkSerial() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "Shutdown") {
      if (currPosW == 1) {
        Serial.println("Grippers Engaged, Cannot Shutdown");
      } else {
        // Return to origin
        moveAbsolute(0, 1000, 0, 0);
        Serial.println("Shutdown");
        while (true) {
          //empty
        }
      }
    } else if (command == "Zero") {
      if (currPosW == 1) {
        Serial.println("Zeroing with Grippers Engaged | Are you sure? (5s) | Y/N");
        for (int x = 0; x<50; x++) {
          String reply = Serial.readStringUntil('\n');
          reply.trim();
          if (reply == "Y") {
            Serial.println("Zeroing");
            moveAbsolute(0, 1000, 0, 1);
            delay(2500);
            break;
          } else if (reply == "N") {
            break;
          } else {
            delay(100);
          }
        }
      } else {
        Serial.println("Zeroing");
        moveAbsolute(0, 1000, 0, 0);
        delay(2500);
      }
    } else if (command == "sudo Shutdown") {
      Serial.println("Initiating Force Shutdown");
      moveAbsolute(0, 1000, 0, currPosW);
      Serial.println("Shutdown");
      while (true) {
        //empty
      }
    }
  }
}

void delayCS(int duration) {
  delay(duration);
  checkSerial();
}
