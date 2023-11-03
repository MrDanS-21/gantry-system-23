void stepStepper(char motor, bool dir) {
  int dirPin;
  int stepPin;
  
  if (motor == 'Z') {
    dirPin = 15;
    stepPin = 16;
    dir ? currPosZ += 1 : currPosZ -= 1;
  } else if (motor == 'X') {
    dirPin = 2;
    stepPin = 3;
    dir ? currPosX += 1 : currPosX -= 1;
  } else {
    return;  //Invalid stepper motor
  }
  
  digitalWrite(dirPin, dir);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(150);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(150);
}

void moveStepper(long targetPosX, long targetPosZ) {
  bool dirX;
  bool dirZ;

  if((targetPosX - currPosX)<0) {
    dirX=0;
    while (currPosX > targetPosX) {
      stepStepper('X', dirX);
    }
  } else {
    dirX=1;
    while (currPosX < targetPosX) {
      stepStepper('X', dirX);
    }
  }
  if ((targetPosZ - currPosZ)<0) {
    dirZ=0;
    while (currPosZ > targetPosZ) {
      stepStepper('Z', dirZ);
    } 
  } else {
    dirZ=1;
    while (currPosZ < targetPosZ) {
      stepStepper('Z', dirZ);
    } 
  }
}

void requestMoveStepper(long x, long z) {  
  if (x > 16000 || z > 44000 || x < 0 || z < 0) {
    data += "ERROR(400 Bad Request: move X,Z => Limit Exceeded! X must be positive and less than 16000 and Z must be positive and less than 44000);";
    return;
  }

  // If everything is okay, move to the position
  Serial.println("INFO(Moving to [" + String(x) + "," + String(z) + "]);");
  moveStepper(x, z);
}
 
void zeroStepper(char motor) {
  if (motor == 'Z') {
    while (digitalRead(14) == HIGH) {
      moveStepper('Z', 0);
    }
  } else if (motor == 'X') {
    while (digitalRead(17) == HIGH) {
      moveStepper('X', 0);
    }
  }
}

void zeroStepper() {
  zeroStepper('Z');
  zeroStepper('X');
}

bool separateCommand(String string, char character, String& a, String& b) {
  int index = string.indexOf(character);

  // Check if character exists
  if (index == -1) {
    data += "ERROR(400 Bad Request: Expected A" + String(character) + "B);";
    return false;
  }

  // Split the string into x and z
  a = string.substring(0, index);
  b = string.substring(index + 1);
  return true;
}

bool toNumber(String string, long& number) {
  number = string.toInt();
  if (string != String(number)) {
    data += "ERROR(400 Bad Request: toNumber failed => expected number);";
    return false;
  }
  return true;
}

bool toBool(String str, bool& tf) {
  if (str == "true" || str == "1") {
    tf = true;
    return true;
  } else if (str == "false" || str == "0") {
    tf = false;
    return true;
  } else {
    data += "ERROR(400 Bad Request: toBool failed => expected boolean);";
    return false;
  }
}

void moveServo(Servo &servo, int position, int duration) {
  servo.writeMicroseconds(position);
  delay(duration);
}

void clamp(bool clamp) {
  int pos;
  if (clamp) {
    pos = 1000;
    isClamped = true;
  } else {
    pos = 1450;
    isClamped = false;
  }
  moveServo(rot, pos, 2000);
}

void pp(int height, bool clmp) {
  moveServo(lin, height, 5000);
  clamp(clmp);
  moveServo(lin, 1000, 5000);
}

void requestPP(int height, bool clmp) {
  if (height < 1000 || height > 2000) {
    data += "ERROR(400 Bad Request: pick/place => height must be interger between 1000 and 2000);";
    return;
  }
  if (clmp) {
    Serial.println("INFO(Picking);");
  } else {
    Serial.println("INFO(Placing);");
  }
  pp(height, clmp);
}

void getColors(float &r, float &g, float &b) {
  tcs.getRGBC(&red, &green, &blue, &Clear);                                                             //this obtains raw sensor values
  uint32_t sum = Clear;
  r = red; r /= sum;                                                                                    // values out of 255
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;                                                                         // calculates r.g.b values
}

bool presence(float r, float g, float b) {
  //if any of the color levels are within ambient level's margin of error
  if (((ambientR - 10)<r and r<(ambientR + 10)) and ((ambientG - 10)<g and g<(ambientG + 10)) and ((ambientB - 10)<b and b<(ambientB + 10))) {
    return false;         //No package is present
  } else {
    return true;          //Otherwise package is present
  }
}

bool outOfBoundsError() {
  if ((currPosX < -1) || (currPosX > 16001) || (currPosZ < -1) || (currPosZ > 44001)) {
    return true;
  } else {
    return false;
  }
}
