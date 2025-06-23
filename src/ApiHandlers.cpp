#include "ApiHandlers.h"
#include "StateManager.h"
#include "ConfigManager.h"

extern ConfigManager configManager;
extern StateManager stateManager;

void handleApiIntensity(WebServer &server)
{
    int slave = server.arg("slave").toInt();
    int intensity = server.arg("intensity").toInt();
    String mode = server.arg("mode");
    ConfigManager::Config config = configManager.loadConfig();
    stateManager.setIntensity(mode, slave, intensity, config);
    Serial.printf("[API] Intensidad actualizada: modo=%s slave=%d valor=%d\n", mode.c_str(), slave, intensity);
    server.send(200, "text/plain", "OK");
}

void handleApiTap(WebServer &server)
{
    String mode = server.arg("mode");
    String effect = server.arg("effect");
    String color = server.arg("color");
    int speed = server.arg("speed").toInt();

    // Aquí puedes guardar estos valores en tu estructura StateManager
    stateManager.setTapEffect(mode, effect, color, speed);

    Serial.printf("[API] TAP: modo=%s efecto=%s color=%s speed=%d\n", mode.c_str(), effect.c_str(), color.c_str(), speed);
    server.send(200, "text/plain", "OK");
}