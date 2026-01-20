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

alarm_manager& user::getUserAlarms(){
    return _UserAlarms;
}
