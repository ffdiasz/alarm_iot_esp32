#include <bot.h>

//builder, inicializa secured_client e telegram_bot
bot::bot(const char* token)
:secured_client(), Telegram_bot(token,secured_client)
{
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
}

void bot::HandleNewMessages(uint16_t newMessages){
    Serial.print("handleNewMessages ");
    Serial.println(newMessages);

    for (int i = 0; i < newMessages; i++){
        String chat_id = Telegram_bot.messages[i].chat_id;
        String text = Telegram_bot.messages[i].text;

        String from_name = Telegram_bot.messages[i].from_name;
        if (from_name == "")
        from_name = "Guest";
    
    if(text == "/start"){
        String Welcome = "MAMBA INICIADO COM SUCESSO! \n";
        Welcome+= "principais comandos:\n";
        Welcome+="1-\n";
        Welcome+="2-\n";
        Welcome+="3-\n";
        Telegram_bot.sendMessage(chat_id,Welcome);
    }
    if(text == "/Novo_Usuario"){
           
    }

    }
}

//Verifica Novos comandos e realiza as ações caso tenha algum
void bot::check_telegram(uint32_t bot_interval){
    if(millis() - bot_lasttime > bot_interval){
        int numMessages = Telegram_bot.getUpdates(Telegram_bot.last_message_received +1);

        while(numMessages){
            Serial.println("got response");
            HandleNewMessages(numMessages);
            numMessages = Telegram_bot.getUpdates(Telegram_bot.last_message_received +1);
        }
        
        bot_lasttime = millis();
    }
}