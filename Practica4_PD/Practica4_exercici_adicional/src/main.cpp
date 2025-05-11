#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"


#define LED_SEGUNDOS 2
#define LED_MODO 4
#define BTN_MODO 16
#define BTN_INCREMENTO 17
#define LED_ALARMA 5


volatile int horas = 0;
volatile int minutos = 0;
volatile int segundos = 0;
volatile int modo = 0; // 0: Normal, 1: Ajuste Horas, 2: Ajuste Minutos, 3: Ajuste Alarma
volatile int alarmaHoras = 0;
volatile int alarmaMinutos = 0;


QueueHandle_t botonQueue;
SemaphoreHandle_t relojMutex;


typedef struct {
  uint8_t boton;
  uint32_t tiempo;
} EventoBoton;


void IRAM_ATTR ISR_Boton(void *arg) {
  EventoBoton evento;
  evento.boton = (uint32_t)arg;
  evento.tiempo = xTaskGetTickCountFromISR();
  xQueueSendFromISR(botonQueue, &evento, NULL);
}


void TareaReloj(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(1000);


  for (;;) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);


    if (xSemaphoreTake(relojMutex, portMAX_DELAY)) {
      if (modo == 0) {
        segundos++;
        if (segundos >= 60) {
          segundos = 0;
          minutos++;
          if (minutos >= 60) {
            minutos = 0;
            horas = (horas + 1) % 24;
          }
        }
      }
      xSemaphoreGive(relojMutex);
    }
  }
}


void TareaLecturaBotones(void *pvParameters) {
  EventoBoton evento;
  uint32_t ultimoTiempoBoton = 0;
  const uint32_t debounceTime = pdMS_TO_TICKS(300);


  for (;;) {
    if (xQueueReceive(botonQueue, &evento, portMAX_DELAY) == pdPASS) {
      if ((evento.tiempo - ultimoTiempoBoton) >= debounceTime) {
        if (xSemaphoreTake(relojMutex, portMAX_DELAY)) {
          if (evento.boton == BTN_MODO) {
            modo = (modo + 1) % 4;
          } else if (evento.boton == BTN_INCREMENTO) {
            if (modo == 1) horas = (horas + 1) % 24;
            else if (modo == 2) minutos = (minutos + 1) % 60;
            else if (modo == 3) alarmaHoras = (alarmaHoras + 1) % 24;
          }
          xSemaphoreGive(relojMutex);
        }
        ultimoTiempoBoton = evento.tiempo;
      }
    }
  }
}


