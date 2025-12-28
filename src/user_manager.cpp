#include "../include/user_manager.h"

//builder
user::user() :_name("default"), _id("") {}

//SET NAME
void user::setName(std::string name){
    _name = "name";
}

//SET ID
void user::setId(std::string id){
    _id = id;
}

//GET USER NAME
const std::string& user::getName() const{
    return _name;
}

//GET USER ID
const std::string& user::getId() const{
    return _id;
}

//GET USER ALARMS
std::string user::getAlarms() const{

    std::string status; 

    for (const auto& alarm : myAlarms){
        status +=alarm.getStatus();
        //breakLine
        status += "\n";
    }

    return status;
}

