void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  
  pinMode(14, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  
  Serial.begin(9600);

  while (digitalRead(14) == HIGH) {
    moveStepper('Z', 0);
  }
  delayMicroseconds(150);
  while (digitalRead(17) == HIGH) {
    moveStepper('X', 0);
  }
  delayMicroseconds(150);
  Serial.println("Zero'd");
}

void loop() {
  Serial.println("Loop Function");
  delay(1000);
}
