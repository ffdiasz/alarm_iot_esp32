#pragma once
#include <cstdint>
#include <string>
#include <time.h>

class Alarm{
private:
    const char* _label;
    struct tm _AlarmTime;
    bool _state;

public:
    //builder
    Alarm();

    //Gets
    const struct tm& getAlarm() const;
    bool getState() const;
    const char* getLabel() const;

    //sets
    void setLabel(const char* label);
    bool setTime(uint8_t hour, uint8_t minutes);
    void setState(bool state);

    //Check if is time to sound alarme
    bool checkTime(const tm& timeNow) const;
};