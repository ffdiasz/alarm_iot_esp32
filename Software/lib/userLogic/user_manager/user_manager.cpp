#include "user_manager.h"

//if user is active and have alarm to ring return true
bool user_manager::CheckAlarms(struct tm& timeNow){
    for (auto& user: _users)
    {
        //user active?
        if (user.isActive())
        {
            //receive userAlarms reference
            alarm_manager userAlarms = user.getUserAlarms();

            //alarm trigged?
            if (userAlarms.checkAlarms(timeNow)){
                return true;
            }
        }
    }

    //no alarms triggered
    return false;
}

//return user index or -1 if doesn't exists
int8_t user_manager::findUserId(const char* id) const{
    for (uint8_t i = 0; i < maxUser; i++){
        if (_users[i].getId() == id){
            return i;
        }
    }
    //user id doesn't exists in array
    return -1;
}

//return user index or -1 if all user status is active
int8_t user_manager::hasFreeUser() const{
    for (uint8_t i = 0; i < maxUser; i++){
        if (!_users[i].isActive()){
            return i;
        }
    }
    //all user is active
    return -1;
}

//return a pointer to user
user* user_manager::getUser(uint8_t index)
{
    if (index >= 0 and index <maxUser){
        user* userPtr = &_users[index];
        return userPtr;
    }

    //out of scope
    else{
        return nullptr;
    }
}