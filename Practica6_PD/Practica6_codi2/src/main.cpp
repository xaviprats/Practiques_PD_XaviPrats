#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9    // GPIO 9 per Reset
#define SS_PIN 10    // GPIO 10 per Chip Select (SDA)

MFRC522 mfrc522(SS_PIN, RST_PIN); // Objecte per controlar el RC522

int lecturaCount = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  SPI.begin(12, 13, 11, 10); // SCK, MISO, MOSI, SS
  mfrc522.PCD_Init();        // Inicialitzem el lector RFID

  // Missatge d'inici personalitzat
  Serial.println("╔══════════════════════════════════╗");
  Serial.println("║     📡 SISTEMA DE LECTURA RFID   ║");
  Serial.println("║     ESP32-S3 + RC522 + SPI       ║");
  Serial.println("╚══════════════════════════════════╝");
  Serial.println("Apropa una targeta RFID per llegir el seu UID...\n");
}

void loop() {
  // Mirem si hi ha una targeta nova
  if (mfrc522.PICC_IsNewCardPresent()) {
    // Intentem llegir-la
    if (mfrc522.PICC_ReadCardSerial()) {
      lecturaCount++;

      Serial.println("----------------------------------------");
      Serial.print("🔁 Lectura nº ");
      Serial.println(lecturaCount);

      Serial.print("🆔 UID de la targeta: ");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) Serial.print("0");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      Serial.println("✅ Lectura completada amb èxit!");
      Serial.println("----------------------------------------\n");

      mfrc522.PICC_HaltA(); // Aturem comunicació amb la targeta
    }
  }
}