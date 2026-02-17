#pragma once
#include "telegram_manager.h"

Telegram_Manager::Telegram_Manager()
:_TelegramBot(Secure::BOT_TOKEN, secured_client),previousTime(0)
{
}

TelegramCommands Telegram_Manager::getCommand(){
    std::string text = _TelegramBot.messages->text.c_str();
    
    // "/start"
    if (text == "/start") {return TelegramCommands::start; }
    // "/showAlarms"
    if (text == "/showAlarms") {return TelegramCommands::showAlarms; }
    // "/configAlarm"
    if (text == "/configAlarm") {return TelegramCommands::configAlarm; }
    // "/help"
    if (text == "/help") {return TelegramCommands::help; }
    // command not found
    else {return TelegramCommands::unknown;}
}