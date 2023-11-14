const int irSensorPin = A0;
float irSensorvalue = 0;
float volts = 0.0;
int distance = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
  float irSensorValue = analogRead(irSensorPin);
  float voltage = irSensorValue * 5 / 1024;  // switching from 0..1023 to 0..5V
  float distance = 13 * pow(voltage, -1);    // extracted from the datasheet
  delay(100);
  Serial.println(distance);
}