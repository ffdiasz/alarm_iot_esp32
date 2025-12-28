#pragma once
#include "../include/alarm_manager.h"

//user class
class user{
private:
    std::string _name;
    std::string _id;
    Alarm myAlarms[5];


public:
    //builder
    user();

    //sets
    void setName(std::string name);
    void setId(std::string id);

    //gets
    const std::string& getName() const;
    const std::string& getId() const;
    std::string getAlarms() const;
    
};
