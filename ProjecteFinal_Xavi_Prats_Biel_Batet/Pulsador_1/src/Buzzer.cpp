#include "Buzzer.h"

Buzzer::Buzzer(int pin) {
    this->pin = pin;
}

void Buzzer::init() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Buzzer::sonar(unsigned int durada) {
    digitalWrite(pin, HIGH);
    delay(durada);
    digitalWrite(pin, LOW);
}