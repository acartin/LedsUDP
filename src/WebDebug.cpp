#include "WebDebug.h"

void handleDebug(WebServer &server, ConfigManager &configManager)
{
    ConfigManager::Config config = configManager.loadConfig();

    String out = "numModes: " + String(config.numModes) + "\n";
    out += "numSlaves: " + String(config.numSlaves) + "\n";
    for (int m = 0; m < config.numModes; ++m)
    {
        out += "Modo " + String(m) + " (" + String(config.modeNames[m]) + "):\n";
        for (int s = 0; s < config.numSlaves; ++s)
        {
            char colorHex[7];
            snprintf(colorHex, sizeof(colorHex), "%06X", config.slaveStates[m][s].color);
            out += "  Slave " + String(s) + ": Int=" + String(config.slaveStates[m][s].intensity) +
                   " Color=" + colorHex + "\n";
        }
    }
    server.send(200, "text/plain", out);
}