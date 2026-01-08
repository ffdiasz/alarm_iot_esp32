#pragma once
#include <cstdint>
#include <string>
#include <time.h>

struct AlarmTime{
    uint8_t _hour;
    uint8_t _minutes;
};

class Alarm{
private:
    std::string _label;
    struct tm _AlarmTime;
    bool _state;

public:
    //builder
    Alarm();

    //Return a string with time, label and state
    std::string getStatus() const;

    //Return alarm state (ON/OFF)
    bool getState() const;

    //sets
    void setLabel(std::string& label);
    bool setTime(uint8_t hour, uint8_t minutes);
    void setState(bool state);

    //Check if is time to sound alarme
    bool checkTime(const tm& timeNow) const;
};