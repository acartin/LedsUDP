#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>
#include <FastLED.h>
#include "AppConfig.h"

class ConfigManager
{
private:
    Preferences prefs;
    static const char *STORAGE_NAMESPACE; // Nombre para la "carpeta" en memoria

public:
    struct SlaveConfig
    {
        char name[16]; // Nombre descriptivo del slave
        char ip[16];   // "192.168.4.10"
    };

    struct EffectConfig
    {
        char name[16];
        uint32_t color;
        uint8_t intensity;
    };

    struct Config
    {
        // Configuración de slaves
        uint8_t numSlaves;
        uint8_t numModes;
        SlaveConfig slaves[MAX_SLAVES];
        char modeNames[MAX_MODES][16]; // Por ejemplo: "Entrada", "Vals", etc.
        // Configuración de efectos
        uint8_t numEffects;
        EffectConfig effects[8]; // Ajusta el tamaño según lo que necesites
    };

    ConfigManager();

    bool begin();                          // Inicia el sistema de almacenamiento
    bool saveConfig(const Config &config); // Guarda la configuración completa
    Config loadConfig();                   // Carga la configuración completa
    void resetConfig();                    // Resetea a valores por defecto

private:
    Config createDefaultConfig(); // Crea configuración por defecto
};

#endif