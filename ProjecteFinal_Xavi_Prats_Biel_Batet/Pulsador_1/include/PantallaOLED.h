#ifndef PANTALLAOLED_H
#define PANTALLAOLED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class PantallaOLED {
private:
    Adafruit_SSD1306 display;
    bool activada;

public:
    PantallaOLED(uint8_t amplada, uint8_t alcada, int8_t sda, int8_t scl);

    void init();
    void mostrarText();
    void mostrarMissatge(const String& missatge);
    void setActivada(bool estat);
    bool estaActivada();

    void animacioImpacte();  // ✅ Nou mètode per animació
};

#endif