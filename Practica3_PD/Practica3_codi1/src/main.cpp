#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>


void handle_root();


// SSID & Password
const char* ssid = "iPhone"; // Enter your SSID here
const char* password = "batet2005"; // Enter your Password here


WebServer server(80); // Object of WebServer(HTTP port, 80 is default)


void setup() {
    Serial.begin(115200);
    Serial.println("Try Connecting to ");
    Serial.println(ssid);


    // Connect to your Wi-Fi network
    WiFi.begin(ssid, password);


    // Check if connected to Wi-Fi network
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }


    Serial.println("");
    Serial.println("WiFi connected successfully");
    Serial.print("Got IP: ");
    Serial.println(WiFi.localIP()); // Show ESP32 IP on serial


    server.on("/", handle_root);
    server.begin();
    Serial.println("HTTP server started");
    delay(100);
}


void loop() {



    server.handleClient();
}


// HTML & CSS content
const char HTML[] = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
    <title>ESP32 Web Server</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial, sans-serif; text-align: center; margin: 50px; }
        h1 { color: #333; }
        button { padding: 10px 20px; font-size: 16px; margin-top: 20px; cursor: pointer; }
    </style>
</head>
<body>
    <h1>Mi Página con ESP32 - Station Mode 😊</h1>
    <p>Bienvenido a mi servidor web alojado en un ESP32.</p>
    <button onclick='alert("¡Botón presionado!")'>Presiona aquí</button>
</body>
</html>
)rawliteral";


// Handle root URL (/)
void handle_root() {
    server.send(200, "text/html", HTML);
}
