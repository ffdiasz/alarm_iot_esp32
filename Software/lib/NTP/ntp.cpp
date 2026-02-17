#include "ntp.h"

bool ntpSync(const int32_t  gmtOffset_sec, const int32_t daylightOffset_sec,const char* ntpServer, const uint32_t MaxWaitTime){

    if (WiFi.status() != WL_CONNECTED){
        Serial.print("ERRO: Wifi isn't connect, trying to resync again ...");
        return false;
    }

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    time_t t = time(NULL);

    uint32_t previous = millis();
    uint32_t currentTime = millis();

    while ( t < 1600000000 && (currentTime - previous < MaxWaitTime)){
        currentTime = millis();
        t = time(NULL);
    }
        
    return (t > 1600000000);
}

//get localTime and return an struct tm
tm getTime(){
    time_t time0;
    time(&time0);
    struct tm *timeNow = localtime(&time0);

    return *timeNow;
}