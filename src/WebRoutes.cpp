#include "WebRoutes.h"
#include "WebHome.h"
#include "WebSlaves.h"
#include "ConfigManager.h"
#include "ApiHandlers.h"
#include "WebModos.h"

// Declara tu instancia global de ConfigManager (debe estar en Main.cpp y marcada como extern aquí si es global)
extern ConfigManager configManager;

void registerWebRoutes(WebServer &server)
{
    server.on("/", [&server]()
              { handleRoot(server); });

    server.on("/slaves", [&server]()
              { handleSlaves(server, configManager); });

    server.onNotFound([&server]()
                      { server.send(404, "text/html", "<h1>404 - Página no encontrada</h1>"); });

    server.on("/modos", HTTP_GET, [&server]()
              { handleModos(server); });

    server.on("/modos", HTTP_POST, [&server]()
              { handleModosPost(server); });

    server.on("/api/intensity", HTTP_POST, [&server]()
              { handleApiIntensity(server); });

    server.on("/api/tap", HTTP_POST, [&server]()
              { handleApiTap(server); });
}