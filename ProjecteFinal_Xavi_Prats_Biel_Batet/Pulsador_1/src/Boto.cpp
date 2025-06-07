#include "Boto.h"
#include <Arduino.h>

Boto::Boto(int pin) : pin(pin), estatAnterior(false) {}

void Boto::init() {
    pinMode(pin, INPUT_PULLUP);
}

bool Boto::sHaPremut() {
    bool estatActual = digitalRead(pin) == LOW;
    bool premut = !estatAnterior && estatActual;
    estatAnterior = estatActual;
    return premut;
}