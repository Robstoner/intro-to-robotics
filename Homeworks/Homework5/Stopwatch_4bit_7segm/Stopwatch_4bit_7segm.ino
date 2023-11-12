/*
 * Create a stopwatch timer using a 4 digit 7 segment display and 3 buttons

 * The circuit:
 * 3 buttons
 * one 4 digit 7 segment display
 * one shift register

 * Created 11/11/2023
 * By Schmidt Robert
 * 
 * Updated 12/11/2023
 * By Schmidt Robert
*/

const int baudValue = 9600;

// Define connections to the shift register
const int dataPin = 8;
const int latchPin = 9;
const int clockPin = 10;

// Define connections to the digit control pins for a 4-digit display
const int segD1Pin = 4;
const int segD2Pin = 5;
const int segD3Pin = 6;
const int segD4Pin = 7;

// Define connections to the three buttons
const int lapButtonPin = 2;
const int pauseButtonPin = 3;
const int resetButtonPin = 11;

// Store the digits in an array for easy access
const int displayCount = 4;
int displayDigits[displayCount] = { segD1Pin, segD2Pin, segD3Pin, segD4Pin };

// Define the number of unique encodings (0-9)
const int encodingsNumber = 10;

// Define byte encodings for the digits 0-9 with DP off
byte byteEncodings[encodingsNumber] = {
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
};

// Variables for controlling the displayed time
struct customFloat {
  int whole;
  short int decimal;
} number;
int msDelay = 100;  // Increment the shown time every 100ms (10 times per second)
unsigned long lastIncrement = 0;

// Variables for the button controls
volatile bool paused = true;
byte reset = LOW;
byte resetReading = HIGH;
byte lastResetReading = HIGH;

// Variables for laps and lap viewing
bool hasBeenReset = false;              // The clock was reset to 0
volatile bool lapButtonPressed = false;
bool viewingLaps = false;
const int nrLaps = 4;
customFloat laps[nrLaps] = { 0 };
short int currentLapNr = 0;
short int nextShownLap = 0;

// Button ISR variables
volatile unsigned long lastInterruptTime = 0;
int debounceInterval = 200;

unsigned long lastDebounceTimeReset = 0;

// Interrupt Service Routine for pause button, to avoid any small delays from the rest of the code
// Debounce w/ FALLING won't work for me, works good enough without debounce
void pauseButtonISR() {
  static unsigned long interruptTime = 0;
  interruptTime = micros();

  if (interruptTime - lastInterruptTime > debounceInterval * 1000) {

    paused = !paused;
  }
  lastInterruptTime = interruptTime;
}

void lapButtonISR() {
  static unsigned long interruptTime = 0;
  interruptTime = micros();

  if (interruptTime - lastInterruptTime > debounceInterval * 1000) {

    lapButtonPressed = true;
  }
  lastInterruptTime = interruptTime;
}

void resetButtonDebounce() {
  resetReading = digitalRead(resetButtonPin);

  if (resetReading != lastResetReading) {
    lastDebounceTimeReset = millis();
  }

  if ((millis() - lastDebounceTimeReset) > debounceInterval) {
    // Default is HIGH
    if (resetReading == LOW) {
      reset = true;
    }
  }

  lastResetReading = resetReading;
}

void setup() {
  // Initialize the pins connected to the shift register as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // Initialize the pins connected to the buttons
  pinMode(resetButtonPin, INPUT_PULLUP);
  pinMode(pauseButtonPin, INPUT_PULLUP);
  pinMode(lapButtonPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pauseButtonPin), pauseButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(lapButtonPin), lapButtonISR, FALLING);

  // Initialize digit control pins and set them to LOW (off)
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  // Begin serial communication for debugging purposes
  Serial.begin(baudValue);
}

