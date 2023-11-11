// Define pin connections to the shift register
const int latchPin = 11;  // Pin for latching data into the register (STCP)
const int clockPin = 10;  // Pin for the shift register clock (SHCP)
const int dataPin = 12;   // Pin for the serial data input to the shift register (DS)
// Define control pins for the individual digits of the display
const int segD1 = 4;  // Control pin for the first digit
const int segD2 = 5;  // Control pin for the second digit
const int segD3 = 6;  // Control pin for the third digit
const int segD4 = 7;  // Control pin for the fourth digit
// Constants for the register size in bits
const byte regSize = 8;  // Define the register size to be 8 bits (1 byte)
// Array of digit control pins
int displayDigits[] = {
  segD1, segD2, segD3, segD4
};
const int displayCount = 4;      // Number of digits in the display
const int encodingsNumber = 16;  // Number of different character encodings
// Array to store the binary encoding for each segment of a 7-segment display for the digits 0-9 andletters A - F 
int byteEncodings[encodingsNumber] = {
  // Binary representations for each character
  //A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
  B11101110,  // A
  B00111110,  // b
  B10011100,  // C
  B01111010,  // d
  B10011110,  // E
  B10001110   // F
};

void setup() {
  // Initialize the pin modes for the shift register pins and the display control pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // Initialize and turn off all display digit control pins
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  Serial.begin(9600);  // Start serial communication (unused in this snippet)
}
void loop() {
  // Loop through all character encodings and display them sequentially
  for (int i = 0; i < encodingsNumber; i++) {
    writeReg(byteEncodings[i]);  // Write each encoding to the shift register
    delay(400);                  // Delay between each character display for visibility
  }
}
void writeReg(byte encoding) {
  // TODO: Implement the logic to write the byte array to the shift register
  digitalWrite(latchPin, LOW);
  byte msb;

  for (int i = 0; i < regSize; ++i) {

    msb = bitRead(encoding, regSize - i - 1);

    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, msb);
    digitalWrite(clockPin, HIGH);
  }

  digitalWrite(latchPin, HIGH);
}