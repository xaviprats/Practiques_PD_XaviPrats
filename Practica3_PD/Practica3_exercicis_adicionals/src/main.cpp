#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


#define SERVICE_UUID            "4FAFC201-1FB5-459E-8FCC-C5C9C331914B"
#define TEMP_CHARACTERISTIC_UUID "BEB5483E-36E1-4688-B7F5-EA07361B26A8"
#define HUM_CHARACTERISTIC_UUID  "2A6E0A12-34BF-47FA-9F4B-9C82DD6968BF"
#define AUTH_CHARACTERISTIC_UUID "F8D4A6C3-6E2B-4BCB-B7EF-6F6E6BBD0217"
#define LED_GPIO                2


BLEServer* pServer = NULL;
BLECharacteristic* pTempCharacteristic = NULL;
BLECharacteristic* pHumCharacteristic = NULL;
BLECharacteristic* pAuthCharacteristic = NULL;


bool deviceConnected = false;
bool oldDeviceConnected = false;
bool isAuthenticated = false;


// 🔹 Callbacks para el servidor BLE
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        Serial.println("✅ Dispositivo conectado");
    }


    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        Serial.println("❌ Dispositivo desconectado");
    }
};


// 🔹 Callbacks para recibir datos del cliente BLE
class CharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();


        Serial.println("\n📩 ----> onWrite llamado <----");
        if (value.length() > 0) {
            Serial.print("📥 Datos recibidos: ");
            for (int i = 0; i < value.length(); i++) {
                Serial.print(value[i]);
            }
            Serial.println();


            // 🔑 Verificar autenticación
            if (value == "1234") {
                isAuthenticated = true;
                Serial.println("🔑 Autenticación exitosa");
            } else if (value == "ON" && isAuthenticated) {
                digitalWrite(LED_GPIO, HIGH);
                Serial.println("💡 LED encendido");
            } else if (value == "OFF" && isAuthenticated) {
                digitalWrite(LED_GPIO, LOW);
                Serial.println("💡 LED apagado");
            } else {
                Serial.println("🚫 Acceso denegado: Necesita autenticación");
            }
        }
    }
};


void setup() {
    Serial.begin(115200);
    Serial.println("🚀 Iniciando aplicación BLE...");


    pinMode(LED_GPIO, OUTPUT);
    digitalWrite(LED_GPIO, LOW);


    BLEDevice::init("ESP32-S3 BLE Sensor");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());


    BLEService *pService = pServer->createService(SERVICE_UUID);


    pTempCharacteristic = pService->createCharacteristic(
        TEMP_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );


    pHumCharacteristic = pService->createCharacteristic(
        HUM_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );


    pAuthCharacteristic = pService->createCharacteristic(
        AUTH_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );


    pTempCharacteristic->addDescriptor(new BLE2902());
    pHumCharacteristic->addDescriptor(new BLE2902());
    pAuthCharacteristic->setCallbacks(new CharacteristicCallbacks());


    pService->start();


    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();


    Serial.println("✅ Servicio BLE iniciado. Esperando conexión...");
}


void loop() {
    delay(2000);  // Simulación cada 2 segundos


    // 🔹 Simulación de temperatura (20.00 - 30.00 °C)
    float temperature = random(2000, 3000) / 100.0;


    // 🔹 Simulación de humedad (40.00 - 60.00 %)
    float humidity = random(4000, 6000) / 100.0;


    Serial.print("🌡️ Temperatura simulada: ");
    Serial.println(temperature);
    Serial.print("💧 Humedad simulada: ");
    Serial.println(humidity);


    if (deviceConnected) {
        pTempCharacteristic->setValue(String(temperature).c_str());
        pTempCharacteristic->notify();
       
        pHumCharacteristic->setValue(String(humidity).c_str());
        pHumCharacteristic->notify();
    }


    if (!deviceConnected && oldDeviceConnected) {
        delay(500);
        pServer->startAdvertising();
        Serial.println("🔄 Reiniciando publicidad BLE...");
        oldDeviceConnected = deviceConnected;
    }


    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }
}

