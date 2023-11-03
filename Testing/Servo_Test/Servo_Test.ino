#include <Servo.h>

Servo lin;
Servo rot;

void setup() {
  lin.attach(10);
  rot.attach(9);
  Serial.begin(9600);
}

void loop() {
  moveServo(lin, 2000, 5000);
  moveServo(rot, 1000, 2000);
  moveServo(lin, 1000, 5000);
  moveServo(lin, 2000, 5000);
  moveServo(rot, 1450, 2000);
  moveServo(lin, 1000, 5000);
}
