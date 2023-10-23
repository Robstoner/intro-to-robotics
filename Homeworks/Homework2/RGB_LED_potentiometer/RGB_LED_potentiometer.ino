// Pins for the 3 LED channels (R, G, B)
const int ledPinR = 9;
const int ledPinG = 10;
const int ledPinB = 11;

// Pins for the 3 potentiometer inputs (analog)
const int inputPinR = A5;
const int inputPinG = A4;
const int inputPinB = A3;

// Potentiometer values
int inputValueR = 0;
int inputValueG = 0;
int inputValueB = 0;

// Old LED values to be able to compare and filter flickering
int oldLedValueR = 0;
int oldLedValueG = 0;
int oldLedValueB = 0;

// Constants
const int serialBaud = 9600;
const int mapInputStart = 0;
const int mapInputEnd = 1023;
const int mapOutputStart = 0;
const int mapOutputEnd = 255;
const int flickerInterval = 1;
const int filterBottom = 20;
const int filterTop = 240;

// Initiate the pins and the Serial connection
void setup() {

  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);

  pinMode(inputPinR, INPUT);
  pinMode(inputPinG, INPUT);
  pinMode(inputPinB, INPUT);

  Serial.begin(serialBaud);
}

/* 
  Take the analog inputs from the potentiometers.
  Convert it to analog outputs (from 0-1023 to 0-255).
  Filter the inputs to take into account the fluctuation between two adjacent values.
  Filter the upper and lower boundary to avoid flickering.
  Write the values into the RGB channels using PWM digital output to emulate an analog output.
*/
void loop() {

  inputValueR = analogRead(inputPinR);
  inputValueG = analogRead(inputPinG);
  inputValueB = analogRead(inputPinB);

  // Input mapping from 0-1023 to 0-255
  int ledValueR = map(inputValueR, mapInputStart, mapInputEnd, mapOutputStart, mapOutputEnd);
  int ledValueG = map(inputValueG, mapInputStart, mapInputEnd, mapOutputStart, mapOutputEnd);
  int ledValueB = map(inputValueB, mapInputStart, mapInputEnd, mapOutputStart, mapOutputEnd);

  // Input filtering for fluctuations
  if ((abs(ledValueR - oldLedValueR) > flickerInterval || abs(ledValueG - oldLedValueG) > flickerInterval || abs(ledValueB - oldLedValueB) > flickerInterval)) {

    oldLedValueR = ledValueR;
    oldLedValueG = ledValueG;
    oldLedValueB = ledValueB;

    // Output filtering for flickering
    if (ledValueR < filterBottom ) {
      analogWrite(ledPinR, 0);
    } else if (ledValueR > filterTop) {
      analogWrite(ledPinR, 255);
    } else {
      analogWrite(ledPinR, ledValueR);
    }

    if (ledValueG < filterBottom) {
      analogWrite(ledPinG, 0);
    } else if (ledValueG > filterTop) {
      analogWrite(ledPinG, 255);
    } else {
      analogWrite(ledPinG, ledValueG);
    }

    if (ledValueB < filterBottom) {
      analogWrite(ledPinB, 0);
    } else if (ledValueB > filterTop) {
      analogWrite(ledPinB, 255);
    } else{
      analogWrite(ledPinB, ledValueB);
    }
    
    // Serial print to show the actual values for creating custom colors
    Serial.println((String) "R:" + ledValueR + "   G:" + ledValueG + "   B:" + ledValueB);
  }
}
