#include <WiFi.h>
#include <WebServer.h>
#include "WebRoutes.h"
#include "ConfigManager.h"
#include "StateManager.h"
#include <ESPmDNS.h>
#include "AppConfig.h"
#include "UdpSender.h"

StateManager stateManager;

const char *ssid = "ESP32-LED-AP";
const char *password = "12345678";
unsigned long lastWatchdog = 0;

// Puerto UDP por defecto para slaves
// const uint16_t SLAVE_UDP_PORT = 4210;

WebServer server(80);
// Instancia global real:
ConfigManager configManager;

void setup()
{
    Serial.begin(115200);

    // Inicializa el almacenamiento de configuración
    configManager.begin();

    // Configura IP fija para el Access Point
    IPAddress apIP(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(apIP, gateway, subnet);

    // WiFi.softAP(ssid, password);
    WiFi.softAP(DEFAULT_SSID, DEFAULT_PASSWORD);
    Serial.println("Access Point iniciado");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    if (MDNS.begin(DEFAULT_MDNS_NAME))
    { // Cambia "maestroled" por el nombre que prefieras
        Serial.println("mDNS responder iniciado: http://iluminate.local");
    }
    else
    {
        Serial.println("Error iniciando mDNS");
    }

    registerWebRoutes(server);
    server.begin();
    Serial.println("Servidor HTTP iniciado");
}

void loop()
{
    server.handleClient();

    // Enviar watchdog UDP cada segundo
    unsigned long now = millis();
    if (now - lastWatchdog > WATCHDOG_INTERVAL)
    {
        // sendWatchdogUDP();
        lastWatchdog = now;
    }
}