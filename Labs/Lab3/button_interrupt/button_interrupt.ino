const int interruptPin = 2;
const int ledPin = 7;
volatile byte buttonState = LOW;

void setup() {
  // put your setup code here, to run once:

  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, RISING);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(ledPin, buttonState);
}

void blink() {
  buttonState = !buttonState;
  
  Serial.println(buttonState);
}
