#include "StateManager.h"

void StateManager::setIntensity(const String &mode, int slave, uint8_t intensity)
{
    int idx = modeNameToIndex(mode);
    if (slave >= 0 && slave < MAX_SLAVES && idx >= 0 && idx < MAX_MODES)
    {
        modos[idx][slave].intensity = intensity;
    }
}