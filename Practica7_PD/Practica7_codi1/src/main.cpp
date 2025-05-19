#include <Arduino.h>
#include "driver/i2s.h"
#include <math.h>

#define I2S_NUM         I2S_NUM_0
#define SAMPLE_RATE     44100
#define I2S_BCLK        9         // GPIO per BCLK
#define I2S_LRC         7         // GPIO per LRC
#define I2S_DOUT        6         // GPIO per DIN
#define VOLUME_GAIN     1.5       // Volum multiplicat

void setupI2S() {
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM, &pin_config);
  i2s_zero_dma_buffer(I2S_NUM);
}

void playTone(float freq, int duration_ms) {
  int samples = SAMPLE_RATE * duration_ms / 1000;
  size_t bytes_written;
  for (int i = 0; i < samples; ++i) {
    float sample = sin(2.0 * PI * freq * i / SAMPLE_RATE);
    int16_t val = (int16_t)(sample * 32767 * VOLUME_GAIN);
    i2s_write(I2S_NUM, &val, sizeof(val), &bytes_written, portMAX_DELAY);
  }
}

void playMelody() {
  // 🎼 Aquesta seria una mini "cançó"
  float melody[] = {
    440.00, 440.00, 392.00, 440.00, 349.23, 392.00, // LA LA SOL LA FA SOL
    293.66, 349.23, 329.63, 261.63                 // RE FA MI DO
  };

  int durations[] = {
    300, 300, 300, 300, 300, 500,
    300, 300, 300, 500
  };

  int length = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < length; i++) {
    Serial.printf("🎵 Nota %.2f Hz (%d ms)\n", melody[i], durations[i]);
    playTone(melody[i], durations[i]);
    delay(50);
  }

  Serial.println("🔁 Repetint melodia...\n");
}

void setup() {
  Serial.begin(115200);
  setupI2S();
  delay(500);
  Serial.println("🎶 Iniciant melodia en bucle amb volum x1.5");
}

void loop() {
  playMelody();
}