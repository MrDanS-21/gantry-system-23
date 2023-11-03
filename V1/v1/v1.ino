#include <Wire.h>                                                                                           //Library to use the I2C communication protocol used by the colour sensor
#include <Servo.h>
#include "DFRobot_TCS34725.h"                                                                               //library to use the colour sensor
#include <avr/sleep.h>

uint16_t Clear, red, green, blue;                                                                           // creates unsigned integer variables to store raw sensor values
                                                
DFRobot_TCS34725 tcs = DFRobot_TCS34725(&Wire, TCS34725_ADDRESS,TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

long currPosX;
long currPosZ;
bool isClamped;

float ambientR;
float ambientG;
float ambientB;

Servo lin;
Servo rot;

String data = "";

void setup() {
  pinMode(2, OUTPUT);                         //Initializing Pins
  pinMode(3, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(14, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);

  lin.attach(10);                             //Initializing Servos
  rot.attach(9);

  tcs.begin();                                //Initializing Color Sensor
  Serial.begin(9600);                         //Initializing Serial Port

  moveServo(lin, 1000, 10);                   //Retract linear servo
  clamp(0);                                   //And open claw

  delayMicroseconds(300);
  while (digitalRead(14) == HIGH) {           //While Z limit switch is not triggered, move stepper towards Z=0
    stepStepper('Z', 0);
  }
  delayMicroseconds(300);
  while (digitalRead(17) == HIGH) {           //While X limit switch is not triggered, move stepper towards X=0
    stepStepper('X', 0);
  }
  delayMicroseconds(300);
  currPosX = 0;                               //When X and Z steppers have moved to origin (0, 0), set position varibles to 0
  currPosZ = 0;

  getColors(ambientR, ambientG, ambientB);    //Get ambient colors measured by color sensor
  Serial.println("INITIALIZED");
}

void loop() {
  data = "";
  
  float r, g, b;
  getColors(r, g, b);
  
  String command = Serial.readStringUntil('\n');
  command.trim();
  int firstSpaceIndex = command.indexOf(' ');
  String firstPart;
  String secondPart;
  if (firstSpaceIndex != -1) {
    firstPart = command.substring(0, firstSpaceIndex);
    secondPart = command.substring(firstSpaceIndex+1);
    secondPart.trim(); // remove leading and trailing white spaces
  } else {
    firstPart = command;
  }

  if (firstPart == "sudo") {
    if (secondPart == "shutdown") {
      data += "INFO(Emergency Shutdown Sequence);";
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_enable();
      sleep_mode(); // The program will stop here
    } else {
      data += "ERROR(400 Bad Request: sudo __" + secondPart + "__ is invalid);";
    }
  } else if (firstPart == "moveGantry") {
    String xStr, zStr;
    long x, z;
    if (separateCommand(secondPart, ',', xStr, zStr)) {
      if (toNumber(xStr, x) && toNumber(zStr, z)) {
        requestMoveStepper(x, z); 
      }
    }
  } else if (firstPart == "pick") {
    int height = secondPart.toInt();
    requestPP(height, 1);
  } else if (firstPart == "place") {
    int height = secondPart.toInt();
    requestPP(height, 0);
  } else if (firstPart == "move") {
    String iXstr, iZstr, iYstr, fXstr, fZstr, fYstr;
    long iX, iZ, iY, fX, fZ, fY;
    if (separateCommand(secondPart, ' ', iXstr, fXstr)) {
      if (separateCommand(iXstr, ',', iXstr, iZstr) && separateCommand(fXstr, ',', fXstr, fZstr)) {
        if (separateCommand(iZstr, ',', iZstr, iYstr) && separateCommand(fZstr, ',', fZstr, fYstr)) {
          if (toNumber(iXstr, iX) && toNumber(iZstr, iZ) && toNumber(iYstr, iY) && toNumber(fXstr, fX) && toNumber(fZstr, fZ) && toNumber(fYstr, fY)) {
            if (isClamped) {
              data += "ERROR(403 Forbidden: move failed => claw is loaded);";
              return;
            }
            requestMoveStepper(iX, iZ);
            requestPP(iY, 1);
            requestMoveStepper(fX, fZ);
            requestPP(fY, 0);
          }
        }
      }
    }
  } else if (firstPart == "stepStepper") {
    String xz, dirStr;
    bool dir;
    if (separateCommand(secondPart, ' ', xz, dirStr)) {
      if (toBool(dirStr, dir)) {
        for (int i = 0; i < 100; i++) {
          if (xz == "X") {
            stepStepper('X', dir);
          } else if (xz == "Z") {
            stepStepper('Z', dir);
          } else {
            data += "ERROR(400 Bad Request: stepStepper => xz needs to be \"X\" or \"Z\");";
            i = 100;
          }
        }
      }
    }
  } else if (firstPart == "height") {
    int height = secondPart.toInt();
    if (height < 1000 || height > 2000) {
      data += "ERROR(400 Bad Request: height needs to be between 1000 and 2000);";
      return;
    }
    moveServo(lin, height, 1000);
  } else if (firstPart == "") {
    //
  } else {
    data += "ERROR(400 Bad Request: __" + firstPart + "__ is invalid);";
  }

  if (presence(r, g, b)) {
    data += "IMPORT(" + String(r) + "," + String(g) + "," + String(b) + ");";
  }

  if (outOfBoundsError()) {
    data += "ERROR(500 Internal Server Error: outOfBoundsError " + String(currPosX) + "," + String(currPosZ) + ");";
  }
  
  if (data == "") {
    data = "INFO(Idle);";
  }
  Serial.println(data);
}
