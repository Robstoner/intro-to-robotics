// Define pin connections to the shift register
const int latchPin = 11;  // Pin for latching data into the register
const int clockPin = 10;  // Pin for the shift register clock
const int dataPin = 12;   // Pin for the serial data input to the shift register
// Define control pins for the individual digits of the display
const int segD1 = 4;  // Control pin for the first digit
const int segD2 = 5;  // Control pin for the second digit
const int segD3 = 6;  // Control pin for the third digit
const int segD4 = 7;  // Control pin for the fourth digit
// Constants for the register and number of encodings
const byte regSize = 8;  // Number of bits in the register
// Array of digit control pins
int displayDigits[] = {
  segD1, segD2, segD3, segD4
};
const int displayCount = 4;      // Number of digits in the display
const int encodingsNumber = 16;  // Number of different character encodings
// Encoding array representing the segments of the display for numbers 0-9 and letters A-F
byte encodingArray[encodingsNumber][regSize] = {
  // A B C D E F G DP
  { 1, 1, 1, 1, 1, 1, 0, 0 },  // 0
  { 0, 1, 1, 0, 0, 0, 0, 0 },  // 1
  { 1, 1, 0, 1, 1, 0, 1, 0 },  // 2
  { 1, 1, 1, 1, 0, 0, 1, 0 },  // 3
  { 0, 1, 1, 0, 0, 1, 1, 0 },  // 4
  { 1, 0, 1, 1, 0, 1, 1, 0 },  // 5
  { 1, 0, 1, 1, 1, 1, 1, 0 },  // 6
  { 1, 1, 1, 0, 0, 0, 0, 0 },  // 7
  { 1, 1, 1, 1, 1, 1, 1, 0 },  // 8
  { 1, 1, 1, 1, 0, 1, 1, 0 },  // 9
  { 1, 1, 1, 0, 1, 1, 1, 0 },  // A
  { 0, 0, 1, 1, 1, 1, 1, 0 },  // b
  { 1, 0, 0, 1, 1, 1, 0, 0 },  // C
  { 0, 1, 1, 1, 1, 0, 1, 0 },  // d
  { 1, 0, 0, 1, 1, 1, 1, 0 },  // E
  { 1, 0, 0, 0, 1, 1, 1, 0 },  // F
};
void setup() {
  // Initialize the shift register pins and the display control pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // Initialize digit control pins and set them to off
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  Serial.begin(9600);  // Start serial communication (not used in this program)
}
void loop() {
  // Sequentially display all encodings on the 7-segment display
  for (int i = 0; i < encodingsNumber; i++) {
    writeReg(encodingArray[i]);  // Write the encoding to the shift register
    delay(400);                  // Wait 400 milliseconds before displaying the next character
  }
}
void writeReg(byte encoding[]) {
  // Send each bit of the encoding to the shift register
  digitalWrite(latchPin, LOW);  // Prepare to send data
  for (int i = 0; i < regSize; i++) {
    digitalWrite(clockPin, LOW);         // Set the clock pin low before sending data
    digitalWrite(dataPin, encoding[i]);  // Send the data bit
    digitalWrite(clockPin, HIGH);        // Clock the data bit into the register
  }
  digitalWrite(latchPin, HIGH);  // Latch the data into the register to update the display
}