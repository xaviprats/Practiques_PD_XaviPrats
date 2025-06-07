#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
private:
    int pin;

public:
    Buzzer(int pin);
    void init();
    void sonar(unsigned int durada = 500); // durada en mil·lisegons
};

#endif