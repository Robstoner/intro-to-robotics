const int serialBaud = 9600;

const int nrOfFloors = 3;

// Pins
const int ledFloorPins[nrOfFloors] = { 9, 10, 11 };
const int buttonFloorPins[nrOfFloors] = { 7, 2, 4 };
const int mainLedPin = 5;
const int elevatorBuzzerPin = 3;

// Inputs
byte reading[nrOfFloors] = { HIGH, HIGH, HIGH };
byte prevReading[nrOfFloors] = { HIGH, HIGH, HIGH };
byte buttonState[nrOfFloors] = { HIGH, HIGH, HIGH };

// Debounce
unsigned long prevDebounceTime[nrOfFloors] = { 0, 0, 0 };
const int debounceDelay = 50;

// Elevator
int currentFloor = 0;
int targetFloor = 0;
unsigned long prevFloorTime = 0;
const int floorDelay = 3000;

// Main led blinking while elevator is moving
const int mainLedBlinkTime = 150;
unsigned long prevMainLedBlink = 0;
byte mainLedState = LOW;

// Button pressed for doors closing
unsigned long buttonPressedTime = 0;

// Buzzer variables
const int doorsTime = 600;
const int doorsOpeningFrequency = 587;
const int doorsClosingFrequency = 587 * 2;
const int elevatorMovingFrequency = 196;

void setup() {
  for (int i = 0; i < nrOfFloors; ++i) {
    pinMode(ledFloorPins[i], OUTPUT);
    pinMode(buttonFloorPins[i], INPUT_PULLUP);
  }

  pinMode(mainLedPin, OUTPUT);
  pinMode(elevatorBuzzerPin, OUTPUT);

  digitalWrite(ledFloorPins[0], HIGH);

  Serial.begin(serialBaud);
}

void loop() {

  // Debounce for button input
  for (int i = 0; i < nrOfFloors; ++i) {
    reading[i] = digitalRead(buttonFloorPins[i]);

    if (reading[i] != prevReading[i]) {
      prevDebounceTime[i] = millis();
    }

    if ((millis() - prevDebounceTime[i]) > debounceDelay) {

      // Button was pressed
      if (buttonState[i] != reading[i]) {
        buttonState[i] = reading[i];

        if (buttonState[i] == HIGH) {
          Serial.println((String) "Button pressed at floor " + i);

          // Only take inputs if elevator is not moving
          if (targetFloor == currentFloor) {
            targetFloor = i;

            prevFloorTime = millis();

            // Doors closing
            buttonPressedTime = millis();
            tone(elevatorBuzzerPin, doorsClosingFrequency, doorsTime);
          }
        }
      }
    }
    prevReading[i] = reading[i];
  }

  // Actual elevator code
  // Check if the floor we want is not the one we're on, and that the doors have closed
  if (targetFloor != currentFloor && ((millis() - buttonPressedTime) > doorsTime)) {
    // Elevator moving sound
    tone(elevatorBuzzerPin, elevatorMovingFrequency);

    // Main led blink
    if ((millis() - prevMainLedBlink) > mainLedBlinkTime) {
      mainLedState = !mainLedState;
      digitalWrite(mainLedPin, mainLedState);

      prevMainLedBlink = millis();
    }

    // Switch floors every floorDelay miliseconds (3 seconds)
    // Check if it's going up or down
    // If it arrived at correct floor buzz the doors opening
    if ((millis() - prevFloorTime) > floorDelay) {
      digitalWrite(ledFloorPins[currentFloor], LOW);

      prevFloorTime = millis();
      if (targetFloor > currentFloor) {
        currentFloor++;
      } else {
        currentFloor--;
      }

      digitalWrite(ledFloorPins[currentFloor], HIGH);

      // Doors opening
      if (currentFloor == targetFloor) {
        Serial.println((String) "Elevator arrived at floor " + currentFloor);
        tone(elevatorBuzzerPin, doorsOpeningFrequency, doorsTime);
      }
    }
  }
}
