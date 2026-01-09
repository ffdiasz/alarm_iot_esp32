#include <user_manager.h>

//builder
user::user() :_name("default"), _id(""), _state(false){}

//SET NAME
void user::setName(std::string& name){
    _name = name;
}

//SET ID
void user::setId(std::string& id){
    _id = id;
}

void user::setState(bool state){
    _state = state;
}

//GET USER NAME
const std::string& user::getName() const{
    return _name;
}

//GET USER ID
const std::string& user::getId() const{
    return _id;
}

//Get user state
bool user::isActive() const{
    return _state;
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

//Check active alarms, if its time to ring return true
bool user::checkAlarms(struct tm& timeNow) const{
    for (const auto& alarm : myAlarms){
        
        if ((alarm.getState()) && (alarm.checkTime(timeNow))){
            return true; 
        } 
    }

    return false;
}

