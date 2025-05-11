#include <Arduino.h>


// Función de la tarea adicional
void anotherTask(void * parameter) {
  for(;;) {
    Serial.println("this is another Task");
    delay(1000);
  }
  vTaskDelete(NULL);
}


void setup() {
  Serial.begin(115200);


  // Crear una nueva tarea
  xTaskCreate(
    anotherTask,    // Función de la tarea
    "another Task", // Nombre de la tarea
    10000,         // Tamaño de la pila
    NULL,          // Parámetro de la tarea
    1,             // Prioridad
    NULL           // Task handle
  );
}


void loop() {
  Serial.println("this is ESP32 Task");
  delay(1000);
}

