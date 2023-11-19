/*
 * Smart Environment Monitor and Logger
 *
 *
 * The circuit:
 * - Ultrasonic sensor (distance)
 * - LDR (light)
 * - RGB LED
 *
 * Created 17/11/2023
 * By Schmidt Robert
 * Updated 19/11/2023
 * By Schmidt Robert
*/

// Sensor pins
const int trigPin = 5;
const int echoPin = 6;

const int ldrPin = A0;

const int blueLedPin = 9;
const int greenLedPin = 10;
const int redLedPin = 11;

// Input variables
char incomingByte = '0';

// Menu settings
// 0 - Main, 1,2,3,4 - The 4 submenus
char currentMenu = '0';
bool menuShown = false;

// Sensor settings
short int samplingInterval = 1;  // seconds
int ultrasonicThresholdMin = 10;
int ldrThreshold = 250;

// Data logging
unsigned long lastSamplingTime = 0;
const int maxSamples = 10;
int ultrasonicSamples[maxSamples] = { 0 };
int ldrSamples[maxSamples] = { 0 };
byte currentSampleNr = 0;
int ultrasonicDuration = 0;
bool showingData = false;

// LED settings
bool automaticLed = true;
byte redLedValue = 125;
byte greenLedValue = 0;
byte blueLedValue = 125;

void setup() {
  // Initiate pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ldrPin, INPUT);

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  sampleData();

  if (automaticLed) {
    automaticLedControls();
  } else {
    digitalWrite(redLedPin, redLedValue);
    digitalWrite(greenLedPin, greenLedValue);
    digitalWrite(blueLedPin, blueLedValue);
  }

  if (showingData) {
    if (Serial.available()) {
      incomingByte = (char)Serial.read();

      if (incomingByte == 'Q') {
        showingData = false;
      }
    }
  } else if (!menuShown) {
    printMenus(currentMenu);
    menuShown = true;
  }

  if (Serial.available()) {
    incomingByte = (char)Serial.read();

    if (menuShown) {
      chooseOption(incomingByte);
    }
  }
}

void sampleData() {
  if (millis() - lastSamplingTime > samplingInterval * 1000) {
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    ultrasonicDuration = pulseIn(echoPin, HIGH);

    int ultrasonicData = ultrasonicDuration * 0.034 / 2;

    int ldrData = analogRead(ldrPin);

    if (showingData) {
      Serial.print(F("Ultrasonic sensor distance: "));
      Serial.println(ultrasonicData, DEC);

      Serial.print(F("LDR data: "));
      Serial.println(ldrData, DEC);
    }

    shiftArrays();
    ultrasonicSamples[maxSamples - 1] = ultrasonicData;
    ldrSamples[maxSamples - 1] = ldrData;

    lastSamplingTime = millis();
  }
}

void shiftArrays() {
  for (int i = maxSamples - 1; i > 0; i--) {
    ultrasonicSamples[i - 1] = ultrasonicSamples[i];
    ldrSamples[i - 1] = ldrSamples[i];
  }
}

void automaticLedControls() {
  digitalWrite(blueLedPin, LOW);

  if (ultrasonicSamples[maxSamples - 1] < ultrasonicThresholdMin || ldrSamples[maxSamples - 1] < ldrThreshold) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
  } else {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
  }
}

void resetSamples() {
  Serial.print(F("Resetting samples"));
  for (int i = 0; i < maxSamples; ++i) {
    ldrSamples[i] = 0;
    ultrasonicSamples[i] = 0;

    if (i % 3 == 0) {
      Serial.print(F("."));
    }
  }
  Serial.println(F(""));
  Serial.println(F("Reset all samples successfully"));
}

void printLoggedData() {
  Serial.println(F("The last logged samples are: "));
  for (int i = 0; i < maxSamples; ++i) {
    Serial.print(F("Ultrasonic sensor distance: "));
    Serial.println(ultrasonicSamples[i], DEC);
    Serial.print(F("LDR reading: "));
    Serial.println(ldrSamples[i], DEC);
  }
}

void readLedValues() {
  Serial.println(F("Input a value for the RED channel on the LED (0-255): "));
  while (!Serial.available()) {}
  redLedValue = Serial.parseInt();
  if (redLedValue > 255) { redLedValue = 255; }
  if (redLedValue < 0) { redLedValue = 0; }

  Serial.println(F("Input a value for the GREEN channel on the LED (0-255): "));
  while (!Serial.available()) {}
  greenLedValue = Serial.parseInt();
  if (greenLedValue > 255) { greenLedValue = 255; }
  if (greenLedValue < 0) { greenLedValue = 0; }

  Serial.println(F("Input a value for the BLUE channel on the LED (0-255): "));
  while (!Serial.available()) {}
  blueLedValue = Serial.parseInt();
  if (blueLedValue > 255) { blueLedValue = 255; }
  if (blueLedValue < 0) { blueLedValue = 0; }
}

