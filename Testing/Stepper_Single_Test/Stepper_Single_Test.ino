void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
}

void loop() {
  moveStepper('X', 1, 4000, 150, 1000);
  moveStepper('X', 0, 4000, 150, 1000);
  moveStepper('Z', 1, 20000, 100, 1000);
  moveStepper('Z', 0, 20000, 100, 1000);
}
