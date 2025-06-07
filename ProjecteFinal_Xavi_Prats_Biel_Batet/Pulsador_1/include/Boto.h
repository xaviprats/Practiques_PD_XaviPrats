#ifndef BOTO_H
#define BOTO_H

class Boto {
private:
    int pin;
    bool estatAnterior;

public:
    Boto(int pin);
    void init();
    bool sHaPremut();
};

#endif