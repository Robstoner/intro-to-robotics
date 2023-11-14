#include <EEPROM.h>

byte nrResets = 0;

void setup() {

  Serial.begin(9600);
  nrResets = EEPROM.read(696);
  Serial.println((String) "Last nr of resets:" + nrResets);
  nrResets++;
  EEPROM.update(696, nrResets);
  Serial.println((String) "New nr of resets:" + nrResets);

}

void loop() {
  // put your main code here, to run repeatedly:

}