void loop() {

  resetButtonDebounce();

  // Reset laps and everything associated to them
  if (reset) {
    if (viewingLaps) {
      Serial.println("Reset laps.");
      for (int i = 0; i < nrLaps; ++i) {
        laps[i].whole = 0;
        laps[i].decimal = 0;
      }

      currentLapNr = 0;
      nextShownLap = 0;

      hasBeenReset = false;
      viewingLaps = false;
      reset = false;
    }
  }

  if (viewingLaps) {
    paused = true;
  }

  if (!paused) {
    // To not allow the viewing of lap times if clock has not been reset yet while paused
    // Without it would allow going through the laps while clock was paused but not at 0
    // after you would reset it once, and also reset the laps and start again from scratch
    hasBeenReset = false;
    runningClock();
  } else {
    pausedClock();
  }

  // Display the current time to the display
  writeNumber(number);

  // Set the reset variable to false
  // In case it gets pressed while the clock is running
  reset = false;
}

// Controls for when the clock is running
// Increment it, and save laps
void runningClock() {
  if (lapButtonPressed) {
    saveLap();

    lapButtonPressed = false;
  }

  if (millis() - lastIncrement > msDelay) {
    if (number.whole < 999 || number.decimal < 9) {
      number.decimal++;

      if (number.decimal > 9) {
        number.whole++;
        number.decimal = 0;
      }

      lastIncrement = millis();
    }
  }
}

// Save the current time as a lap
void saveLap() {
  Serial.println((String) "Saved lap: " + number.whole + "." + number.decimal);
  laps[currentLapNr].whole = number.whole;
  laps[currentLapNr].decimal = number.decimal;

  currentLapNr++;
  if (currentLapNr >= nrLaps) {
    currentLapNr = 0;
  }
}

// Controls for when the clock is in the paused state/lap viewing
void pausedClock() {
  if (lapButtonPressed && hasBeenReset) {
    viewingLaps = true;

    number.whole = laps[nextShownLap].whole;
    number.decimal = laps[nextShownLap].decimal;

    Serial.println((String) "Showing lap: " + number.whole + "." + number.decimal);
    
    nextShownLap++;
    if (nextShownLap >= nrLaps) {
      nextShownLap = 0;
    }
  }
  // Set to false in case it didn't enter the if
  lapButtonPressed = false;

  if (reset) {
    number.whole = 0;
    number.decimal = 0;

    reset = false;
    hasBeenReset = true;
  }
}

/*
 * Write the digit to the shift register
 * To be decoded and sent to the display
*/
void writeReg(int digit) {
  digitalWrite(latchPin, LOW);

  shiftOut(dataPin, clockPin, MSBFIRST, digit);

  digitalWrite(latchPin, HIGH);
}

// Activate only the digit of the display that we want to write to
void activateDisplay(int displayNumber) {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }

  digitalWrite(displayDigits[displayNumber], LOW);
}

// Check if we are on the 3rd display digit, if yes, turn on the decimal point
int checkDecimal(int digit, int displayNumber) {
  if (displayNumber == 2) {
    digit += 1;
  }

  return digit;
}

void writeNumber(customFloat number) {
  int currentNumber = number.whole;
  int displayDigit = 3;  // Start with the least significant digit

  // Display the decimal
  sendDigit(byteEncodings[number.decimal], displayDigit);
  displayDigit--;

  // Loop through the digits of the number
  // And display each one, activating only the current digit display
  int lastDigit = 0;

  while (currentNumber != 0 && displayDigit >= 0) {
    lastDigit = currentNumber % 10;

    sendDigit(byteEncodings[lastDigit], displayDigit);

    displayDigit--;
    currentNumber /= 10;
  }

  while (displayDigit >= 0) {
    sendDigit(byteEncodings[0], displayDigit);
    displayDigit--;
  }
}

void sendDigit(int encoding, int displayDigit) {
  activateDisplay(displayDigit);
  // Check if it's needed to turn on the decimal point (3rd display only)
  int checkedEncoding = checkDecimal(encoding, displayDigit);
  writeReg(checkedEncoding);

  writeReg(B00000000);  // Clear the register to avoid ghosting
}