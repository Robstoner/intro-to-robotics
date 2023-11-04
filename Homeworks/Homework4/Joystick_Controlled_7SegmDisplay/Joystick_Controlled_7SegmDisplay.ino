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
byte state = LOW;
struct segments {
  int pin;
} segments[segSize] = {
  { pinA },
  { pinB },
  { pinC },
  { pinD },
  { pinE },
  { pinF },
  { pinG },
  { pinDP }
};

bool commonAnode = false;  // Modify if you have common anode

// Movement
const int nrDirections = 4;
short int nextSegmentIndexMatrix[segSize][nrDirections] = {
  // UP DOWN LEFT RIGHT
  { -1, 6, 5, 1 },   // a
  { 0, 6, 5, -1 },   // b
  { 6, 3, 4, 7 },    // c
  { 6, -1, 4, 2 },   // d
  { 6, 3, -1, 2 },   // e
  { 0, 6, -1, 1 },   // f
  { 0, 3, -1, -1 },  // g
  { -1, -1, 2, -1 }  // dp
};
int currentSegmentIndex = segSize - 1;

void setup() {
  // Check if the display is common anode and set the state accordingly
  if (commonAnode) {
    state = !state;
  }

  // Initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i].pin, OUTPUT);

    digitalWrite(segments[i].pin, state);
  }

  pinMode(pinVRx, INPUT);
  pinMode(pinVRy, INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  // Initialize DP as HIGH
  digitalWrite(segments[currentSegmentIndex].pin, !state);

  Serial.begin(baudValue);
}

void loop() {

  int direction = readMovement();

  if (direction != -1) {
    moveSegment(currentSegmentIndex, direction);
  }
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

void moveSegment(int &currentSegmentIndex, int direction) {
  int nextSegmentIndex = nextSegmentIndexMatrix[currentSegmentIndex][direction];

  if (nextSegmentIndex != -1) {

    digitalWrite(segments[currentSegmentIndex].pin, state);

    currentSegmentIndex = nextSegmentIndex;

    digitalWrite(segments[currentSegmentIndex].pin, !state);
  }
}
