const int ledPin = 9;

byte ledState = LOW;
long previousMillis = 0;
const long interval = 500;

void setup() {
  // put your setup code here, to run once:

  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    if (ledState == HIGH)
      ledState = LOW;
    else
      ledState = HIGH;

    previousMillis = millis();

    Serial.println("S-a schimbat starea LED-ului!");
  }

  digitalWrite(ledPin, ledState);
}