#include "notes.h"

const int buzzerPin = 9;

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_A3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  // put your setup code here, to run once:

  pinMode(buzzerPin, OUTPUT);

  for (int thisNote = 0, thisNote < 8; ++thisNote) {

    tone(buzzerPin, melody[thisNote], noteDuration[thisNote]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
