#include "SystemControl.h"

//Constructor
SystemControl :: SystemControl(UniversalTelegramBot& TelegramBot, std::array <user,4>& users)
:_TelegramBot(TelegramBot), _users(users)
{

}

void SystemControl::HandleMessages(uint16_t numMessages){
    Serial.print("HandleMessages: ");
    Serial.println(numMessages);

    for (uint16_t i = 0; i < numMessages; i++){
        const String& id = _TelegramBot.messages[i].chat_id;
        const String& text = _TelegramBot.messages[i].text;

        if (text.length() == 0){
            continue; //pass to the next iteration
        }

        if (text == "/start"){
            const char* messageSend = "AlarmClockBot begin";
            _TelegramBot.sendMessage(id,messageSend);
        }
    }
}

//if user is active and have alarm to ring return true
bool SystemControl::CheckAlarms(struct tm& timeNow){
    for (const auto& user : _users){
        if (user.isActive() && user.checkAlarms(timeNow)){
            return true;
        }

        return false;
    }
}