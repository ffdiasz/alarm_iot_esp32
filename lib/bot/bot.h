#pragma once
#include "UniversalTelegramBot.h"
#include "WiFiClientSecure.h"
#include "user_manager.h"

class bot{
private:
    //WiFi includes
    WiFiClientSecure secured_client;
    UniversalTelegramBot Telegram_bot;
    //Intervalos
    uint32_t bot_lasttime;

public:
    //builder
    bot(const char* token);

    //execute the commands
    void HandleNewMessages(uint16_t NewMessages);

    //Check New Messages
    void check_telegram(uint32_t interval);

    //get users
    void getUsers() const;
};