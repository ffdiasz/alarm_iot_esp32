#include <alarm_manager.h>

//builder default
Alarm::Alarm(){
    _title = "";
    _timeAlarm = {0,0};
    _state = false;
}

//Status
std::string Alarm::getStatus() const{
    std::string status = "Alarm: ";
    status += _title;
    status += " Time: ";

    //format hours
    if(_timeAlarm._hour <10){
        status += "0";
        status += std::to_string(_timeAlarm._hour);
    } 
    
    else {
        status += std::to_string(_timeAlarm._hour);
    }

    status += ":";

    //format minutes
    if(_timeAlarm._minutes <10){
        status += "0";
        status += std::to_string(_timeAlarm._minutes);
    }

    else {
        status += std::to_string(_timeAlarm._minutes);
    }

    status += " State: ";
    status += (_state) ? "Enabled" : "Disabled";

    return status;

}

//set title
void Alarm::setTitle(std::string& title){
    _title = title;
}

//set time
void Alarm::setTime(AlarmTime time){
    _timeAlarm = time;
}

//set state
void Alarm::setState(bool state){
    _state = state;
}

