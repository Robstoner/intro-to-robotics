int enablePin = 9;
int in1Pin = 10;
int in2Pin = 11;
int motorSpeed = 250;
boolean reverse = LOW;

void setup() {
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
}

void loop() {
  setMotor(motorSpeed, reverse);
}

void setMotor(int motorSpeed, boolean reverse) {
  analogWrite(enablePin, motorSpeed);
  digitalWrite(in1Pin, !reverse);
  digitalWrite(in2Pin, reverse);
}