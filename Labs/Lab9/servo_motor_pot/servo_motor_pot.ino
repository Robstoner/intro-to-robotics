#include <Servo.h>

const byte servoPin = 9;
const byte potentiometerPin = A0;

int angle = 0;  // servo position in degrees
Servo servo;

void setup() {
  servo.attach(servoPin);
}

void loop() {
  int potValue = analogRead(potentiometerPin);
  angle = map(potValue, 0, 1023, 0, 255);
  servo.write(angle);
}