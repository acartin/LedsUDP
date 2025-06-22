src/
Main.cpp
WebRoutes.cpp / .h // Todas las rutas HTTP
WebHome.cpp / .h // Página principal y lógica de UI
WebConfig.cpp / .h // Página de configuración de slaves
ApiHandlers.cpp / .h // Endpoints tipo /api/ para AJAX (slider, color, tap, etc.)
UdpDaemon.cpp / .h // Demonio que envía los estados por UDP
StateManager.cpp / .h // Estructura y lógica de estados por modo/slave
include/
ConfigManager.h
WebRoutes.h
WebHome.h
WebConfig.h
ApiHandlers.h
UdpDaemon.h
StateManager.h

#include "ApiHandlers.h"
#include "StateManager.h"

extern StateManager stateManager;

void handleApiIntensity(WebServer &server) {
int slave = server.arg("slave").toInt();
int intensity = server.arg("intensity").toInt();
String mode = server.arg("mode");
stateManager.setIntensity(mode, slave, intensity);
Serial.printf("[API] Intensidad actualizada: modo=%s slave=%d valor=%d\n", mode.c_str(), slave, intensity);
server.send(200, "text/plain", "OK");
}
