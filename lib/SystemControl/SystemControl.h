#pragma once
#include "UniversalTelegramBot.h"
#include "user_manager.h"
#include "cstdint"
#include "string"

constexpr const uint8_t maxUsers = 4;

enum class TelegramStates : uint8_t{
    idle,
    checkUser,
    checkFreeUser,
    registerUser,
    getMessage,
    handle,
    erro,
};

//Messages receive from Telegram
enum class TelegramCommands : uint8_t{
    start,
    showAlarms,
    configAlarm,
    help,
    unknown,
};

enum class MachineState : uint8_t{
    waiting,
    sucess,
    erro
};

class SystemControl {
private:
    user_manager& _users;
    UniversalTelegramBot& _TelegramBot;
    const char* _LastUserID;
    TelegramStates _State;
    uint16_t _newMessages;
public:
    //Constructor
    SystemControl(UniversalTelegramBot& TelegramBot, user_manager& users);

    void TelegramManager();

    TelegramCommands getCommand(); 

    MachineState HandleMessages(const char* id, TelegramCommands msg); 

    MachineState configAlarm();

    MachineState newUser(uint8_t UserIndex);
};