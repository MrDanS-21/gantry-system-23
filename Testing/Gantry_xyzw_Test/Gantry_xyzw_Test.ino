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
  lin.attach(9);
  rot.attach(10);
  moveServo(lin, 1000, 1000);
  currPosX = 0;
  currPosY = 1000;
  currPosZ = 0;
  currPosW = 0;
  Serial.begin(9600);
}

void loop() {
  move(8000, 8000);
  delayCS(250);
  pp(1);
  delayCS(250);
  move(2000, 10000);
  delayCS(250);
  pp(0);
  delayCS(250);
  move(5000, 20000);
  delayCS(250);
  pp(1);
  delayCS(250);
  move(2000, 2000);
  delayCS(250);
  pp(0);
  delayCS(250);
  move(0, 0);
  delayCS(5000);
  pp(1);
  delayCS(250);
  move(16000, 44000);
  delayCS(250);
  pp(0);
  delayCS(250);
}
