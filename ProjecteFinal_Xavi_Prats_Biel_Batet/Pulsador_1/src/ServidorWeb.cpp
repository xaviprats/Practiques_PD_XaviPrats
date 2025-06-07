#include "ServidorWeb.h"

ServidorWeb::ServidorWeb() : server(80) {
    impactesCount = 0;
    historicsCount = 0;
}

void ServidorWeb::begin() {
    Serial.print("Connectant a WiFi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("WiFi connectada. IP: ");
    Serial.println(WiFi.localIP());

    server.on("/", std::bind(&ServidorWeb::handleRoot, this));
    server.on("/toggleTots", std::bind(&ServidorWeb::handleToggleTots, this));
    server.on("/reiniciar", std::bind(&ServidorWeb::handleReiniciar, this));
    server.on("/esborrar", std::bind(&ServidorWeb::handleEsborrar, this));
    server.on("/impactes.json", std::bind(&ServidorWeb::handleImpactesJSON, this));
    server.on("/impactes_taula", std::bind(&ServidorWeb::handleImpactesTaula, this));
    server.onNotFound(std::bind(&ServidorWeb::handleNotFound, this));

    server.begin();
    Serial.println("Servidor HTTP iniciat");
}

void ServidorWeb::loop() {
    server.handleClient();
}

void ServidorWeb::setLedVerd(bool estat) { ledVerdEstat = estat; }
void ServidorWeb::setLedVermell(bool estat) { ledVermellEstat = estat; }
void ServidorWeb::setPantalla(bool estat) { pantallaEstat = estat; }
void ServidorWeb::setBuzzer(bool estat) { buzzerEstat = estat; }

bool ServidorWeb::getLedVerd() { return ledVerdEstat; }
bool ServidorWeb::getLedVermell() { return ledVermellEstat; }
bool ServidorWeb::getPantalla() { return pantallaEstat; }
bool ServidorWeb::getBuzzer() { return buzzerEstat; }

void ServidorWeb::afegirImpacte(String impacte) {
    if (impactesCount < 20) {
        impactes[impactesCount++] = impacte;
    } else {
        for (int i = 1; i < 20; i++) impactes[i - 1] = impactes[i];
        impactes[19] = impacte;
    }
}

void ServidorWeb::registrarImpacte(String hora, float intensitat) {
    String registre = "{\"hora\":\"" + hora + "\",\"intensitat\":" + String(intensitat, 2) + "}";
    if (historicsCount < 100) {
        historicsJSON[historicsCount++] = registre;
    } else {
        for (int i = 1; i < 100; i++) historicsJSON[i - 1] = historicsJSON[i];
        historicsJSON[99] = registre;
    }
}

String ServidorWeb::generarHTML() {
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Monitor Impactes ESP32-S3</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@400;700&display=swap" rel="stylesheet">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        body {
            font-family: 'Roboto', sans-serif;
            background: #e0f7fa;
            margin: 0;
            padding: 20px;
        }
        h1 {
            text-align: center;
            color: #006064;
        }
        .container {
            max-width: 900px;
            margin: auto;
            background: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 4px 10px rgba(0,0,0,0.1);
        }
        button {
            font-size: 1em;
            padding: 10px 20px;
            margin: 5px;
            border: none;
            border-radius: 5px;
            background-color: #0097a7;
            color: white;
            cursor: pointer;
            transition: background-color 0.3s;
        }
        button:hover {
            background-color: #006064;
        }
        table {
            border-collapse: collapse;
            width: 100%;
            margin-top: 20px;
        }
        th, td {
            border: 1px solid #ccc;
            padding: 10px;
            text-align: center;
        }
        th {
            background: #0097a7;
            color: white;
        }
        .controls p {
            margin: 10px 0;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Monitor Impactes ESP32-S3</h1>

        <div class="controls">
            <p><strong>Estat general:</strong></p>
            <p>
                <button onclick="toggle('/toggleTots')">🔀 Canviar tot</button>
            </p>
            <p>
                <button onclick="toggle('/reiniciar')">🔄 Reiniciar</button>
                <button onclick="toggle('/esborrar')">🗑️ Esborrar registres</button>
            </p>
        </div>

        <h2>📋 Últims registres d'impactes</h2>
        <table>
            <tr><th>#</th><th>Timestamp</th></tr>
            <tbody id="taulaImpactes">
                %TAULA_IMPACTES%
            </tbody>
        </table>

        <h2>📈 Gràfica d'impactes</h2>
        <canvas id='impactesChart' width='100%' height='40'></canvas>
    </div>

    <script>
        function toggle(url) {
            fetch(url).then(() => location.reload());
        }

        // Actualitzar taula cada 3 segons
        setInterval(() => {
            fetch('/impactes_taula')
                .then(r => r.text())
                .then(html => {
                    document.getElementById('taulaImpactes').innerHTML = html;
                });
        }, 3000);

        // Gràfica dinàmica amb actualització automàtica
        const ctx = document.getElementById('impactesChart').getContext('2d');

        const impactesChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [{
                    label: 'Intensitat (g)',
                    data: [],
                    borderColor: '#006064',
                    backgroundColor: 'rgba(0,150,136,0.1)',
                    fill: true
                }]
            },
            options: {
                responsive: true,
                scales: {
                    y: {
                        beginAtZero: true
                    }
                }
            }
        });

        function actualitzarGrafica() {
            fetch('/impactes.json')
                .then(r => r.json())
                .then(data => {
                    impactesChart.data.labels = data.map(e => e.hora);
                    impactesChart.data.datasets[0].data = data.map(e => e.intensitat);
                    impactesChart.update();
                });
        }

        setInterval(actualitzarGrafica, 3000); // cada 3 segons
        actualitzarGrafica(); // inicial
    </script>
</body>
</html>
)rawliteral";

    String files = "";
    for (int i = 0; i < impactesCount; i++) {
        files += "<tr><td>" + String(i + 1) + "</td><td>" + impactes[i] + "</td></tr>";
    }
    html.replace("%TAULA_IMPACTES%", files);

    return html;
}


void ServidorWeb::handleImpactesJSON() {
    String json = "[";
    for (int i = 0; i < historicsCount; i++) {
        json += historicsJSON[i];
        if (i < historicsCount - 1) json += ",";
    }
    json += "]";
    server.send(200, "application/json", json);
}

void ServidorWeb::handleImpactesTaula() {
    String taula = "";
    for (int i = 0; i < impactesCount; i++) {
        taula += "<tr><td>" + String(i + 1) + "</td><td>" + impactes[i] + "</td></tr>";
    }
    server.send(200, "text/html", taula);
}

void ServidorWeb::handleRoot() {
    server.send(200, "text/html", generarHTML());
}

void ServidorWeb::handleToggleTots() {
    ledVerdEstat = !ledVerdEstat;
    ledVermellEstat = !ledVermellEstat;
    pantallaEstat = !pantallaEstat;
    buzzerEstat = !buzzerEstat;
    server.sendHeader("Location", "/");
    server.send(303);
}

void ServidorWeb::handleReiniciar() {
    server.send(200, "text/html", "<h1>Dispositiu reiniciant...</h1>");
    delay(1000);
    ESP.restart();
}

void ServidorWeb::handleEsborrar() {
    impactesCount = 0;
    historicsCount = 0;
    server.sendHeader("Location", "/");
    server.send(303);
}

void ServidorWeb::handleNotFound() {
    server.send(404, "text/plain", "404: No trobat");
}
