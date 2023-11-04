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
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

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
byte state = LOW;

// Use a struct to store the pins
// So I can use the same indexes with any pin configuration
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
byte segmentStates[segSize] = {
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};

bool commonAnode = false;  // Modify if you have common anode

// Movement
const int nrDirections = 4;
const short int nextSegmentIndexMatrix[segSize][nrDirections] = {
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

// Flicker Segment
const int flickerInterval = 100;
unsigned long lastFlicker = 0;
byte currentSegmentState = HIGH;

// Long press
unsigned long lastButtonPressTime = 0;
const int longButtonPressInterval = 2000;

void setup() {
  // Check if the display is common anode and set the state accordingly
  if (commonAnode) {
    state = !state;
  }

  // Initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i].pin, OUTPUT);

    // Set all pins to LOW
    digitalWrite(segments[i].pin, state);
  }

  pinMode(pinVRx, INPUT);
  pinMode(pinVRy, INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  Serial.begin(baudValue);
}

void loop() {
  showSegments();

  flickerCurrentSegment();

  int direction = readMovement();

  if (direction != -1) {
    moveSegment(direction);
  }

  swState = digitalRead(pinSW);

  // Button press
  if (swState != lastSwState) {
    if (swState == LOW) {
      // Toggle segment state
      lastButtonPressTime = millis();
      segmentStates[currentSegmentIndex] = !segmentStates[currentSegmentIndex];
    } else {
      // Long press - reset all segments to LOW
      if (millis() - lastButtonPressTime > longButtonPressInterval) {
        for (int i = 0; i < segSize; ++i) {
          segmentStates[i] = LOW;
        }
      }
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
    // up
    joyMoved = true;

    return 0;
  } else if (yValue < minThreshold && !joyMoved) {
    // down
    joyMoved = true;

    return 1;
  } else if (xValue > maxThreshold && !joyMoved) {
    // left
    joyMoved = true;

    return 2;
  } else if (xValue < minThreshold && !joyMoved) {
    // right
    joyMoved = true;

    return 3;
  }

  // back to center
  if ((xValue >= minThreshold && xValue <= maxThreshold) && (yValue >= minThreshold && yValue <= maxThreshold)) {
    joyMoved = false;
    return -1;
  }

  return -1;
}

/*
 * Move the current segment according to the matrix received in the lab
 * using the index to pin struct array
*/
void moveSegment(int direction) {
  int nextSegmentIndex = nextSegmentIndexMatrix[currentSegmentIndex][direction];

  if (nextSegmentIndex != -1) {

    digitalWrite(segments[currentSegmentIndex].pin, state);

    currentSegmentIndex = nextSegmentIndex;

    digitalWrite(segments[currentSegmentIndex].pin, !state);
  }
}

void toggleSegment() {
}

void flickerCurrentSegment() {

  if (millis() - lastFlicker > flickerInterval) {
    currentSegmentState = !currentSegmentState;
    digitalWrite(segments[currentSegmentIndex].pin, currentSegmentState);

    lastFlicker = millis();
  }
}

// Display the segments based on the segmentStates array
// Excluding the current one (it is flickering)
void showSegments() {
  for (int i = 0; i < segSize; ++i) {
    if (i != currentSegmentIndex) {
      digitalWrite(segments[i].pin, segmentStates[i]);
    }
  }
}
