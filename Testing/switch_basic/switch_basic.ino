void setup() {
  pinMode(14, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  Serial.print("D14: ");
  if (digitalRead(14) == LOW) {
    Serial.print("ON\t");
  } else {
    Serial.print("OFF");
  }
  Serial.print("\t");
  Serial.print("D17: ");
  if (digitalRead(17) == LOW) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
  delay(100);
}
