#include <Arduino.h>
#include "audio.h"
#include "SD.h"
#include "FS.h"

// 🎵 Connexió a la SD (pins del teu esquema)
#define SD_CS       10
#define SPI_MOSI    11
#define SPI_MISO    13
#define SPI_SCK     12

// 🔊 Connexió al MAX98357A
#define I2S_DOUT    6
#define I2S_BCLK    9
#define I2S_LRC     7

Audio audio;

void setup() {
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  Serial.begin(115200);
  delay(1000);

  Serial.println("🎧 ESP32-S3 preparat per fer festa 🎉");
  Serial.println("📀 Comprovant la targeta SD...");

  if (!SD.begin(SD_CS)) {
    Serial.println("❌ Error muntant la targeta SD!");
    return;
  }

  Serial.println("✅ Targeta SD detectada amb èxit!");
  Serial.println("🎶 Reproduint 'patito_juan_short.wav'");

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(21);  // Volum al màxim (0 - 21)
  audio.connecttoFS(SD, "/patito_juan_short.wav");
}

void loop() {
  audio.loop();
}