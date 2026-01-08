#pragma once
#include "UniversalTelegramBot.h"
#include "user_manager.h"
#include "array"

enum class SystemStates : int8_t{
    idle,
    newMessage,
    handlingMessage,
    error,
};

class SystemControl {
private:
    std::array <user,4>& _users;
    UniversalTelegramBot& _TelegramBot;
public:
    //builder
    SystemControl(UniversalTelegramBot& TelegramBot, std::array <user,4>& users);

    void HandleMessages(uint16_t numMessages);

};


