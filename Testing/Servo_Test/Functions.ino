void moveServo(Servo &servo, int position, int duration) {
  servo.writeMicroseconds(position);
  delay(duration);
}
