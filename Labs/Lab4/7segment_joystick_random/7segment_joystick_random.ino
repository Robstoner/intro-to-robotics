// Declare all the segments pins
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

// Joystick pins
const int pinVRx = A0;
const int pinVRy = A1;
const int pinSW = 2;

// Joystick variables
byte swState = LOW;
byte lastSwState = LOW;
int yValue = 0;
int xValue = 0;
bool joyMoved = false;
const int minThreshold = 400;
const int maxThreshold = 600;

// 7 segment variables
const int segSize = 8;
int digit = 0;

bool commonAnode = false;  // Modify if you have common anode
const int noOfDigits = 10;
byte state = HIGH;
byte dpState = LOW;
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte digitMatrix[noOfDigits][segSize - 1] = {
  // a  b  c  d  e  f  g
  { 1, 1, 1, 1, 1, 1, 0 },  // 0
  { 0, 1, 1, 0, 0, 0, 0 },  // 1
  { 1, 1, 0, 1, 1, 0, 1 },  // 2
  { 1, 1, 1, 1, 0, 0, 1 },  // 3
  { 0, 1, 1, 0, 0, 1, 1 },  // 4
  { 1, 0, 1, 1, 0, 1, 1 },  // 5
  { 1, 0, 1, 1, 1, 1, 1 },  // 6
  { 1, 1, 1, 0, 0, 0, 0 },  // 7
  { 1, 1, 1, 1, 1, 1, 1 },  // 8
  { 1, 1, 1, 1, 0, 1, 1 }   // 9
};

void setup() {
  // Initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }

  pinMode(pinVRx, INPUT);
  pinMode(pinVRy, INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  // TODO: Check if commonAnode should be modified here
  if (commonAnode) {
    state = !state;
  }
}

void loop() {

  int random1 = 0, random3 = 0, random2 = 0;

  xValue = analogRead(pinVRx);

  if (xValue < minThreshold && !joyMoved) {

    joyMoved = true;
  } else if (xValue > maxThreshold && !joyMoved) {
    joyMoved = true;
  }

  if (xValue >= minThreshold && xValue <= maxThreshold) {
    joyMoved = false;
  }

  swState = digitalRead(pinSW);

  if (swState != lastSwState) {
    digit = random() % 6 + 1;
    random1 = random() % 6 + 1;
    random2 = random() % 6 + 1;
    random3 = random() % 6 + 1;

    displayNumber(random1, dpState);
    delay(250);
    displayNumber(random2, dpState);
    delay(250);
    displayNumber(random3, dpState);
    delay(250);
  }

  lastSwState = swState;

  displayNumber(digit, dpState);
  delay(1);
}

void displayNumber(byte digit, byte decimalPoint) {
  // TODO: Write the code to display the given digit on the 7-segment display
  digitalWrite(segments[segSize - 1], decimalPoint);

  for (int i = 0; i < segSize - 1; ++i) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
}