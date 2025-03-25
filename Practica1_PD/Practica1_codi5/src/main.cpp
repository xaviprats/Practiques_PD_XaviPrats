#include <Arduino.h>


#define LED_PIN 2  // Cambia este pin por uno disponible en tu ESP32-S3


void setup() {
   pinMode(LED_PIN, OUTPUT);
   Serial.begin(115200);
}


void loop() {
   digitalWrite(LED_PIN, HIGH);
   digitalWrite(LED_PIN, LOW);
}
