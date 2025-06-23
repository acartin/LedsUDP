#include "StateManager.h"

void StateManager::setIntensity(const String &mode, int slave, uint8_t intensity, const ConfigManager::Config &config)
{
    int idx = modeNameToIndex(mode, config);
    if (slave >= 0 && slave < MAX_SLAVES && idx >= 0 && idx < MAX_MODES)
    {
        modos[idx][slave].intensity = intensity;
    }
}

void StateManager::setTapEffect(const String &mode, const String &effect, const String &color, int speed)
{
    // Aquí deberías guardar los valores en una estructura adecuada
    // Por ejemplo, podrías tener un struct TapState por modo
    // Esto es solo un ejemplo de impresión:
    Serial.printf("Guardado TAP: modo=%s efecto=%s color=%s speed=%d\n", mode.c_str(), effect.c_str(), color.c_str(), speed);
}