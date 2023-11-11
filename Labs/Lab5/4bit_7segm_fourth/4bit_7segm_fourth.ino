// Pin definitions for interfacing with the shift register
const int latchPin = 11;  // Connects to STCP of the shift register
const int clockPin = 10;  // Connects to SHCP of the shift register
const int dataPin = 12;   // Connects to DS of the shift register
// Pin definitions for controlling the individual digits of the 7-segment display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;
// Constant for the size of the shift register in bits (8 bits for a standard register)
const byte regSize = 8;
// Array to hold the pins that control the segments of each digit of the display
int displayDigits[] = {
  segD1, segD2, segD3, segD4
};
const int displayCount = 4;      // Total number of digits in the display
const int encodingsNumber = 16;  // Total number of encodings for the hexadecimal characters (0-F)
// Array holding binary encodings for numbers and letters on a 7-segment display
byte byteEncodings[encodingsNumber] = {
  // Encoding for segments A through G and the decimal point (DP)
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
  // Setup function is run once at the start of the program
  // Set the shift register pins as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // Initialize the digit control pins and set them to an off state
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  Serial.begin(9600);  // Begin serial communication, not used in this code snippet
}
void loop() {
  // Main loop runs indefinitely
  for (int i = 0; i < encodingsNumber; i++) {
    writeReg(byteEncodings[i]);  // Write each encoding to the shift register to display it
    delay(400);                  // Wait for 400ms between updates to the display
  }
}
void writeReg(int encoding) {
  // Function to output a byte to the shift register
  digitalWrite(latchPin, LOW);  // Pull latch low to start data transfer
  // Shift out the bits of the 'encoding' to the shift register
  shiftOut(dataPin, clockPin, MSBFIRST, encoding);  // MSBFIRST means the most significant bit is shifted out first
  // Pull latch high to transfer data from shift register to storage register
  digitalWrite(latchPin, HIGH);  // This action updates the output of the shift register
}