#include "wifi_manager.h"

bool wifiConect(const char* ssid, const char* password, uint32_t maxWaitTime){

    //if wifi disconect this will act automatically.
    WiFi.setAutoConnect(true);
    //save wifi credentials
    WiFi.persistent(true);

    Serial.printf("Connecting to %s\n",ssid);

    WiFi.begin(ssid, password);

    uint32_t previousErrorTime = millis();
    uint32_t previousDotTime = millis();
    uint32_t dotTime = 500;

    while (WiFi.status() != WL_CONNECTED) {

        uint32_t now = millis();
        
        if (now - previousDotTime > dotTime){
            Serial.print(".");
            previousDotTime = millis();
        }

        if (now - previousErrorTime > maxWaitTime){
            Serial.println("");
            Serial.println("Error: WiFi not connected");
            Serial.println("Check ssid and password");

            return false;
        }
    }

    Serial.println("");
    Serial.println("WiFi connected.");

    return true;
}


void checkWifiStatus(){
    static bool lastState = false;
    bool state = (WiFi.status() == WL_CONNECTED);

    //normal
    if (state && lastState){
        return;
    }

    //DISCONNECTED
    if (!state && lastState){
        Serial.println("Wifi disconnected, trying to connect...");
        lastState = false;
        return;
    }

    //RECONNECTED
    if (state && !lastState){
        Serial.println("Wifi reconnected");
        lastState = true;
        return;
    }

    //Trying to connect
    if (!state && !lastState){
        Serial.println("trying to connect...");
        WiFi.begin();
        return;
    }
}
