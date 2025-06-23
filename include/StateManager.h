#pragma once

#include <Arduino.h>
#include <String.h>
#include "AppConfig.h"
#include "ConfigManager.h"

// Ejemplo de estructura básica para un modo y slave
struct SlaveState
{
    uint8_t intensity;
    uint32_t color; // RGB hex
    bool onoff;
};

class StateManager
{
public:
    // static const int MAX_SLAVES = 8;
    // static const int MAX_MODES = 6;

    // Por ejemplo: modos[modo][slave]
    SlaveState modos[MAX_MODES][MAX_SLAVES];

    int modeNameToIndex(const String &modeName, const ConfigManager::Config &config)
    {
        for (int i = 0; i < config.numModes; ++i)
        {
            if (modeName == config.modeNames[i])
                return i;
        }
        return 0; // Por defecto, el primero
    }

    void setTapEffect(const String &mode, const String &effect, const String &color, int speed);

    void setIntensity(const String &mode, int slave, uint8_t intensity, const ConfigManager::Config &config); // ...otros setters/getters...
};