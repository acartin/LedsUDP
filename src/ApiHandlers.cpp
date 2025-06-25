#include "ApiHandlers.h"
#include "StateManager.h"
#include "ConfigManager.h"
#include "UdpSender.h"

extern ConfigManager configManager;
extern StateManager stateManager;

void handleApiIntensity(WebServer &server)
{
    int slave = server.arg("slave").toInt();
    int intensity = server.arg("intensity").toInt();
    String mode = server.arg("mode");
    ConfigManager::Config config = configManager.loadConfig();

    // Busca el índice del modo
    int modeIdx = -1;
    for (int i = 0; i < config.numModes; ++i)
    {
        if (mode == config.modeNames[i])
        {
            modeIdx = i;
            break;
        }
    }
    if (modeIdx >= 0 && slave >= 0 && slave < config.numSlaves)
    {
        config.slaveStates[modeIdx][slave].intensity = intensity;
        configManager.saveConfig(config);
        Serial.printf("[API] Intensidad actualizada: modo=%s (idx=%d) slave=%d valor=%d\n", mode.c_str(), modeIdx, slave, intensity);
        stateManager.setIntensity(mode, slave, intensity, config);

        // Recarga la configuración para asegurar que tienes los datos actualizados
        config = configManager.loadConfig();

        // --- Armado de arrays y envío UDP ---
        int intensidades[MAX_SLAVES];
        const char *colores[MAX_SLAVES];
        bool onOff[MAX_SLAVES];
        char coloresHex[MAX_SLAVES][8];

        for (int i = 0; i < config.numSlaves; ++i)
        {
            intensidades[i] = config.slaveStates[modeIdx][i].intensity;
            snprintf(coloresHex[i], sizeof(coloresHex[i]), "#%06X", config.slaveStates[modeIdx][i].color);
            colores[i] = coloresHex[i];
            onOff[i] = config.slaveStates[modeIdx][i].on;
        }

        sendEstadoUDP(
            config.modeNames[modeIdx], // <-- sin .c_str()
            config.numSlaves,
            intensidades,
            colores,
            onOff);
        // --- Fin envío UDP ---

        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(400, "text/plain", "Modo o slave inválido");
    }
}

void handleApiTap(WebServer &server)
{
    String mode = server.arg("mode");
    int speed = server.arg("speed").toInt();

    ConfigManager::Config config = configManager.loadConfig();

    int modeIdx = -1;
    for (int i = 0; i < config.numModes; ++i)
        if (mode == config.modeNames[i]) { modeIdx = i; break; }

    if (modeIdx >= 0)
    {
        int intensidades[MAX_SLAVES];
        const char *colores[MAX_SLAVES];
        char coloresHex[MAX_SLAVES][8];

        for (int i = 0; i < config.numSlaves; ++i)
        {
            intensidades[i] = config.slaveStates[modeIdx][i].intensity;
            snprintf(coloresHex[i], sizeof(coloresHex[i]), "#%06X", config.slaveStates[modeIdx][i].color);
            colores[i] = coloresHex[i];
        }

        sendTapUDP(intensidades, colores, config.numSlaves, speed);

        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(400, "text/plain", "Modo inválido");
    }
}

