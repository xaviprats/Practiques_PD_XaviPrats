#include "Accelerometre.h"

Accelerometre::Accelerometre(float llindar)
    : llindarImpacte(llindar), impacteDetectat(false), ultimaIntensitat(0) {}

bool Accelerometre::init() {
    if (!mpu.begin()) {
        return false;
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    return true;
}

bool Accelerometre::hiHaImpacte() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    float magnitud = sqrt(a.acceleration.x * a.acceleration.x +
                          a.acceleration.y * a.acceleration.y +
                          a.acceleration.z * a.acceleration.z);

    float delta = abs(magnitud - 9.81);  // comparat amb gravetat terrestre

    if (delta >= llindarImpacte) {
        impacteDetectat = true;
        ultimaIntensitat = delta;
        return true;
    } else {
        impacteDetectat = false;
        return false;
    }
}

float Accelerometre::getIntensitatImpacte() {
    return ultimaIntensitat;
}