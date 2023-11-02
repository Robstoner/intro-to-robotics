// Declare all the segments pins
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

const int segSize = 8;
int index = 0;

bool commonAnode = false;  // Modify if you have common anode
const int noOfDigits = 10;
byte state = HIGH;
byte dpState = LOW;
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte digitMatrix[noOfDigits][segSize - 1] = {
  // a  b  c  d  e  f  g
  { 0, 1, 1, 0, 1, 1, 1 },  // H
  { 1, 0, 0, 1, 1, 1, 1 },  // E
  { 0, 0, 0, 1, 1, 1, 0 },  // L
  { 0, 0, 0, 1, 1, 1, 0 },  // L
  { 1, 1, 1, 1, 1, 1, 0 },  // O
  { 0, 0, 0, 1, 0, 0, 0 },  // _
  { 1, 0, 1, 1, 1, 1, 1 },  // G
  { 0, 1, 1, 1, 1, 1, 0 },  // U
  { 0, 1, 1, 0, 0, 1, 1 },  // Y
  { 1, 0, 1, 1, 0, 1, 1 }   // S
};

void setup() {
  // Initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  // TODO: Check if commonAnode should be modified here
  if (commonAnode) {
    state = !state;
  }
}

void loop() {
  // TODO: Implement the logic to display numbers on the 7-segment display

  displayNumber(index, dpState);

  // TODO: Control the timing for switching between numbers
  index++;
  delay(500);
  
  if (index >= noOfDigits) {
    index = 0;
    dpState = !dpState;

  }
}

void displayNumber(byte digit, byte decimalPoint) {
  // TODO: Write the code to display the given digit on the 7-segment display
  digitalWrite(segments[segSize - 1], decimalPoint);

  for (int i = 0; i < segSize - 1; ++i) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
}