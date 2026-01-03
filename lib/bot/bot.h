#pragma once
#include "UniversalTelegramBot.h"
#include "WiFiClientSecure.h"
#include <vector>

struct Message{
    String id;
    String text;
};

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

    //Send a message to User using the Telegram Bot
    //Parameter is a message type (chat id and message to be sent)
    void sendMessage(Message* messageToSent);

    //return the number of New Messages 
    uint16_t CheckUpdates();

    //return a vector of the type Message
    //newMessages is the parameter used to reserve space
    std::vector<Message> bot::getMessages(uint16_t newMessages);

    //execute the commands
    void HandleNewMessages(uint16_t NewMessages);

    //Check New Messages
    void check_telegram(uint32_t interval);
};