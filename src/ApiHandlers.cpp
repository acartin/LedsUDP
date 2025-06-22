#include "ApiHandlers.h"
#include "StateManager.h"

extern StateManager stateManager;

void handleApiIntensity(WebServer &server)
{
    int slave = server.arg("slave").toInt();
    int intensity = server.arg("intensity").toInt();
    String mode = server.arg("mode");
    stateManager.setIntensity(mode, slave, intensity);
    Serial.printf("[API] Intensidad actualizada: modo=%s slave=%d valor=%d\n", mode.c_str(), slave, intensity);
    server.send(200, "text/plain", "OK");
}