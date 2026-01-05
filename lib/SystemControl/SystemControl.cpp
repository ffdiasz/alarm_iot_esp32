#include "SystemControl.h"

//builder
SystemControl :: SystemControl(UniversalTelegramBot& TelegramBot)
:_TelegramBot(TelegramBot)
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