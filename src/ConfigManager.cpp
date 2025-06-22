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

    return config;
}