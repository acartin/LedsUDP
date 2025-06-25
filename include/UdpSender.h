#pragma once

void sendEstadoUDP(const char *modo, int numSlaves, const int *intensidades, const char **colores, const bool *onOff);
void sendTapUDP(const int *intensidades, const char **colores, int numSlaves, int duracion);
void sendWatchdogUDP();