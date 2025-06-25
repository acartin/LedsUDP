#pragma once

constexpr int MAX_SLAVES = 8;
constexpr int MAX_MODES = 8; // O el número que prefieras

constexpr uint16_t SLAVE_UDP_PORT = 4210;
constexpr const char *UDP_BROADCAST_IP = "192.168.4.255";
constexpr const char *DEFAULT_SSID = "ESP32-LED-AP";
constexpr const char *DEFAULT_PASSWORD = "12345678";
constexpr const char *DEFAULT_MDNS_NAME = "iluminate"; // Cambia por el nombre que prefieras
const unsigned long WATCHDOG_INTERVAL = 1000;          // 1 segundoconst unsigned long WATCHDOG_INTERVAL = 1000; // 1 segundo