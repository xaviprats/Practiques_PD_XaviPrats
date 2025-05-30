#include <Arduino.h>

#define RXD2 16
#define TXD2 17

String bufferUART0 = "";
String bufferUART2 = "";

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Inici del bucle UART2...");
}

void loop() {
  // Recollim l'entrada del terminal
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      Serial2.println(bufferUART0);  // envia amb \n
      bufferUART0 = "";
    } else if (c != '\r') {
      bufferUART0 += c;
    }
  }

  // Recollim el que retorna UART2
  while (Serial2.available()) {
    char c = Serial2.read();
    if (c == '\n') {
      Serial.println(bufferUART2);
      bufferUART2 = "";
    } else if (c >= 32 && c <= 126) {
      bufferUART2 += c;
    }
  }
}
