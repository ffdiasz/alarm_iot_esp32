#pragma once
#include "UniversalTelegramBot.h";
#include "WiFiClientSecure.h";
#include "secure.h";

//Messages receive from Telegram
enum class TelegramCommands : uint8_t{
    start,
    showAlarms,
    configAlarm,
    help,
    unknown,
};

class Telegram_Manager{

private:
    WiFiClientSecure secured_client;
    UniversalTelegramBot _TelegramBot;

    uint32_t previousTime;

public:

    Telegram_Manager();

    //add messages to queue
    bool addMessage();

    //check queue and send messages
    bool checkNewMessagesToSent();

    //sendMessages 
    bool sendMessage(const char messages, uint32_t cooldown);

    //get commands from telegram 
    //parse to TelegramCommands type to be process by MachineState
    TelegramCommands getCommand(); 

};