#include "SystemControl.h"

//builder
SystemControl :: SystemControl(bot& TelegramBot)
:_TelegramBot(TelegramBot)
{

}

void SystemControl::HandleMessages(uint16_t numMessages){
    Serial.print("HandleNewMessages ");
    Serial.println(numMessages);

    //Reserve space and get the newMessages
    std::vector<Message> messageList;
    messageList.reserve(numMessages);
    messageList = _TelegramBot.getMessages(numMessages);

    //Execute the commands
    for (const auto& msg : messageList ){

        if(msg.text == "/start"){
            String Welcome = "AlarmClockBot INICIADO COM SUCESSO! \n";
            Welcome+= "principais comandos:\n";
            Welcome+="1-\n";
            Welcome+="2-\n";
            Welcome+="3-\n";

            Message messageToSent {msg.id,Welcome};

            _TelegramBot.sendMessage(&messageToSent);
        }
    }
}