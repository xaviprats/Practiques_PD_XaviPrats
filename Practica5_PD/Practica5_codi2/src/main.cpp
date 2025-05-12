#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definir la mida de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Direcció I2C confirmada de la pantalla OLED
#define SCREEN_ADDRESS 0x3C 

// Definir els pins I2C (ESP32-S3 per defecte: GPIO 8 = SDA, GPIO 9 = SCL)
#define SDA_PIN 8
#define SCL_PIN 9

// Crear l'objecte de la pantalla OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
    Serial.begin(115200);  // Inicialitza el monitor sèrie
    Wire.begin(SDA_PIN, SCL_PIN);  // Inicialitza el bus I2C amb els pins adequats

    // Comprovar si la pantalla OLED està connectada
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("Error: No s'ha detectat la pantalla OLED!"));
        for (;;);  // Es queda en bucle si no la detecta
    }

    Serial.println("Pantalla OLED detectada!");

    // Netejar la pantalla i escriure un missatge
    display.clearDisplay();
    display.setTextSize(2);  // Mida del text
    display.setTextColor(SSD1306_WHITE);  // Color blanc sobre fons negre
    display.setCursor(10, 10);  // Posició del text
    display.println("Hola Manu");
    display.display();  // Actualitza la pantalla
}

void loop() {
    // No fem res al loop, només es mostra el text estàtic
}