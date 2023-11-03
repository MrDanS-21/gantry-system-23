#include <Servo.h>

long currPosX = 0;
int currPosY = 1000;
long currPosZ = 0;
bool currPosW = 0;

Servo lin;
Servo rot;

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  lin.attach(10);
  rot.attach(9);
  moveServo(lin, 1000, 1000);
  currPosX = 0;
  currPosY = 1000;
  currPosZ = 0;
  currPosW = 0;
  Serial.begin(9600);
}

void loop() {
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
  } else if (command == "pick") {
    Serial.println("Picking");
    pp(1);
    delay(250);
  } else if (command == "place") {
    Serial.println("Placing");
    pp(0);
    delay(250);
  } else if (command == "import") {
    Serial.println("Importing");
    move(0, 44000);
    delay(250);
    pp(1);
  } else if (command == "export") {

  } else {
    // Split command into parts
    int commaIndex = command.indexOf(',');
    if (commaIndex != -1) {
      String firstPart = command.substring(0, commaIndex);
      String secondPart = command.substring(commaIndex+1);

      // Convert to integers
      long x = firstPart.toInt();
      long z = secondPart.toInt();

      // Check if conversion was successful
      if (x != 0 || firstPart == "0" && z != 0 || secondPart == "0") {
        move(x, z);
        delay(250);
      } else {
        Serial.println("Invalid command. Expected format: x, z where x and z are numbers.");
      }
    } else if (command != "") {
      Serial.println("Unknown command.");
    }
  }
  delay(250);
}
