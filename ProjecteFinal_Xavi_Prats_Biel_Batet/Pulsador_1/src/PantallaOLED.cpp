#include "PantallaOLED.h"

PantallaOLED::PantallaOLED(uint8_t amplada, uint8_t alcada, int8_t sda, int8_t scl)
    : display(amplada, alcada, &Wire, -1), activada(false) {}

void PantallaOLED::init() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("No s'ha trobat la pantalla OLED!"));
        return;
    }
    display.clearDisplay();
    display.display();
}

void PantallaOLED::mostrarText() {
    if (!activada) {
        display.clearDisplay();
        display.display();
        return;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Monitor d'Impactes");
    display.setCursor(0, 20);
    display.println("- ESP32-S3 -");
    display.display();
}

void PantallaOLED::mostrarMissatge(const String& missatge) {
    if (!activada) return;

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.println(missatge);
    display.display();
}

void PantallaOLED::setActivada(bool estat) {
    activada = estat;
}

bool PantallaOLED::estaActivada() {
    return activada;
}

void PantallaOLED::animacioImpacte() {
    if (!activada) return;

    for (int i = 0; i < 3; i++) {
        display.clearDisplay();
        display.setTextColor(BLACK, WHITE); // negatiu
        display.setCursor(10, 25);
        display.setTextSize(2);
        display.print("IMPACTE!");
        display.display();
        delay(150);

        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setCursor(10, 25);
        display.setTextSize(2);
        display.print("IMPACTE!");
        display.display();
        delay(150);
    }
}