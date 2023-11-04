/*
 * Joystick controller 7 segment display
 *
 * Control what is shown on a single 7 segment display using a joystick
 * by turning on or off each segment of the display, moving through them
 * with the joystick and toggling them with the joystick button
 *
 * The circuit:
 * - one 7 segment display
 * - one joystick
 *
 * Created 02/11/2023
 * By Schmidt Robert
 * Updated 04/11/2023
 * By Schmidt Robert
*/

const int baudValue = 9600;

// Declare all the segments pins
const int pinA = 2;
const int pinB = 3;
const int pinC = 4;
const int pinD = 5;
const int pinE = 6;
const int pinF = 7;
const int pinG = 8;
const int pinDP = 9;
// How much to the right the pins are shifted from 0 (for array indexing)
const int pinShift = 2;

// Joystick pins
const int pinVRx = A0;
const int pinVRy = A1;
const int pinSW = 11;

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
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
int segment = 0;
byte state = HIGH;
byte dpState = LOW;

bool commonAnode = false;  // Modify if you have common anode

// Movement
const int nrDirections = 4;
byte nextSegmentMatrix[segSize][nrDirections] = {
  // UP DOWN LEFT RIGHT
  { 0, pinG, pinF, pinB },      // a
  { pinA, pinG, pinF, 0 },      // b
  { pinG, pinD, pinE, pinDP },  // c
  { pinG, 0, pinE, pinC },      // d
  { pinG, pinD, 0, pinC },      // e
  { pinA, pinG, 0, pinB },      // f
  { pinA, pinD, 0, 0 },         // g
  { 0, 0, pinC, 0 }             // dp
}; 
int currentSegment = pinDP;

void setup() {
  // Initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);

    digitalWrite(segments[i], LOW);
  }

  pinMode(pinVRx, INPUT);
  pinMode(pinVRy, INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  digitalWrite(currentSegment, HIGH);

  // TODO: Check if commonAnode should be modified here
  if (commonAnode) {
    state = !state;
  }

  Serial.begin(baudValue);
}

void loop() {

  int direction = readMovement();

  if (direction != -1) {
    moveSegment(currentSegment, direction);
  }


  swState = digitalRead(pinSW);

  if (swState != lastSwState) {
    if (swState == LOW) {
    }
  }

  lastSwState = swState;
}

/*
 * Read the joystick movement and return a value representing the direction
 * -1 - static
 * 0 - up
 * 1 - down
 * 2 - left
 * 3 - right
*/
int readMovement() {
  xValue = analogRead(pinVRx);
  yValue = analogRead(pinVRy);

  if (yValue > maxThreshold && !joyMoved) {
    joyMoved = true;

    return 0;
  } else if (yValue < minThreshold && !joyMoved) {
    joyMoved = true;

    return 1;
  } else if (xValue > maxThreshold && !joyMoved) {
    joyMoved = true;

    return 2;
  } else if (xValue < minThreshold && !joyMoved) {
    joyMoved = true;

    return 3;
  }

  if ((xValue >= minThreshold && xValue <= maxThreshold) && (yValue >= minThreshold && yValue <= maxThreshold)) {
    joyMoved = false;
    return -1;
  }

  return -1;
}

void moveSegment(int &currentSegment, int direction) {
  // substract pinShift to be able to use the pin's number as an array index
  // !! not good because pins have to be in specific order
  int nextSegment = nextSegmentMatrix[currentSegment - pinShift][direction];

  if (nextSegment != 0) {

    digitalWrite(currentSegment, LOW);

    currentSegment = nextSegment;

    digitalWrite(currentSegment, HIGH);
  }
}
