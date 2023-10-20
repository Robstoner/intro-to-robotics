const int ledPin = 9;

bool sw = 0;
long prevSw = 0;
const int intervalSw = 255;
long prev = 0;
const int interval = 1;

int intensity = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (millis() - prevSw > intervalSw) {
    sw = !sw;
    prevSw = millis();
  }

  if (millis() - prev > interval) {

    analogWrite(ledPin, intensity);
    if (sw)
      intensity--;
    else
      intensity++;
    
    prev = millis();
  }
}