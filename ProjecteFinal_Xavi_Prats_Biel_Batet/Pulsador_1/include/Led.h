#ifndef LED_H
#define LED_H

class Led {
private:
    int pin;

public:
    Led(int pin);
    void init();
    void encendre();
    void apagar();
};

#endif
