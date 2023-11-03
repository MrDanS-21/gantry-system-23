String data;

void setup() {
  Serial.begin(9600);
  Serial.println("INITIALIZED");
}

void loop() {
  Serial.println("INFO(Hello World);");
  String command = Serial.readStringUntil('\n');
  command.trim();
  if (command != "") {
    Serial.println("INFO(Recieved Command:" + command + ");");
  }
  Serial.print(data);
}
