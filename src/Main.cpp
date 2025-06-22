#include <WiFi.h>
#include <WebServer.h>
#include "WebRoutes.h"
#include "ConfigManager.h"
#include "StateManager.h"

StateManager stateManager; // <-- Esta línea crea la instancia global

const char *ssid = "ESP32-LED-AP";
const char *password = "12345678";

// Puerto UDP por defecto para slaves
const uint16_t SLAVE_UDP_PORT = 4210;

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

    WiFi.softAP(ssid, password);
    Serial.println("Access Point iniciado");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    registerWebRoutes(server);
    server.begin();
    Serial.println("Servidor HTTP iniciado");
}

void loop()
{
    server.handleClient();
}