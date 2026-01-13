#include <user_manager.h>

//builder
user::user() :_name("default"), _id(""), _state(false){}

//SET NAME
void user::setName(const char* name){
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
bool user::addAlarm(uint8_t index, uint8_t hour, uint8_t min, const char* label){
    
    //check if index is valid and define alarm
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
const char* user::getName() const{
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

//return formated time, label and state
std::string user::getAlarms() const{
    std::string msg;
    char charAlarms[64];

    for (const auto& alarm : myAlarms){
        snprintf(charAlarms, sizeof(charAlarms), "*Alarme:* [%02d:%02d] *Label:* %s *State:* %s \n",
        alarm.getAlarm().tm_hour,
        alarm.getAlarm().tm_min,
        alarm.getLabel(),
        alarm.getState() ? "ON" : "OFF");

        msg += charAlarms;
    }

    return msg;
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

