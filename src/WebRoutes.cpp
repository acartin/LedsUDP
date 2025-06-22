#include "WebRoutes.h"
#include "WebHome.h"
#include "WebConfig.h"
#include "ConfigManager.h"
#include "ApiHandlers.h"

// Declara tu instancia global de ConfigManager (debe estar en Main.cpp y marcada como extern aquí si es global)
extern ConfigManager configManager;

void registerWebRoutes(WebServer &server)
{
    server.on("/", [&server]()
              { handleRoot(server); });

    server.on("/config", [&server]()
              { handleConfig(server, configManager); });

    server.onNotFound([&server]()
                      { server.send(404, "text/html", "<h1>404 - Página no encontrada</h1>"); });

    server.on("/api/intensity", HTTP_POST, [&server]()
              { handleApiIntensity(server); });
}