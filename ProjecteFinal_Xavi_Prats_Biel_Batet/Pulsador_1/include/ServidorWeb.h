#ifndef SERVIDORWEB_H
#define SERVIDORWEB_H

#include <WiFi.h>
#include <WebServer.h>

class ServidorWeb {
private:
    const char* ssid = "iPhone";
    const char* password = "batet2005";

    WebServer server;

    bool ledVerdEstat = false;
    bool ledVermellEstat = true;
    bool pantallaEstat = false;
    bool buzzerEstat = false;

    // Per la taula d’impactes
    String impactes[20];
    int impactesCount = 0;

    // Per la gràfica d’impactes
    String historicsJSON[100];
    int historicsCount = 0;

    void handleRoot();
    void handleNotFound();
    void handleToggleLedVerd();
    void handleToggleLedVermell();
    void handleTogglePantalla();
    void handleToggleBuzzer();
    void handleReiniciar();
    void handleEsborrar();
    void handleImpactesJSON();     // Gràfica
    void handleImpactesTaula();    // 🔁 Taula en temps real
    void handleToggleTots();       // Botó "Canviar tot"

    String generarHTML();

public:
    ServidorWeb();

    void begin();
    void loop();

    void setLedVerd(bool estat);
    void setLedVermell(bool estat);
    void setPantalla(bool estat);
    void setBuzzer(bool estat);

    bool getLedVerd();
    bool getLedVermell();
    bool getPantalla();
    bool getBuzzer();

    void afegirImpacte(String impacte);                // Per la taula
    void registrarImpacte(String hora, float intensitat); // Per la gràfica
};

#endif
