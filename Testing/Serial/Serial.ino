void setup() {
  Serial.begin(9600);
}

void loop() {
  String command = Serial.readStringUntil('\n');
  command.trim();

  int firstSpaceIndex = command.indexOf(' ');

  String firstPart;
  String secondPart;

  if (firstSpaceIndex != -1) {
    firstPart = command.substring(0, firstSpaceIndex);
    secondPart = command.substring(firstSpaceIndex+1);
  } else {
    firstPart = command;
  }

  Serial.println("First Part: " + firstPart);
  if(firstSpaceIndex != -1) {
    Serial.println("Second Part: " + secondPart);
  }
  delay(100);
}
