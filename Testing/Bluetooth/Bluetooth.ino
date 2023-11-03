#include <SoftwareSerial.h>

// create a SoftwareSerial object
SoftwareSerial myBluetooth(12, 13); // RX, TX

void setup() {
  // Open the hardware serial port to communicate with your computer
  Serial.begin(9600);

  // Set the baud rate for the SoftwareSerial port
  // This should match the baud rate set for your Bluetooth module (usually 9600 or 38400)
  myBluetooth.begin(9600);
}

void loop() {
  Serial.println("Hi");
  delay(1000);
}
