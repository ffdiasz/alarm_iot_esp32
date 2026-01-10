#pragma once
#include "UniversalTelegramBot.h"
#include "user_manager.h"
#include "cstdint"
#include "array"
#include "string"

constexpr const uint8_t maxUsers = 4;

enum class TelegramStates : uint8_t{
    idle,
    handleMessages,
    waitingResponse,
    erro,
};

class SystemControl {
private:
    std::array <user,maxUsers> _users;
    UniversalTelegramBot& _TelegramBot;
    TelegramStates _State;
    uint16_t _newMessages;
public:
    //Constructor
    SystemControl(UniversalTelegramBot& TelegramBot);

    void HandleMessages(uint16_t numMessages);

    void TelegramManager();

    bool CheckAlarms(struct tm& timeNow);

    int8_t findUserId(const char* id) const;

};