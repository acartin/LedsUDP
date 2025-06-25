#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "UdpSender.h"
#include "AppConfig.h"

WiFiUDP udp;

void sendEstadoUDP(const char *modo, int numSlaves, const int *intensidades, const char **colores, const bool *onOff)
{
    StaticJsonDocument<512> doc;
    doc["tipo"] = "estado";
    doc["modo"] = modo;
    JsonArray valores = doc.createNestedArray("valores");
    for (int i = 0; i < numSlaves; ++i)
    {
        JsonObject slave = valores.createNestedObject();
        slave["slave"] = i;
        slave["intensidad"] = intensidades[i];
        slave["color"] = colores[i];
        slave["on"] = onOff[i];
    }
    char buffer[512];

    // Imprime el JSON por Serial antes de enviarlo

    size_t len = serializeJson(doc, buffer, sizeof(buffer));
    Serial.print("[UDP] Enviando: ");
    Serial.write(buffer, len);
    Serial.println();

    udp.beginPacket(UDP_BROADCAST_IP, SLAVE_UDP_PORT);
    udp.write((uint8_t *)buffer, len);
    udp.endPacket();
}

void sendTapUDP(const int *intensidades, const char **colores, int numSlaves, int duracion)
{
    StaticJsonDocument<512> doc;
    doc["tipo"] = "tap";
    doc["duracion"] = duracion;
    JsonArray valores = doc.createNestedArray("valores");
    for (int i = 0; i < numSlaves; ++i)
    {
        JsonObject slave = valores.createNestedObject();
        slave["slave"] = i;
        slave["intensidad"] = intensidades[i];
        slave["color"] = colores[i];
    }
    char buffer[512];
    size_t len = serializeJson(doc, buffer, sizeof(buffer));
    Serial.print("[UDP] Enviando TAP: ");
    Serial.write(buffer, len);
    Serial.println();
    udp.beginPacket(UDP_BROADCAST_IP, SLAVE_UDP_PORT);
    udp.write((uint8_t *)buffer, len);
    udp.endPacket();
}

void sendWatchdogUDP()
{
    StaticJsonDocument<128> doc;
    doc["tipo"] = "watchdog";
    doc["timestamp"] = millis(); // o usa time(NULL) si tienes RTC

    char buffer[128];
    size_t len = serializeJson(doc, buffer, sizeof(buffer));
    Serial.print("[UDP] Enviando WATCHDOG: ");
    Serial.write(buffer, len);
    Serial.println();

    udp.beginPacket(UDP_BROADCAST_IP, SLAVE_UDP_PORT);
    udp.write((uint8_t *)buffer, len);
    udp.endPacket();
}