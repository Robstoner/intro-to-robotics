const int buttonPin = 2;
const int ledPin = 7;

volatile bool buttonPressed = false;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 100;

bool ledState = LOW;
bool lastButtonState = HIGH;
bool buttonState = HIGH;

void setup() {

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(buttonPin), handleInterrupt, FALLING);
}

void loop() {

  if (buttonPressed) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      lastDebounceTime = millis();
      
      buttonState = digitalRead(buttonPin);
      if (buttonState != lastButtonState) {
        
        if (lastButtonState == HIGH) {
          ledState = !ledState;
          digitalWrite(ledPin, ledState);
        }
        
        lastButtonState = !lastButtonState;
        buttonPressed = false;
      }
    }
  }
}

void handleInterrupt() {
  buttonPressed = true;
}