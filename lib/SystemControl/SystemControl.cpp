#include "SystemControl.h"

//Constructor
SystemControl::SystemControl(UniversalTelegramBot& TelegramBot)
:_TelegramBot(TelegramBot), _State(TelegramStates::idle), _newMessages(0)
{

}

//if user is active and have alarm to ring return true
bool SystemControl::CheckAlarms(struct tm& timeNow){
    for (const auto& user : _users){
        if (user.isActive() && user.checkAlarms(timeNow)){
            return true;
        }
    }

    return false;
}

void SystemControl::HandleMessages(uint16_t numMessages){
    Serial.print("HandleMessages: ");
    Serial.println(numMessages);

    for (uint16_t i = 0; i < numMessages; i++){
        //c_str() return a const char pointer
        const char* id = _TelegramBot.messages[i].chat_id.c_str();
        //Using string& because i need a string to compare 
        const std::string& text = _TelegramBot.messages[i].text.c_str();

        int8_t UserIndex = findUserId(id);

        if (UserIndex == -1){
            const char* message = "User dont register";
            Serial.println(message);
            _TelegramBot.sendMessage(id,message);
            continue; //pass to the next iteration
        }

        if (text == ""){
            Serial.println("void message");
            continue; //pass to the next iteration
        }

        if (text == "/start"){
            Serial.println("/start");
            const char* messageSend = "AlarmClockBot begin";
            _TelegramBot.sendMessage(id,messageSend);
        }

        else if (text == "/showAlarms"){
            
        }
    }
}


void SystemControl::TelegramManager(){
    switch (_State){
    
        //waiting new messages from telegram
        case TelegramStates::idle:
        {
            _newMessages = _TelegramBot.getUpdates(_TelegramBot.last_message_received + 1);
            
            if (_newMessages > 0){
                Serial.print("New messages: ");
                Serial.println(_newMessages);
                _State = TelegramStates::checkUser;
            }
            
            break;
        }
        
        //Check if use is register
        case TelegramStates::checkUser:
        {
            _LastUserID = _TelegramBot.messages->chat_id.c_str();
            Serial.print("ID: ");
            Serial.println(_LastUserID);

            if (findUserId(_LastUserID) == -1){
                const char* message = "User doesn't register";
                Serial.println(message);
                _TelegramBot.sendMessage(_LastUserID,message);

                _State = TelegramStates::registerUser;
                break;
            }

            _State = TelegramStates::idle;
            break;
        }
        
        case TelegramStates::registerUser:
        {
            int8_t freeUserIndex = hasFreeUser();

            if(freeUserIndex == -1){
                const char* message = "All users is active";
                Serial.println(message);
                _TelegramBot.sendMessage(_LastUserID,message);

                _State = TelegramStates::idle;
                break;
            }

            //when a new user be registered return to idle state 
            if (newUser(freeUserIndex)){
                _State = TelegramStates::idle;
                break;
            }

            break;
        }

    }
}

//return user index or -1 if doesn't exists
int8_t SystemControl::findUserId(const char* id) const{
    for (uint8_t i = 0; i < maxUsers; i++){
        if (_users[i].getId() == id){
            return i;
        }
    }

    //user id doesn't exists in array
    return -1;
}

//return user index or -1 if all user status is active
int8_t SystemControl::hasFreeUser() const{
    for (uint8_t i = 0; i < maxUsers; i++){
        
        if (!_users[i].isActive()){
            return i;
        }
    }

    //all user is active
    return -1;
}

//state machine to register new user, return true when finished
bool SystemControl::newUser(uint8_t UserIndex){
    static uint8_t newUserState = 0;
    static const char* username;

    switch(newUserState){
        case 0://Register New User
        {
            const char* message0 = "Register user";
            Serial.println(message0);
            _TelegramBot.sendMessage(_LastUserID,message0);
            _TelegramBot.sendMessage(_LastUserID,"Send your username");
            
            newUserState = 1;
            break;
        }

        case 1://Waiting username
        { 
            Serial.println("Waiting username to register user");
            
            if (_TelegramBot.getUpdates(_TelegramBot.last_message_received + 1)){
                username = _TelegramBot.messages->text.c_str();
                Serial.print("Username receive: ");
                Serial.println(username);

                newUserState = 2;
                break;
            }
            
            //No response
            break;
        }

        case 2://Config User
        {
            _users[UserIndex].setName(username);
            _users[UserIndex].setId(_LastUserID);
            _users[UserIndex].setState(true);

            _TelegramBot.sendMessage(_LastUserID, "New user registered");
            _TelegramBot.sendMessage(_LastUserID,"Now you can use commands");

            newUserState = 0;
            return true; //User registered
        }
    }

    //in progress
    return false;
}