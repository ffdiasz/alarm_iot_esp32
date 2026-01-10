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
        _newMessages = _TelegramBot.getUpdates(_TelegramBot.last_message_received + 1);
        
        if (_newMessages > 0){
            _State = TelegramStates::handleMessages;
        }
        
        break;
    
    //Handle New Messages
    case TelegramStates::handleMessages:
        HandleMessages(_newMessages);
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