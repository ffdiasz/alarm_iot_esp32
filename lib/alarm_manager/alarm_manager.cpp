#include "alarm_manager.h"

//builder default
Alarm::Alarm(){
    _label = "";
    _state = false;
    _AlarmTime.tm_hour = 0;
    _AlarmTime.tm_min = 0;
}

//Return a struct tm alarm
const struct tm& Alarm::getAlarm() const{
    return _AlarmTime;
}

//Return alarm state (ON/OFF)
bool Alarm::getState() const{
    return _state;
}

const char* Alarm::getLabel() const{
    return _label;
}

//Check if it's time to sound alarm
bool Alarm::checkTime(const tm& timeNow) const{

    bool hourCheck = (timeNow.tm_hour == _AlarmTime.tm_hour);
    bool minCheck = (timeNow.tm_min == _AlarmTime.tm_min);

    if (hourCheck && minCheck){
        return true;
    }

    return false;
}

//Set Alarm label
void Alarm::setLabel(const char* label){
    _label = label;
}

//set time
bool Alarm::setTime(uint8_t hour, uint8_t minutes){

    if ((hour >= 0 && hour <=23) && (minutes >= 0 && minutes <=59)){
        _AlarmTime.tm_hour = hour;
        _AlarmTime.tm_min = minutes;

        return true;
    }

    return false;
}

//set state
void Alarm::setState(bool state){
    _state = state;
}