#pragma once
#include <alarm_manager.h>

//user class
class user{
private:
    std::string _name;
    std::string _id;
    Alarm myAlarms[5];
    bool _state;

public:
    //builder
    user();

    //sets
    void setName(std::string& name);
    void setId(std::string& id);
    void setState(bool state);

    //gets
    const std::string& getName() const;
    const std::string& getId() const;
    bool isActive() const;
    std::string getAlarms() const;

    //Check active alarms
    bool checkAlarms(struct tm& timeNow) const;
};
