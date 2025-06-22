#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>
#include <FastLED.h>

class ConfigManager
{
private:
    Preferences prefs;
    static const char *STORAGE_NAMESPACE; // Nombre para la "carpeta" en memoria

public:
    static const int MAX_SLAVES = 8;

    struct SlaveConfig
    {
        char name[16]; // Nombre descriptivo del slave
        char ip[16];   // "192.168.4.10"
    };

    struct Config
    {
        // Configuración de slaves
        uint8_t numSlaves;
        SlaveConfig slaves[MAX_SLAVES];
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