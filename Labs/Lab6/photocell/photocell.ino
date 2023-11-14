int photocellPin = 0;  // the cell and 10K pulldown are connected to a0
int photocellValue;    // the analog reading from the sensor divider
void setup(void) {
  Serial.begin(9600);
}
void loop(void) {
  photocellValue = analogRead(photocellPin);
  Serial.print("Analog reading = ");
  Serial.println(photocellValue);  // the raw analog reading
  delay(100);
}