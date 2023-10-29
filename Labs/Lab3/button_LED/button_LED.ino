const int buttonPin = 9;
const int ledPin = 7;
byte buttonState = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  buttonState = digitalRead(buttonPin);

  Serial.println(buttonState);
  digitalWrite(ledPin, buttonState);
  
  delay(100);
  
}
