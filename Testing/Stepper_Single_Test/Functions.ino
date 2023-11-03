void moveStepper(char a, bool b, int c, int d, int e) {
  int dir;
  int step;
  
  if (a == 'Z') {
    dir = 15;
    step = 16;
  } else if (a == 'X') {
    dir = 2;
    step = 3;
  } else {
    return;  //Invalid stepper motor
  }
  
  digitalWrite(dir, b);
  for(int x = 0; x < c; x++) {
    digitalWrite(step, HIGH);
    delayMicroseconds(d);
    digitalWrite(step, LOW);
    delayMicroseconds(d);
  }
  delay(e);
}
