#pragma once

#include <WiFi.h>

// init wifi conection
// maxWaitTime parameter
bool wifiConect(const char* ssid, const char* password, uint32_t maxWaitTime);

//manage the wifi connection
bool checkWifiStatus();