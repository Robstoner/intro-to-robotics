int photocellPin = A0;    // the cell and 10K pulldown are connected to a0
int photocellValue = 0;  // the analog reading from the sensor divider
int ledPin = 11;
int ledValue = 0;
void setup(void) {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}
void loop(void) {
  photocellValue = analogRead(photocellPin);
  Serial.print("Analog reading = ");
  Serial.println(photocellValue);  // the raw analog reading
  // it is useful to invert the reading so that we get a higher value when there is less light
  photocellValue = 1023 - photocellValue;
  // we need to map the value from 0..1023 to 0..255
  ledValue = map(photocellValue, 0, 1023, 0, 255);
  analogWrite(ledPin, ledValue);
  delay(100);
}