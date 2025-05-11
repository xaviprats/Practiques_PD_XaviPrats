#include <Arduino.h>


const int ledPin = 2;
SemaphoreHandle_t semaphore;


void task1(void *parameter) {
  for (;;) {
    xSemaphoreTake(semaphore, portMAX_DELAY);
    digitalWrite(ledPin, HIGH);
    Serial.println("Tarea 1: LED Encendido");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    xSemaphoreGive(semaphore);
    vTaskDelay(10);
  }
}


void task2(void *parameter) {
  for (;;) {
    xSemaphoreTake(semaphore, portMAX_DELAY);
    digitalWrite(ledPin, LOW);
    Serial.println("Tarea 2: LED Apagado");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    xSemaphoreGive(semaphore);
    vTaskDelay(10);
  }
}


void task3(void *parameter) {
  for (;;) {
    xSemaphoreTake(semaphore, portMAX_DELAY);
    Serial.println("Tarea 3: Esperando...");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    xSemaphoreGive(semaphore);
    vTaskDelay(10);
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);


  semaphore = xSemaphoreCreateCounting(1, 1);


  xTaskCreate(task1, "Tarea 1", 2048, NULL, 1, NULL);
  xTaskCreate(task2, "Tarea 2", 2048, NULL, 1, NULL);
  xTaskCreate(task3, "Tarea 3", 2048, NULL, 1, NULL);
}


void loop() {
}
