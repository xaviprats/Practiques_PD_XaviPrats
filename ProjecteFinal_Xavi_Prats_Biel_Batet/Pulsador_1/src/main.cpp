#include <Arduino.h>
#include <Wire.h>
#include "Boto.h"
#include "PantallaOLED.h"
#include "Led.h"
#include "ServidorWeb.h"
#include "Accelerometre.h"
#include "Buzzer.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP_Mail_Client.h>

#define OLED_SDA 4
#define OLED_SCL 5
#define BUTTON_PIN 12
#define LED_VERD 13
#define LED_VERMELL 15
#define BUZZER_PIN 18

// ✅ Dades de correu configurades per tu
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define EMAIL_SENDER "bielbt2005@gmail.com"
#define EMAIL_PASSWORD "totzheqooxbdltpr"
#define EMAIL_RECIPIENT "biel.batet@estudiantat.upc.edu"

const float UMBRAL_IMPACTE_ALT = 10;

Boto boto(BUTTON_PIN);
PantallaOLED pantalla(128, 64, OLED_SDA, OLED_SCL);
Led ledVerd(LED_VERD);
Led ledVermell(LED_VERMELL);
Buzzer buzzer(BUZZER_PIN);
ServidorWeb servidor;
Accelerometre accelerometre(1.5);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7200, 60000);

unsigned long tempsUltimImpacte = 0;
const unsigned long DURADA_MISSATGE = 2000;
unsigned long ignorarImpactesFins = 0;
bool mostrantImpacte = false;
bool ledVerdEstat = false;
bool ledVermellEstat = true;

ESP_Mail_Session mailSession;
SMTP_Message message;
SMTPSession smtp;

void enviarCorreu(float intensitat, String hora) {
    smtp.debug(1);

    mailSession.server.host_name = SMTP_HOST;
    mailSession.server.port = SMTP_PORT;
    mailSession.login.email = EMAIL_SENDER;
    mailSession.login.password = EMAIL_PASSWORD;

    message.sender.name = "Sensor Impactes";
    message.sender.email = EMAIL_SENDER;
    message.subject = "‼️ ALERTA: Impacte fort detectat";

    String cos = "S'ha detectat un impacte de " + String(intensitat, 2) + " g a les " + hora;
    message.text.content = cos.c_str();
    message.text.charSet = "utf-8";
    message.addRecipient("Biel Batet", EMAIL_RECIPIENT);

    if (!smtp.connect(&mailSession)) {
        Serial.println("❌ Error de connexió SMTP");
        return;
    }

    if (!MailClient.sendMail(&smtp, &message)) {
        Serial.println("❌ Error enviant correu.");
    } else {
        Serial.println("✅ Correu enviat!");
    }

    message.clear();
}

void setup() {
    Serial.begin(115200);
    Wire.begin(OLED_SDA, OLED_SCL);

    boto.init();
    pantalla.init();
    ledVerd.init();
    ledVermell.init();
    buzzer.init();

    pantalla.setActivada(false);
    pantalla.mostrarText();
    ledVerd.apagar();
    ledVermell.encendre();

    servidor.begin();
    timeClient.begin();

    if (!accelerometre.init()) {
        Serial.println("Sistema en marxa!");
    }

    servidor.setLedVerd(ledVerdEstat);
    servidor.setLedVermell(ledVermellEstat);
    servidor.setPantalla(false);
}

void loop() {
    servidor.loop();
    timeClient.update();

    bool nouEstatLedVerd = servidor.getLedVerd();
    bool nouEstatLedVermell = servidor.getLedVermell();
    bool nouEstatPantalla = servidor.getPantalla();
    bool nouEstatBuzzer = servidor.getBuzzer();

    if (nouEstatLedVerd != ledVerdEstat) {
        nouEstatLedVerd ? ledVerd.encendre() : ledVerd.apagar();
        ledVerdEstat = nouEstatLedVerd;
    }

    if (nouEstatLedVermell != ledVermellEstat) {
        nouEstatLedVermell ? ledVermell.encendre() : ledVermell.apagar();
        ledVermellEstat = nouEstatLedVermell;
    }

    if (nouEstatPantalla != pantalla.estaActivada()) {
        pantalla.setActivada(nouEstatPantalla);
        pantalla.mostrarText();
        ignorarImpactesFins = millis() + 1000;
    }

    if (nouEstatBuzzer) {
        buzzer.sonar(150);
        delay(100);
        buzzer.sonar(150);
        servidor.setBuzzer(false);
    }

    servidor.setLedVerd(ledVerdEstat);
    servidor.setLedVermell(ledVermellEstat);
    servidor.setPantalla(pantalla.estaActivada());

    if (boto.sHaPremut()) {
        bool nouEstat = !pantalla.estaActivada();
        pantalla.setActivada(nouEstat);

        if (nouEstat) {
            ledVerd.encendre();
            ledVermell.apagar();
            ledVerdEstat = true;
            ledVermellEstat = false;
        } else {
            ledVerd.apagar();
            ledVermell.encendre();
            ledVerdEstat = false;
            ledVermellEstat = true;
        }

        pantalla.mostrarText();
        ignorarImpactesFins = millis() + 1000;

        servidor.setLedVerd(ledVerdEstat);
        servidor.setLedVermell(ledVermellEstat);
        servidor.setPantalla(pantalla.estaActivada());

        Serial.print("Pantalla ");
        Serial.println(nouEstat ? "ENCESA" : "APAGADA");
    }

    if (pantalla.estaActivada() && millis() > ignorarImpactesFins && accelerometre.hiHaImpacte()) {
        Serial.println("‼️ IMPACTE DETECTAT!");
        pantalla.animacioImpacte();
        buzzer.sonar(500);

        String horaActual = timeClient.getFormattedTime();
        float intensitat = accelerometre.getIntensitatImpacte();
        String impacte = horaActual + " - " + String(intensitat, 2) + " g";

        servidor.afegirImpacte(impacte);
        servidor.registrarImpacte(horaActual, intensitat);

        Serial.println("Impacte a les " + horaActual + " amb intensitat: " + String(intensitat, 2) + " g");

        if (intensitat > UMBRAL_IMPACTE_ALT) {
            enviarCorreu(intensitat, horaActual);
        }

        tempsUltimImpacte = millis();
        mostrantImpacte = true;
    }

    if (mostrantImpacte && millis() - tempsUltimImpacte > DURADA_MISSATGE) {
        pantalla.mostrarText();
        mostrantImpacte = false;
    }

    delay(50);
}