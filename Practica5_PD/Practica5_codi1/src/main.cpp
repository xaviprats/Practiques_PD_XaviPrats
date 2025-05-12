#include <Arduino.h>
#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 22

void scanI2CDevices() {
    byte error, address;
    int nDevices = 0;

    Serial.println("Scanning...");

    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");
            nDevices++;
        }
    }

    if (nDevices == 0) {
        Serial.println("No I2C devices found.\n");
    } else {
        Serial.println("done\n");
    }
}

void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN); // ESP32 default I2C pins: GPIO21 (SDA), GPIO22 (SCL)
}

void loop() {
    scanI2CDevices();
    delay(5000); // Wait 5 seconds before next scan
}