// Used a switch case to choose which menu to print, instead of 5 different functions
// 0 - main, 1,2,3,4 - each submenu respectively
void printMenus(char menu) {

  switch (menu) {
    case '0':
      Serial.println(F("1. Sensor Settings."));
      Serial.println(F("2. Reset Logger Data."));
      Serial.println(F("3. System Status."));
      Serial.println(F("4. RGB LED Control."));
      currentMenu = '0';

      break;
    case '1':
      Serial.println(F("1. Sensor Settings."));
      Serial.println(F("  1. Sensors Sampling Interval."));
      Serial.println(F("  2. Ultrasonic Alert Threshold."));
      Serial.println(F("  3. LDR Alert Threshold."));
      Serial.println(F("  4. Back."));
      currentMenu = '1';

      break;
    case '2':
      Serial.println(F("ATTENTION THIS WILL RESET ALL THE LOGGED DATA!"));
      Serial.println(F("2. Reset Logger Data. Are you sure you want to do this?"));
      Serial.println(F("  1. Yes."));
      Serial.println(F("  2. No."));
      currentMenu = '2';

      break;
    case '3':
      Serial.println(F("3. System Status."));
      Serial.println(F("  1. Current Sensor Readings."));
      Serial.println(F("  2. Current Sensor Settings."));
      Serial.println(F("  3. Display Logged Data."));
      Serial.println(F("  4. Back."));
      currentMenu = '3';

      break;
    case '4':
      Serial.println(F("4. RGB LED Control."));
      Serial.println(F("  1. Manual Color Control."));
      Serial.println(F("  2. LED: Toggle Automatic ON/OFF."));
      Serial.println(F("  3. Back."));
      currentMenu = '4';

      break;
    default:
      printMenus('0');

      break;
  }
}

void chooseOption(char option) {
  if (option == '\n') {
    return;
  }

  switch (option) {
    case '1':
      // Main menu to submenus
      if (currentMenu == '0') {
        printMenus('1');
        break;
      }

      // 2.1 Reset the stored logged samples
      if (currentMenu == '2') {
        resetSamples();
        currentMenu = '0';
        menuShown = false;
      }

      // 4.1 Input new manual LED values
      if (currentMenu == '4') {
        readLedValues();
        menuShown = false;
      }

      // 1.1 Input a new sampling interval
      if (currentMenu == '1') {
        Serial.println(F("Please input a new sensor sampling interval (1-10 s):"));

        while (!Serial.available()) {}
        samplingInterval = Serial.parseInt();
        while (samplingInterval < 1 || samplingInterval > 10) {
          Serial.println(F("Incorrent value inserted, input a number between 1 and 10: "));
          while (!Serial.available()) {}
          samplingInterval = Serial.parseInt();
        }

        menuShown = false;
        break;
      }

      // 3.1 Show taken samples continuously
      if (currentMenu == '3') {
        Serial.println(F("Now showing continuous data logging. Press Q to exit."));
        showingData = true;
        menuShown = false;
      }

      break;
    case '2':
      // Back from submenus
      if (currentMenu == '2') {
        printMenus('0');
        break;
      }

      // Main menu to submenus
      if (currentMenu == '0') {
        printMenus('2');
        break;
      }

      // 4.2 Turn Automatic LED on/off
      if (currentMenu == '4') {
        Serial.print(F("Switched led to "));
        automaticLed = !automaticLed;

        if (automaticLed) {
          Serial.println(F("automatic mode."));
        } else {
          Serial.println(F("manual mode."));
        }

        menuShown = false;
        break;
      }

      // 1.2 Input a threshold for the ultrasonic sensor
      if (currentMenu == '1') {
        Serial.println(F("Please input a new minimum threshold for the ultrasonic sensor (1-30 cm): "));

        while (!Serial.available()) {}
        ultrasonicThresholdMin = Serial.parseInt();
        while (ultrasonicThresholdMin < 1 || ultrasonicThresholdMin > 30) {
          Serial.println(F("Incorrent value inserted, input a number between 1 and 30: "));
          while (!Serial.available()) {}
          ultrasonicThresholdMin = Serial.parseInt();
        }

        menuShown = false;
        break;
      }

      // 3.2 Show system settings
      if (currentMenu == '3') {
        Serial.println(F("Current sensor settings are:"));
        Serial.print(F("  Sampling interval: "));
        Serial.println(samplingInterval, DEC);
        Serial.print(F("  Ultrasonic sensor minimum threshold: "));
        Serial.println(ultrasonicThresholdMin, DEC);
        Serial.print(F("  LDR threshold: "));
        Serial.println(ldrThreshold, DEC);

        menuShown = false;
        break;
      }

      break;
    case '3':
      // Check if current submenu has this option
      if (currentMenu == '2') {
        defaultCaseInvalidOption();
        break;
      }

      // Back from submenus
      if (currentMenu == '4') {
        printMenus('0');
        break;
      }

      // Main menu to submenus
      if (currentMenu == '0') {
        printMenus('3');
        break;
      }

      // 1.3 Input a threshold for the LDR
      if (currentMenu == '1') {
        Serial.println(F("Please input a new minimum threshold for the light dependent resistor (1-1023): "));

        while (!Serial.available()) {}
        ldrThreshold = Serial.parseInt();
        while (ldrThreshold < 1 || ldrThreshold > 1023) {
          Serial.println(F("Incorrent value inserted, input a number between 1 and 1023: "));
          while (!Serial.available()) {}
          ldrThreshold = Serial.parseInt();
        }

        menuShown = false;
        break;
      }

      // 3.3 Print the last maxSamples of logged data
      if (currentMenu == '3') {
        printLoggedData();
        menuShown = false;
      }

      break;
    case '4':
      // Check if current submenu has this option
      if (currentMenu == '2' || currentMenu == '4') {
        defaultCaseInvalidOption();
        break;
      }

      // Back from submenus
      if (currentMenu == '1' || currentMenu == '3') {
        printMenus('0');
        break;
      }

      // Main menu to submenus
      if (currentMenu == '0') {
        printMenus('4');
        break;
      }

      break;
    default:
      defaultCaseInvalidOption();

      break;
  }
}

void defaultCaseInvalidOption() {
  Serial.println(F("Invalid option, please choose one of the options listed below."));
  menuShown = false;
}