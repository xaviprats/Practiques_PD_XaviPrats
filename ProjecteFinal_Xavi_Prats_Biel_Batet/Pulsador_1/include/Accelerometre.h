#ifndef ACCELEROMETRE_H
#define ACCELEROMETRE_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class Accelerometre {
private:
    Adafruit_MPU6050 mpu;
    float llindarImpacte;
    bool impacteDetectat;
    float ultimaIntensitat;

public:
    Accelerometre(float llindar);
    bool init();
    bool hiHaImpacte();
    float getIntensitatImpacte();
};

#endif