const int buttonPin = 9;
const int ledPin = 7;

byte ledState = 0;

byte oldButtonState = 0;
byte buttonState = 0;
int buttonPushCounter = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  buttonState = digitalRead(buttonPin);


  if (buttonState != oldButtonState) {
    if (buttonState == LOW) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      buttonPushCounter++;
      Serial.println(buttonPushCounter);
    }
  }

  oldButtonState = buttonState;
  
}
