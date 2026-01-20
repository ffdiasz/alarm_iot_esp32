#include "alarm_manager.h"

//Check active alarms and return true when its time to trigger
bool alarm_manager::checkAlarms(struct tm &timeNow) const{
    for (const auto& alarm : _alarms)
    {
        //alarm active and time to trigger
        if ((alarm.getState()) && (alarm.checkTime(timeNow))){
            return true; 
        } 
    }

    return false;
}

//return std::string of formated time, label and state
std::string alarm_manager::getAlarms() const{
    std::string msg;
    char charAlarms[64];

    for (const auto& alarm : _alarms){
        snprintf(charAlarms, sizeof(charAlarms), "*Alarme:* %02d:%02d *Label:* %s *State:* %s \n",
        alarm.getAlarm().tm_hour,
        alarm.getAlarm().tm_min,
        alarm.getLabel(),
        alarm.getState() ? "ON" : "OFF");

        msg += charAlarms;
    }

    return msg;
}

//config an alarm in array
bool alarm_manager::addAlarm(uint8_t index, uint8_t hour, uint8_t min, const char* label){
    //check if index is valid and define alarm
    if(index >=0 && index < maxAlarms){ 
        _alarms[index].setTime(hour,min);
        _alarms[index].setLabel(label);
        _alarms[index].setState(true);

        return true;
    }

    //index out of scope
    return false;
}
