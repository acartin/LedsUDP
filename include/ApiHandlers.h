#pragma once
#include <WebServer.h>

void handleApiIntensity(WebServer &server);
// Puedes agregar aquí otros handlers, por ejemplo:
void handleApiColor(WebServer &server);
void handleApiOnOff(WebServer &server);
void handleApiTap(WebServer &server);
void handleApiGetModeState(WebServer &server);
void handleApiSetMode(WebServer &server);