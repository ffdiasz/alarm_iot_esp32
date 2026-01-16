#pragma once
#include "UniversalTelegramBot.h"
#include "user_manager.h"
#include "cstdint"
#include "array"
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
    std::array <user,maxUsers> _users;
    UniversalTelegramBot& _TelegramBot;
    const char* _LastUserID;
    TelegramStates _State;
    uint16_t _newMessages;
public:
    //Constructor
    SystemControl(UniversalTelegramBot& TelegramBot);

    void TelegramManager();

    bool CheckAlarms(struct tm& timeNow);

    int8_t findUserId(const char* id) const;

    int8_t hasFreeUser() const;

    TelegramCommands getCommand(); 

    MachineState HandleMessages(const char* id, TelegramCommands msg); 

    MachineState configAlarm();

    MachineState newUser(uint8_t UserIndex);
};