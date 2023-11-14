#include <EEPROM.h>
byte currentValue = 0;
void setup() {
  Serial.begin(9600);
  for (int index = 0; index < EEPROM.length(); index++) {
    // EEPROM[index] += 1; // increment the values of each cell by 1
    currentValue = EEPROM.read(index);
    Serial.print("EEPROM[");
    Serial.print(index);
    Serial.print("] = ");
    Serial.println(currentValue);
  }
}
void loop() {}