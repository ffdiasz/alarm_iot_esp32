#include "SystemControl.h"

//Constructor
SystemControl::SystemControl(UniversalTelegramBot& TelegramBot)
:_TelegramBot(TelegramBot), _State(TelegramStates::idle), _newMessages(0)
{

}

//if user is active and have alarm to ring return true
bool SystemControl::CheckAlarms(struct tm& timeNow){
    for (const auto& user : _users){
        if (user.isActive() && user.checkAlarms(timeNow)){
            return true;
        }
    }

    return false;
}

//Handle new telegram messages
void SystemControl::HandleMessages(const char* id,uint16_t numMessages){

    for (uint16_t i = 0; i < numMessages; i++){
        //Using string& because i need a string to compare 
        const std::string& text = _TelegramBot.messages[i].text.c_str();

        if (text == "")
        {
            Serial.println("void message");
            continue; //pass to the next iteration
        }

        if (text == "/start")
        {
            Serial.println("/start");
            const char* messageSend = "AlarmClockBot begin";
            _TelegramBot.sendMessage(id,messageSend);
        }

        else if (text == "/showAlarms")
        {   
            Serial.println("/ShowAlarms");

            uint8_t index = findUserId(_LastUserID); //get user index
            std::string message = _users[index].getAlarms();//get alarms

            _TelegramBot.sendMessage(_LastUserID,message.c_str(), "Markdown");
        }

        else if (text == "/configAlarm")
        {
            Serial.println("/configAlarm");

            if (configAlarm() == -1){
                Serial.println("ERRO");
            }

            else if (configAlarm() == true){
                Serial.println("SUCESS");
            }

        }
        
        else //command not found
        {   
            const char* message = "command not found, try /help";
            
            Serial.println(message);
            _TelegramBot.sendMessage(_LastUserID,message);
        }
    }
}

//State Machine for manager telegram actions
void SystemControl::TelegramManager(){
    switch (_State){
    
        case TelegramStates::idle: //waiting new messages from telegram
        {
            _newMessages = _TelegramBot.getUpdates(_TelegramBot.last_message_received + 1);
            
            if (_newMessages > 0){
                Serial.print("New messages: ");
                Serial.println(_newMessages);
                _State = TelegramStates::checkUser;
            }
            
            break;
        }
        
        case TelegramStates::checkUser: //Check if use is register
        {
            _LastUserID = _TelegramBot.messages->chat_id.c_str();
            Serial.print("ID: ");
            Serial.println(_LastUserID);

            //if user isn't registered send to RegisterUser
            if (findUserId(_LastUserID) == -1){
                const char* message = "User doesn't register";
                Serial.println(message);
                _TelegramBot.sendMessage(_LastUserID,message);

                _State = TelegramStates::registerUser;
                break;
            }

            //handle messages
            _State = TelegramStates::handle;
            break;
        }
        
        case TelegramStates::registerUser: //Register new user
        {
            int8_t freeUserIndex = hasFreeUser();

            if(freeUserIndex == -1){
                const char* message = "All users is active";
                Serial.println(message);
                _TelegramBot.sendMessage(_LastUserID,message);

                _State = TelegramStates::idle;
                break;
            }

            //when a new user be registered return to idle state 
            if (newUser(freeUserIndex)){
                _State = TelegramStates::idle;
                break;
            }

            //waiting register 
            break;
        }

        case TelegramStates::handle: //handle new messages
        {
            HandleMessages(_LastUserID,_newMessages);
            _State = TelegramStates::idle;
            break;
        }
    }
}

//return user index or -1 if doesn't exists
int8_t SystemControl::findUserId(const char* id) const{
    for (uint8_t i = 0; i < maxUsers; i++){
        if (_users[i].getId() == id){
            return i;
        }
    }
    //user id doesn't exists in array
    return -1;
}

//return user index or -1 if all user status is active
int8_t SystemControl::hasFreeUser() const{
    for (uint8_t i = 0; i < maxUsers; i++){
        if (!_users[i].isActive()){
            return i;
        }
    }
    //all user is active
    return -1;
}

//state machine to register new user, return true when finished
bool SystemControl::newUser(uint8_t UserIndex){
    static uint8_t newUserState = 0;
    static const char* username;

    switch(newUserState){
        case 0://Register New User
        {
            const char* message0 = "Register user";
            Serial.println(message0);
            _TelegramBot.sendMessage(_LastUserID,message0);
            _TelegramBot.sendMessage(_LastUserID,"Send your username");
            
            newUserState = 1;
            break;
        }

        case 1://Waiting username
        { 
            Serial.println("Waiting username to register user");
            
            if (_TelegramBot.getUpdates(_TelegramBot.last_message_received + 1)){
                username = _TelegramBot.messages->text.c_str();
                Serial.print("Username receive: ");
                Serial.println(username);

                newUserState = 2;
                break;
            }
            
            //No response
            break;
        }

        case 2://Config User
        {
            _users[UserIndex].setName(username);
            _users[UserIndex].setId(_LastUserID);
            _users[UserIndex].setState(true);

            _TelegramBot.sendMessage(_LastUserID, "New user registered");
            _TelegramBot.sendMessage(_LastUserID,"Now you can use commands");

            newUserState = 0;
            return true; //User registered
        }
    }

    //in progress
    return false;
}

int8_t SystemControl::configAlarm(){
    static uint8_t state = 0;
    static int8_t alarmIndex;

    switch(state){
        
        case 0: //showAlarms
        {   
            _TelegramBot.sendMessage(_LastUserID, "Which alarm you want config? send the line number");
            uint8_t index = findUserId(_LastUserID); //get user index
            std::string message = _users[index].getAlarms();//get alarms

            _TelegramBot.sendMessage(_LastUserID,message.c_str(), "Markdown");

            state = 1;
            break;
        }

        case 1: //waiting alarm index
        {
            if (_TelegramBot.getUpdates(_TelegramBot.last_message_received + 1))
            {
                //receive msg from telegram and convert to int
                const char* msg = _TelegramBot.messages->text.c_str();
                alarmIndex = std::atoi(msg); //convert string to int

                Serial.println("String converted");

                state = 2;
                break;
            }

            //no messages
            break;
        }
        
        case 2: //check if input is a number
        {
            if (alarmIndex != 0){

                Serial.println("IS A NUMBER");
                state = 3;
                break;
            }

            //isn't a number
            Serial.println("IS NOT A NUMBER");
            state = 4;
        }

        case 3: //security check to prevent overflow
        {
            if (alarmIndex > 0 && alarmIndex <= maxSizeOfAlarmsArray)
            {
                Serial.println("input correct");
                state = 0;
                return true;
            }

            else //incorrect input, alarm doesn't exist
            {
                Serial.println("buffer overflow");
                state = 4;
            }
        }

        case 4: //error
        {
            state = 0;
            return -1;
        }
    }

    return false;
}