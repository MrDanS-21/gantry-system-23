long currPosX = 0;
long currPosZ = 0;

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  currPosX = 0;
  currPosZ = 0;
  Serial.begin(9600);
}

void loop() {
  moveToPosition(8000, 8000);
  delayCS(1000);
  moveToPosition(2000, 10000);
  delayCS(1000);
  moveToPosition(5000, 20000);
  delayCS(1000);
  moveToPosition(2000, 2000);
  delayCS(1000);
  moveToPosition(0, 0);
  delayCS(2500);
  moveToPosition(16000, 44000);
  delayCS(1000);
}
