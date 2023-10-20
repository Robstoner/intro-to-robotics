const int led1Pin = 9;
const int led2Pin = 10;
const int led3Pin = 11;


void setup() {
  // put your setup code here, to run once:

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  analogWrite(led1Pin, 127);
  analogWrite(led2Pin, 0);
  analogWrite(led3Pin, 127);


}
