#pragma once
#include "alarm.h"
#include "array"

constexpr const uint8_t maxAlarms = 5;

class alarm_manager{

private:
    std::array <Alarm,maxAlarms> _alarms;
public:

    //Check active alarms
    bool checkAlarms(struct tm& timeNow) const;

    //return string of alarms
    std::string getAlarms() const;

    //config an alarm in array
    bool addAlarm(uint8_t index, uint8_t hour, uint8_t min, const char* label);
};