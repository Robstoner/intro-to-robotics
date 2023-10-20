const int potPin = A5;
int potVal = 0;

const int ledPin = 9;

void setup() {
  // put your setup code here, to run once:

  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  potVal = analogRead(potPin);

  int ledVal = map(potVal, 0, 1023, 0, 255);

  analogWrite(ledPin, ledVal);

  Serial.println(potVal);
  delay(1);
}