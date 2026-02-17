#pragma once
#include <wifi_manager.h>
#include <time.h>

bool ntpSync(const int32_t  gmtOffset_sec, const int32_t daylightOffset_sec,const char* ntpServer, const uint32_t MaxWaitTime);

//get local time and return an struct tm
tm getTime();