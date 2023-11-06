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

// Buzzer
const int buzzerPin = 3;
const int moveFrequency = 1200;
const int toggleFrequency = 440;
const int resetFrequency = 4400;
const int buzzTime = 350;

// Joystick variables
byte swState = HIGH;
byte lastSwState = HIGH;
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
volatile byte segmentStates[segSize] = {
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
const unsigned long longButtonPressInterval = 2000;
volatile unsigned long lastButtonPressTime = 0;

// Interrupt
const unsigned long debounceInterval = 200;
volatile unsigned long lastInterruptTime = 0;
volatile bool buttonPressed = false;
volatile bool longButtonPress = false;

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

  pinMode(buzzerPin, OUTPUT);

  // Button interrupt
  attachInterrupt(digitalPinToInterrupt(pinSW), buttonPressISR, CHANGE);  // CHANGE so i can control either falling or rising, for short/long press

  Serial.begin(baudValue);
}

void loop() {
  showSegments();

  flickerCurrentSegment();

  int direction = readMovement();

  if (direction != -1) {
    // tone(buzzerPin, moveFrequency, buzzTime);
    moveSegment(direction);
  }

  // Button press with interrupt
  if (buttonPressed) {
    tone(buzzerPin, toggleFrequency, buzzTime);
    segmentStates[currentSegmentIndex] = !segmentStates[currentSegmentIndex];

    buttonPressed = false;
  }

  if (longButtonPress) {
    tone(buzzerPin, resetFrequency, buzzTime);
    resetSegments();

    longButtonPress = false;
  }

  // Simple button press
  // swState = digitalRead(pinSW);

  // // Button press
  // if (swState != lastSwState) {
  //   toggleSegment();
  // }

  // lastSwState = swState;
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

  if (nextSegmentIndexMatrix[currentSegmentIndex][direction] != -1) {
    currentSegmentIndex = nextSegmentIndexMatrix[currentSegmentIndex][direction];
  }
}

// Toggle the current segment state
// For simple button press - no interrupts
void toggleSegment() {

  if (swState == LOW) {
    // Toggle segment state
    lastButtonPressTime = millis();
    segmentStates[currentSegmentIndex] = !segmentStates[currentSegmentIndex];
  } else {
    // Long press - reset all segments to LOW
    if (millis() - lastButtonPressTime > longButtonPressInterval) {
      resetSegments();
    }
  }
}

void resetSegments() {

  for (int i = 0; i < segSize; ++i) {
    segmentStates[i] = LOW;
  }
}

// Interrupt for button press
void buttonPressISR() {
  static unsigned long interruptTime = 0;
  static byte buttonValue = HIGH;

  interruptTime = micros();

  buttonValue = digitalRead(pinSW);

  if (buttonValue == LOW) {
    // FALLING - toggle with debounce
    if (interruptTime - lastInterruptTime > debounceInterval * 1000) {
      lastButtonPressTime = interruptTime;
      buttonPressed = true;

      lastInterruptTime = interruptTime;
    }
  } else {
    // RISING - long press
    if (interruptTime - lastInterruptTime > debounceInterval * 1000) {
      if (interruptTime - lastButtonPressTime > longButtonPressInterval * 1000) {
        longButtonPress = true;
      }
    }
    
    lastInterruptTime = interruptTime;
  }
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
