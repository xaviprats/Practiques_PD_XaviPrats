#include <Arduino.h>


const int ledPin = 2; // GPIO2
SemaphoreHandle_t semaphore;


void taskEncender(void *parameter) {
  for (;;) {
    xSemaphoreTake(semaphore, portMAX_DELAY);
    digitalWrite(ledPin, HIGH);
    Serial.println("LED Encendido");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    xSemaphoreGive(semaphore);
    vTaskDelay(10); // petita espera per assegurar l'alternança
  }
}


void taskApagarLED(void *parameter) {
  for (;;) {
    if(xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE) {
      digitalWrite(ledPin, LOW);
      Serial.println("LED Apagado");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      xSemaphoreGive(semaphore);
      vTaskDelay(10); // petita espera per assegurar l'alternança
    }
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);


  semaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(semaphore); // inicialment disponible


  xTaskCreate(taskEncender, "Tarea Enciende LED", 2048, NULL, 1, NULL);
  xTaskCreate(taskApagarLED, "Tarea Apaga LED", 2048, NULL, 1, NULL);
}


void loop() {
  // loop buit
}
