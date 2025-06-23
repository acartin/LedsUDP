#include "ConfigManager.h"

const char *ConfigManager::STORAGE_NAMESPACE = "busled";

ConfigManager::ConfigManager() {}

bool ConfigManager::begin()
{
    return prefs.begin(STORAGE_NAMESPACE, false);
}

bool ConfigManager::saveConfig(const Config &config)
{
    size_t written = prefs.putBytes("config", &config, sizeof(Config));
    return written == sizeof(Config);
}

ConfigManager::Config ConfigManager::loadConfig()
{
    Config config;

    // Si no hay configuración guardada, crear una por defecto
    if (!prefs.isKey("config"))
    {
        config = createDefaultConfig();
        saveConfig(config);
        return config;
    }

    // Leer configuración guardada
    size_t readSize = prefs.getBytes("config", &config, sizeof(Config));
    if (readSize != sizeof(Config))
    {
        config = createDefaultConfig();
        saveConfig(config);
    }

    // Validar rangos seguros
    if (config.numSlaves > MAX_SLAVES)
        config.numSlaves = MAX_SLAVES;
    if (config.numModes < 1)
        config.numModes = 1;
    if (config.numModes > MAX_MODES)
        config.numModes = MAX_MODES;

    return config;
}

void ConfigManager::resetConfig()
{
    Config config = createDefaultConfig();
    saveConfig(config);
}

ConfigManager::Config ConfigManager::createDefaultConfig()
{
    Config config;

    // Valores por defecto para slaves
    config.numSlaves = 2;
    strncpy(config.slaves[0].name, "Slave 1", sizeof(config.slaves[0].name));
    config.slaves[0].name[sizeof(config.slaves[0].name) - 1] = '\0';
    strncpy(config.slaves[0].ip, "192.168.4.10", sizeof(config.slaves[0].ip));
    config.slaves[0].ip[sizeof(config.slaves[0].ip) - 1] = '\0';

    strncpy(config.slaves[1].name, "Slave 2", sizeof(config.slaves[1].name));
    config.slaves[1].name[sizeof(config.slaves[1].name) - 1] = '\0';
    strncpy(config.slaves[1].ip, "192.168.4.11", sizeof(config.slaves[1].ip));
    config.slaves[1].ip[sizeof(config.slaves[1].ip) - 1] = '\0';

    for (int i = 2; i < MAX_SLAVES; ++i)
    {
        config.slaves[i].name[0] = '\0';
        config.slaves[i].ip[0] = '\0';
    }

    // Inicializa los modos por defecto
    config.numModes = 3;
    strncpy(config.modeNames[0], "Entrada", sizeof(config.modeNames[0]));
    strncpy(config.modeNames[1], "Beat", sizeof(config.modeNames[1]));
    strncpy(config.modeNames[2], "Manual", sizeof(config.modeNames[2]));
    for (int i = 3; i < MAX_MODES; ++i)
    {
        config.modeNames[i][0] = '\0'; // Inicializa vacíos
    }

    // Inicializa los efectos por defecto
    config.numEffects = 3;

    strncpy(config.effects[0].name, "Strobe", sizeof(config.effects[0].name));
    config.effects[0].name[sizeof(config.effects[0].name) - 1] = '\0';
    config.effects[0].color = 0xFFFFFF;
    config.effects[0].intensity = 128;

    strncpy(config.effects[1].name, "Fire", sizeof(config.effects[1].name));
    config.effects[1].name[sizeof(config.effects[1].name) - 1] = '\0';
    config.effects[1].color = 0xFF3300;
    config.effects[1].intensity = 128;

    strncpy(config.effects[2].name, "Fade", sizeof(config.effects[2].name));
    config.effects[2].name[sizeof(config.effects[2].name) - 1] = '\0';
    config.effects[2].color = 0x00FF00;
    config.effects[2].intensity = 128;

    // El resto vacíos
    for (int i = 3; i < 8; ++i)
    {
        config.effects[i].name[0] = '\0';
        config.effects[i].color = 0;
        config.effects[i].intensity = 0;
    }

    return config;
}