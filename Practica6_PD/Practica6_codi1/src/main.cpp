#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("📦 Iniciant targeta SD...");

  if (!SD.begin(10)) { // GPIO 10 com a CS
    Serial.println("❌ ERROR: No s'ha pogut inicialitzar la SD.");
    return;
  }

  Serial.println("✅ SD inicialitzada correctament!");

  myFile = SD.open("/archivo.txt");
  if (myFile) {
    Serial.println("📄 Contingut de archivo.txt:");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
    Serial.println("\n✅ Lectura completada.");
  } else {
    Serial.println("❌ ERROR: No s'ha pogut obrir el fitxer.");
  }
}

void loop() {
  // No fem res al bucle
}