#pragma once
#include <alarm_manager.h>

constexpr const uint8_t maxSizeOfAlarmsArray = 5;

//user class
class user{
private:
    std::string _name;
    const char* _id;
    Alarm myAlarms[maxSizeOfAlarmsArray];
    bool _state;

public:
    //builder
    user();

    //sets
    void setName(std::string& name);
    void setId(const char* id);
    void setState(bool state);
    bool addAlarm(uint8_t index, uint8_t hour, uint8_t min, std::string label);

    //gets
    const std::string& getName() const;
    const char* getId() const;
    bool isActive() const;
    std::string getAlarms() const;

    //Check active alarms
    bool checkAlarms(struct tm& timeNow) const;
};
