#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MAX30105.h>  // Llibreria correcta per MAX30102

// Definició de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C  // Direcció I2C de la pantalla OLED

// Pins I2C per ESP32-S3
#define SDA_PIN 8
#define SCL_PIN 9

// Inicialitzar objectes
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
MAX30105 particleSensor;

void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);

    // Inicialitzar pantalla OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("Error: No s'ha detectat la pantalla OLED!"));
        for (;;);
    }
    display.clearDisplay();

    // Inicialitzar sensor MAX30102
    if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
        Serial.println(F("Error: No s'ha detectat el sensor MAX30102!"));
        for (;;);
    }
    
    particleSensor.setup(50, 4, 2, 100, 411, 4096);
}

void loop() {
    uint32_t irValue = particleSensor.getIR();  // Llegir sensor
    uint32_t redValue = particleSensor.getRed();

    // Simular càlcul de SpO2 i ritme cardíac (valors de prova)
    int spo2 = random(95, 100);  // Percentatge d'oxigen en sang
    int heartRate = random(60, 100);  // BPM

    // Mostrar dades al monitor sèrie
    Serial.print("Freq. Cardíaca: "); Serial.print(heartRate);
    Serial.print(" BPM | Oxigen: "); Serial.print(spo2); Serial.println(" %");

    // Mostrar dades a la pantalla OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Freq. Cardíaca: ");
    display.print(heartRate);
    display.println(" BPM");

    display.print("Oxigen: ");
    display.print(spo2);
    display.println(" %");

    display.display();
    delay(1000);
}