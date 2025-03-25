#include <Arduino.h>


#define LED_BUILTIN 2
#define DELAY 500


void setup() {
pinMode(LED_BUILTIN, OUTPUT);
Serial.begin(115200);
}
void loop() {
digitalWrite(LED_BUILTIN, HIGH);
delay(DELAY);
digitalWrite(LED_BUILTIN, LOW);
Serial.println("ON");
delay(DELAY);
Serial.println("OFF");
}
