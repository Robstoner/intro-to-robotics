char incomingByte = 0;  // for incoming serial data

String inputString = "";
bool stringComplete = false;

void setup() {
  Serial.begin(9600);
}
void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = (char)Serial.read();
    inputString += incomingByte;
    // say what you got:

    if (incomingByte == '\n') {
      stringComplete = true;
    }
    if (stringComplete) {
      Serial.print("I received: ");
      Serial.println(inputString);
      inputString = "";
      stringComplete = false;
    }
  }
}