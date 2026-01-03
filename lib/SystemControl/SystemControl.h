#include "bot.h"
#include "user_manager.h"
#include "secure.h"

#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

enum class SystemStates : int8_t{
    idle,
    newMessage,
    handlingMessage,
    error,
};

class SystemControl {
private:
    user _Users[4];
    bot& _TelegramBot;
public:
    //builder
    SystemControl(bot& TelegramBot);

    void HandleMessages();
};