// maneja el despliegue del estado de la matriz de configuracon
void handleApiGetModeState(WebServer &server)
{
    String mode = server.arg("mode");
    ConfigManager::Config config = configManager.loadConfig();

    int modeIdx = -1;
    for (int i = 0; i < config.numModes; ++i)
    {
        if (mode == config.modeNames[i])
        {
            modeIdx = i;
            break;
        }
    }

    if (modeIdx >= 0)
    {
        String json = "[";
        for (int s = 0; s < config.numSlaves; ++s)
        {
            if (s > 0)
                json += ",";
            json += "{";
            json += "\"intensity\":" + String(config.slaveStates[modeIdx][s].intensity);

            // Aquí el formato correcto para el color:
            char colorHex[8];
            snprintf(colorHex, sizeof(colorHex), "#%06X", config.slaveStates[modeIdx][s].color);
            json += ",\"color\":\"";
            json += colorHex;
            json += "\"";

            json += ",\"on\":" + String(config.slaveStates[modeIdx][s].on ? "true" : "false");
            json += "}";
        }
        json += "]";
        server.send(200, "application/json", json);
    }
    else
    {
        server.send(400, "text/plain", "Modo inválido");
    }
}
void handleApiSetMode(WebServer &server)
{
    String mode = server.arg("mode");
    ConfigManager::Config config = configManager.loadConfig();

    int modeIdx = -1;
    for (int i = 0; i < config.numModes; ++i)
    {
        if (mode == config.modeNames[i])
        {
            modeIdx = i;
            break;
        }
    }

    if (modeIdx >= 0)
    {
        // --- Armado de arrays y envío UDP ---
        int intensidades[MAX_SLAVES];
        const char *colores[MAX_SLAVES];
        bool onOff[MAX_SLAVES];
        char coloresHex[MAX_SLAVES][8];

        for (int i = 0; i < config.numSlaves; ++i)
        {
            intensidades[i] = config.slaveStates[modeIdx][i].intensity;
            snprintf(coloresHex[i], sizeof(coloresHex[i]), "#%06X", config.slaveStates[modeIdx][i].color);
            colores[i] = coloresHex[i];
            onOff[i] = config.slaveStates[modeIdx][i].on;
        }

        sendEstadoUDP(
            config.modeNames[modeIdx],
            config.numSlaves,
            intensidades,
            colores,
            onOff);

        Serial.printf("[API] Modo cambiado: %s (idx=%d)\n", mode.c_str(), modeIdx);
        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(400, "text/plain", "Modo inválido");
    }
}
void handleApiColor(WebServer &server)
{
    int slave = server.arg("slave").toInt();
    String mode = server.arg("mode");
    uint32_t color = strtoul(server.arg("color").c_str(), nullptr, 16);
    ConfigManager::Config config = configManager.loadConfig();

    int modeIdx = -1;
    for (int i = 0; i < config.numModes; ++i)
    {
        if (mode == config.modeNames[i])
        {
            modeIdx = i;
            break;
        }
    }
    if (modeIdx >= 0 && slave >= 0 && slave < config.numSlaves)
    {
        config.slaveStates[modeIdx][slave].color = color;
        configManager.saveConfig(config);

        // Recarga la configuración para asegurar que tienes los datos actualizados
        config = configManager.loadConfig();

        // --- Armado de arrays y envío UDP ---
        int intensidades[MAX_SLAVES];
        const char *colores[MAX_SLAVES];
        bool onOff[MAX_SLAVES];
        char coloresHex[MAX_SLAVES][8];

        for (int i = 0; i < config.numSlaves; ++i)
        {
            intensidades[i] = config.slaveStates[modeIdx][i].intensity;
            snprintf(coloresHex[i], sizeof(coloresHex[i]), "#%06X", config.slaveStates[modeIdx][i].color);
            colores[i] = coloresHex[i];
            onOff[i] = config.slaveStates[modeIdx][i].on;
        }

        sendEstadoUDP(
            config.modeNames[modeIdx],
            config.numSlaves,
            intensidades,
            colores,
            onOff);
        // --- Fin envío UDP ---

        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(400, "text/plain", "Modo o slave inválido");
    }
    Serial.printf("[API] Color actualizado: modo=%s (idx=%d) slave=%d color=%06X\n", mode.c_str(), modeIdx, slave, color);
}
void handleApiOnOff(WebServer &server)
{
    int slave = server.arg("slave").toInt();
    String mode = server.arg("mode");
    bool on = server.arg("on") == "1";
    ConfigManager::Config config = configManager.loadConfig();

    int modeIdx = -1;
    for (int i = 0; i < config.numModes; ++i)
    {
        if (mode == config.modeNames[i])
        {
            modeIdx = i;
            break;
        }
    }
    if (modeIdx >= 0 && slave >= 0 && slave < config.numSlaves)
    {
        config.slaveStates[modeIdx][slave].on = on;
        configManager.saveConfig(config);

        // Recarga la configuración para asegurar que tienes los datos actualizados
        config = configManager.loadConfig();

        // --- Armado de arrays y envío UDP ---
        int intensidades[MAX_SLAVES];
        const char *colores[MAX_SLAVES];
        bool onOff[MAX_SLAVES];
        char coloresHex[MAX_SLAVES][8];

        for (int i = 0; i < config.numSlaves; ++i)
        {
            intensidades[i] = config.slaveStates[modeIdx][i].intensity;
            snprintf(coloresHex[i], sizeof(coloresHex[i]), "#%06X", config.slaveStates[modeIdx][i].color);
            colores[i] = coloresHex[i];
            onOff[i] = config.slaveStates[modeIdx][i].on;
        }

        sendEstadoUDP(
            config.modeNames[modeIdx],
            config.numSlaves,
            intensidades,
            colores,
            onOff);
        // --- Fin envío UDP ---

        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(400, "text/plain", "Modo o slave inválido");
    }
}
