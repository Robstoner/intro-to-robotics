#include <EEPROM.h>  //include the library
byte brightness;     //Create an empty integer variable
void setup() {
  Serial.begin(9600);    //Start serial monitor
  EEPROM.update(1, 25);  // write value 25 on address 0
  //Value must be between 0 and 255
}
void loop() {
  // read a byte from the address "0" of the EEPROM
  brightness = EEPROM.read(1);
  Serial.println(brightness);  //Print it to the monitor
  delay(500);
}