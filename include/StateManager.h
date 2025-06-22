#pragma once

#include <Arduino.h>
#include <String.h>

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
    static const int MAX_SLAVES = 8;
    static const int MAX_MODES = 6;

    // Por ejemplo: modos[modo][slave]
    SlaveState modos[MAX_MODES][MAX_SLAVES];

    int modeNameToIndex(const String &modeName)
    {
        if (modeName == "Entrada")
            return 0;
        if (modeName == "Vals")
            return 1;
        if (modeName == "Ceremonia")
            return 2;
        if (modeName == "Fotobooth")
            return 3;
        if (modeName == "Crucero")
            return 4;
        if (modeName == "Beat")
            return 5;
        return 0; // Por defecto, Entrada
    }

    void setIntensity(const String &mode, int slave, uint8_t intensity);
    // ...otros setters/getters...
};