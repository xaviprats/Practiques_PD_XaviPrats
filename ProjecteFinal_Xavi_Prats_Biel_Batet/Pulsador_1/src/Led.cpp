#include "Led.h"
#include <Arduino.h>

Led::Led(int pin) : pin(pin) {}

void Led::init() {
    pinMode(pin, OUTPUT);
    apagar();
}

void Led::encendre() {
    digitalWrite(pin, HIGH);
}

void Led::apagar() {
    digitalWrite(pin, LOW);
}