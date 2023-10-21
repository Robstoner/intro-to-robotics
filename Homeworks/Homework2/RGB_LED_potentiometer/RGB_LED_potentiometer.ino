// Pins for the 3 LED channels (R, G, B)
const int ledPinR = 9;
const int ledPinG = 10;
const int ledPinB = 11;

// Pins for the 3 potentiometer inputs (analog)
const int potPinR = A5;
const int potPinG = A4;
const int potPinB = A3;

// Potentiometer values
int potValR = 0;
int potValG = 0;
int potValB = 0;

// 
int oldLedValR = 0;
int oldLedValG = 0;
int oldLedValB = 0;

// Initiate the pins and the Serial connection
void setup() {

  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);

  pinMode(potPinR, INPUT);
  pinMode(potPinG, INPUT);
  pinMode(potPinB, INPUT);

  Serial.begin(9600);
}

/* 
  Take the analog inputs from the potentiometers.
  Convert it to analog outputs (from 0-1023 to 0-255).
  Filter the inputs to take into account the fluctuation between two adjacent values.
  Write the values into the RGB channels using PWM digital output to emulate an analog output.
*/
void loop() {

  potValR = analogRead(potPinR);
  potValG = analogRead(potPinG);
  potValB = analogRead(potPinB);

  // Input mapping from 0-1023 to 0-255
  int ledValR = map(potValR, 0, 1023, 0, 255);
  int ledValG = map(potValG, 0, 1023, 0, 255);
  int ledValB = map(potValB, 0, 1023, 0, 255);

  // input filtering
  if (abs(ledValR - oldLedValR) > 1 || abs(ledValG - oldLedValG) > 1 || abs(ledValB - oldLedValB) > 1) {

    oldLedValR = ledValR;
    oldLedValG = ledValG;
    oldLedValB = ledValB;

    analogWrite(ledPinR, ledValR);
    analogWrite(ledPinG, ledValG);
    analogWrite(ledPinB, ledValB);

    // Serial print to show the actual values for creating custom colors
    Serial.println((String) "R:" + ledValR + "   G:" + ledValG + "   B:" + ledValB);
  }
}
