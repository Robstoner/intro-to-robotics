// DS = [D]ata [S]torage - data
// STCP = [ST]orage [C]lock [P]in latch
// SHCP = [SH]ift register [C]lock [P]in clock
const byte latchPin = 11;  // STCP to 12 on Shift Register
const byte clockPin = 10;  // SHCP to 11 on Shift Register
const byte dataPin = 12;   // DS to 14 on Shift Register
// Pin assignments for controlling the common cathode/anode pins of the 7-segment digits
const byte segD1 = 4;
const byte segD2 = 5;
const byte segD3 = 6;
const byte segD4 = 7;
// Size of the register in bits
const byte regSize = 8;
// Array to keep track of the digit control pins
byte displayDigits[] = {
  segD1, segD2, segD3, segD4
};
const byte displayCount = 4;  // Number of digits in the display
// Array representing the state of each bit in the shift register
byte registers[regSize];

void setup() {
  // Initialize the digital pins connected to the shift register as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // Initialize the digit control pins as outputs and turn them off
  for (byte i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  // Decomment any of the following lines to turn off the specific digit.
  // digitalWrite(displayDigits[0], HIGH);
  // digitalWrite(displayDigits[1], HIGH);
  // digitalWrite(displayDigits[2], HIGH);
  // digitalWrite(displayDigits[3], HIGH);
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Turn on all segments in forward order and print the state
  for (int i = 0; i < regSize; i++) {
    registers[i] = HIGH;
    writeReg(registers);
    printRegisters();  // Print the state for debugging
    delay(200);        // Small delay to see the effect on the display
  }
  Serial.println();  // New line for readability
  // Turn off all segments in reverse order and print the state
  for (int i = regSize - 1; i >= 0; i--) {
    registers[i] = LOW;
    writeReg(registers);
    printRegisters();  // Print the state for debugging
    delay(200);        // Small delay to see the effect on the display
  }
  Serial.println();  // New line for readability
}

// Function to write the contents of the 'encoding' array to the shift register
void writeReg(byte encoding[]) {
  digitalWrite(latchPin, LOW);           // Begin sending data to the shift register
  for (int i = 0; i < regSize; i++) {    // Loop over each bit
    digitalWrite(clockPin, LOW);         // Prepare to send the bit
    digitalWrite(dataPin, encoding[i]);  // Send the bit
    digitalWrite(clockPin, HIGH);        // Clock in the bit
  }
  digitalWrite(latchPin, HIGH);  // Latch the data, updating the output of the shift register
}

void printRegisters() {
  for (int i = 0; i < regSize; i++) {
    Serial.print(registers[i]);
  }
  Serial.println();
}