#include <LiquidCrystal.h>
const byte rs = 2;
const byte en = 3;
const byte d4 = 4;
const byte d5 = 5;
const byte d6 = 6;
const byte d7 = 7;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte customChar[8] = {
	0b00000,
	0b00100,
	0b01010,
	0b01010,
	0b10001,
	0b11111,
	0b10001,
	0b10001
};

unsigned long scrollInterval = 500;
unsigned long lastScroll = 0;
byte currentCol = 0;
bool currentRow = false;

void setup() 
{
	lcd.createChar(0, customChar);

	lcd.begin(16, 2);
	lcd.write((uint8_t)0);
}

void loop()
{
  if (millis() - lastScroll > scrollInterval) {
    lcd.scrollDisplayRight();

    currentCol++;
    if (currentCol > 15) {
      currentCol = 0;
      lcd.clear();

      currentRow = !currentRow;
      if (currentRow) {
        lcd.setCursor(0, 1);
      }

      lcd.write((uint8_t)0);
    }
    
    lastScroll = millis();
  }
}