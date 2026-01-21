#pragma once
#include <alarm_manager.h>

//user class
class user{
private:
    const char* _name;
    const char* _id;
    bool _state;

    alarm_manager _UserAlarms;

public:
    //builder
    user();

    //sets
    void setName(const char* name);
    void setId(const char* id);
    void setState(bool state);

    //gets
    const char* getName() const;
    const char* getId() const;
    bool isActive() const;

    alarm_manager& getUserAlarms();
};
