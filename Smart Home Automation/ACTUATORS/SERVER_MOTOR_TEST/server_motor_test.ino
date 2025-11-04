#include <ESP32_Servo.h>
Servo servo1;
int minUs = 1000;
int maxUs = 2800;
int servo1Pin = 5;
int pos = 0;
void setup() {
  // put your setup code here, to run once:
servo1.attach(servo1Pin, minUs, maxUs);
}

void loop() {
  // put your main code here, to run repeatedly:
servo1.write(pos);
delay(20);
}
