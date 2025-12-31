#pragma once
#include <iostream>
#include <cstdint>
#include <string>

struct AlarmTime{
    uint8_t _hour;
    uint8_t _minutes;
};

class Alarm{
private:
    std::string _title;
    AlarmTime _timeAlarm;
    bool _state;

public:
    //builder
    Alarm();

    //Status
    std::string getStatus() const;

    //sets
    void setTitle(std::string& title);
    void setTime(AlarmTime time);
    void setState(bool state);
};