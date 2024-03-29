#include <Servo.h>

const byte servoPin = 9;
int angle = 0;  // servo position in degrees
Servo servo;

void setup() {
  servo.attach(servoPin);
}
void loop() {

  // scan from 0 to 180 degrees
  for (angle = 0; angle < 180; angle++) {
    servo.write(angle);
    delay(15);
  }
  // now scan back from 180 to 0 degrees
  for (angle = 180; angle > 0; angle--) {
    servo.write(angle);
    delay(15);
  }
}