void TareaControlLEDs(void *pvParameters) {
  bool estadoLedSegundos = false;
  for (;;) {
    if (xSemaphoreTake(relojMutex, portMAX_DELAY)) {
      digitalWrite(LED_SEGUNDOS, segundos % 2);
      digitalWrite(LED_MODO, modo > 0);
      bool alarmaActiva = (horas == alarmaHoras && minutos == alarmaMinutos);
      digitalWrite(LED_ALARMA, alarmaActiva);
      xSemaphoreGive(relojMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(LED_SEGUNDOS, OUTPUT);
  pinMode(LED_MODO, OUTPUT);
  pinMode(LED_ALARMA, OUTPUT);
  pinMode(BTN_MODO, INPUT_PULLUP);
  pinMode(BTN_INCREMENTO, INPUT_PULLUP);


  botonQueue = xQueueCreate(10, sizeof(EventoBoton));
  relojMutex = xSemaphoreCreateMutex();


  attachInterruptArg(BTN_MODO, ISR_Boton, (void *)BTN_MODO, FALLING);
  attachInterruptArg(BTN_INCREMENTO, ISR_Boton, (void *)BTN_INCREMENTO, FALLING);


  xTaskCreate(TareaReloj, "RelojTask", 2048, NULL, 1, NULL);
  xTaskCreate(TareaLecturaBotones, "BotonesTask", 2048, NULL, 2, NULL);
  xTaskCreate(TareaControlLEDs, "LEDsTask", 2048, NULL, 1, NULL);
}


void loop() {
  vTaskDelay(portMAX_DELAY);
}

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"


#define LED_SEGUNDOS 2
#define LED_MODO 4
#define BTN_MODO 16
#define BTN_INCREMENTO 17
#define LED_ALARMA 5


volatile int horas = 0;
volatile int minutos = 0;
volatile int segundos = 0;
volatile int modo = 0; // 0: Normal, 1: Ajuste Horas, 2: Ajuste Minutos, 3: Ajuste Alarma
volatile int alarmaHoras = 0;
volatile int alarmaMinutos = 0;


QueueHandle_t botonQueue;
SemaphoreHandle_t relojMutex;


typedef struct {
  uint8_t boton;
  uint32_t tiempo;
} EventoBoton;


void IRAM_ATTR ISR_Boton(void *arg) {
  EventoBoton evento;
  evento.boton = (uint32_t)arg;
  evento.tiempo = xTaskGetTickCountFromISR();
  xQueueSendFromISR(botonQueue, &evento, NULL);
}


void TareaReloj(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(1000);


  for (;;) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);


    if (xSemaphoreTake(relojMutex, portMAX_DELAY)) {
      if (modo == 0) {
        segundos++;
        if (segundos >= 60) {
          segundos = 0;
          minutos++;
          if (minutos >= 60) {
            minutos = 0;
            horas = (horas + 1) % 24;
          }
        }
      }
      xSemaphoreGive(relojMutex);
    }
  }
}


void TareaLecturaBotones(void *pvParameters) {
  EventoBoton evento;
  uint32_t ultimoTiempoBoton = 0;
  const uint32_t debounceTime = pdMS_TO_TICKS(300);


  for (;;) {
    if (xQueueReceive(botonQueue, &evento, portMAX_DELAY) == pdPASS) {
      if ((evento.tiempo - ultimoTiempoBoton) >= debounceTime) {
        if (xSemaphoreTake(relojMutex, portMAX_DELAY)) {
          if (evento.boton == BTN_MODO) {
            modo = (modo + 1) % 4;
          } else if (evento.boton == BTN_INCREMENTO) {
            if (modo == 1) horas = (horas + 1) % 24;
            else if (modo == 2) minutos = (minutos + 1) % 60;
            else if (modo == 3) alarmaHoras = (alarmaHoras + 1) % 24;
          }
          xSemaphoreGive(relojMutex);
        }
        ultimoTiempoBoton = evento.tiempo;
      }
    }
  }
}


void TareaControlLEDs(void *pvParameters) {
  bool estadoLedSegundos = false;
  for (;;) {
    if (xSemaphoreTake(relojMutex, portMAX_DELAY)) {
      digitalWrite(LED_SEGUNDOS, segundos % 2);
      digitalWrite(LED_MODO, modo > 0);
      bool alarmaActiva = (horas == alarmaHoras && minutos == alarmaMinutos);
      digitalWrite(LED_ALARMA, alarmaActiva);
      xSemaphoreGive(relojMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(LED_SEGUNDOS, OUTPUT);
  pinMode(LED_MODO, OUTPUT);
  pinMode(LED_ALARMA, OUTPUT);
  pinMode(BTN_MODO, INPUT_PULLUP);
  pinMode(BTN_INCREMENTO, INPUT_PULLUP);


  botonQueue = xQueueCreate(10, sizeof(EventoBoton));
  relojMutex = xSemaphoreCreateMutex();


  attachInterruptArg(BTN_MODO, ISR_Boton, (void *)BTN_MODO, FALLING);
  attachInterruptArg(BTN_INCREMENTO, ISR_Boton, (void *)BTN_INCREMENTO, FALLING);


  xTaskCreate(TareaReloj, "RelojTask", 2048, NULL, 1, NULL);
  xTaskCreate(TareaLecturaBotones, "BotonesTask", 2048, NULL, 2, NULL);
  xTaskCreate(TareaControlLEDs, "LEDsTask", 2048, NULL, 1, NULL);
}


void loop() {
  vTaskDelay(portMAX_DELAY);
}
