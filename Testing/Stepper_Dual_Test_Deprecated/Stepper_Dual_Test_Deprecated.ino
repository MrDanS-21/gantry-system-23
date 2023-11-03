void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
}

void loop() {
  moveSteppers(1, 1, 4000, 20000, 300, 200);
  delay(1000);
  moveSteppers(0, 0, 4000, 20000, 300, 200);
  delay(1000);
}
