#include <user_manager.h>

//builder
user::user() :_name("default"), _id(""), _state(false){}

//SET NAME
void user::setName(std::string& name){
    _name = name;
}

//SET ID
void user::setId(const char* id){
    _id = id;
}

//Set user state
void user::setState(bool state){
    _state = state;
}

//check unactive alarms and replace with the new one
//return false if all are active.
bool user::addAlarm(uint8_t index, uint8_t hour, uint8_t min, std::string label){
    
    if(index >=0 && index < maxSizeOfAlarmsArray){ 
        myAlarms[index].setTime(hour,min);
        myAlarms[index].setLabel(label);
        myAlarms[index].setState(true);

        return true;
    }

    //all alarms in use
    return false;
}

//GET USER NAME
const std::string& user::getName() const{
    return _name;
}

//GET USER ID
const char* user::getId() const{
    return _id;
}

//Get user state
bool user::isActive() const{
    return _state;
}

//GET USER ALARMS
std::string user::getAlarms() const{
    std::string status; 

    for (uint8_t i = 0; i < maxSizeOfAlarmsArray; i++){
        status = ((i+1) + ": " + myAlarms[i].getStatus());
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

