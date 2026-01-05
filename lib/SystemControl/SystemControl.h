#pragma once
#include "UniversalTelegramBot.h"
#include "user_manager.h"

enum class SystemStates : int8_t{
    idle,
    newMessage,
    handlingMessage,
    error,
};

class SystemControl {
private:
    user _Users[4];
    UniversalTelegramBot& _TelegramBot;
public:
    //builder
    SystemControl(UniversalTelegramBot& TelegramBot);

    void HandleMessages(uint16_t numMessages);

};


