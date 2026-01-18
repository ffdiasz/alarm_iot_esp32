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

MachineState SystemControl::HandleMessages(const char* id, TelegramCommands command){

    switch (command){

        case TelegramCommands::start:
        {
            Serial.println("/start");
            const char* messageSend = "AlarmClockBot begin";
            _TelegramBot.sendMessage(id,messageSend);
            
            return MachineState::sucess;

        }

        case TelegramCommands::showAlarms:
        {
            Serial.println("/ShowAlarms");
            uint8_t index = findUserId(_LastUserID); //get user index
            std::string message = _users[index].getAlarms();//get alarms

            _TelegramBot.sendMessage(_LastUserID,message.c_str(), "Markdown");

            return MachineState::sucess;
        }

        case TelegramCommands::configAlarm: //state machine to configAlarms
        {
            Serial.println("/configAlarm");
            MachineState ConfigState = configAlarm();

            if (ConfigState == MachineState::waiting){
                break;
            }

            else if (ConfigState == MachineState::sucess){
                return MachineState::sucess;
            }

            else{
                return MachineState::erro;
            }

        }

        case TelegramCommands::help:
        {
            Serial.println("/help");
            _TelegramBot.sendMessage(_LastUserID,"/help");
            return MachineState::sucess;
        }

        case TelegramCommands::unknown: //command not found
        {
            const char* message = "command not found, try /help";
            Serial.println(message);
            _TelegramBot.sendMessage(_LastUserID,message);
            
            return MachineState::sucess;
        }

    }

    return MachineState::waiting;
}

//State Machine for manager telegram actions
void SystemControl::TelegramManager(){

    static TelegramCommands LastTelegramMessage;
    static int8_t freeUserIndex;

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

            //get messages
            _State = TelegramStates::getMessage;
            break;
        }
        
        case TelegramStates::checkFreeUser: //if doesn't register
        {
            freeUserIndex = hasFreeUser();

            if(freeUserIndex == -1){
                const char* message = "All users slot is busy";
                Serial.println(message);
                _TelegramBot.sendMessage(_LastUserID,message);

                _State = TelegramStates::idle;
                break;
            }

            else{
                _State = TelegramStates::registerUser;
                break;
            }
        }
        
        case TelegramStates::registerUser: //Register new user
        {
            MachineState registerState = newUser(freeUserIndex);

            if (registerState == MachineState::waiting){
                break;
            }

            //when a new user be registered return to idle state 
            else if (registerState == MachineState::sucess){
                _State = TelegramStates::idle;
                break;
            }

            //error to register user;
            else{
                _TelegramBot.sendMessage(_LastUserID,"erro to register user");

                _State =TelegramStates::idle;
                break;
            }

        }

        case TelegramStates::getMessage: //get messages and convert to state machine
        {
            LastTelegramMessage = getCommand();
            _State = TelegramStates::handle;
            break;
        }

        case TelegramStates::handle: //handle new messages
        {
            MachineState handleState = HandleMessages(_LastUserID, LastTelegramMessage);

            if (handleState == MachineState::waiting){
                break;
            }

            else if (handleState == MachineState::sucess){
                _State = TelegramStates::idle;
                break;
            }

            else{
                _TelegramBot.sendMessage(_LastUserID,"error to handle command");
                
                _State = TelegramStates::idle;
                break;
            }
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
MachineState SystemControl::newUser(uint8_t UserIndex){
    static uint8_t newUserState = 0;
    static const char* username;

    switch(newUserState){
        case 0://Register New User
        {
            const char* message = "Register user";
            Serial.println(message);
            _TelegramBot.sendMessage(_LastUserID,message);
            _TelegramBot.sendMessage(_LastUserID,"Send your username");
            
            newUserState = 1;
            break;
        }

        case 1://Waiting username
        { 
            Serial.println("Waiting username to register user");
            
            if (_TelegramBot.getUpdates(_TelegramBot.last_message_received + 1))
            {
                username = _TelegramBot.messages->text.c_str();
                Serial.print("Username receive: ");
                Serial.println(username);

                newUserState = 2;
                break;
            }

            else //waiting
            {
                break;
            }
        }

        case 2://Config User
        {
            _users[UserIndex].setName(username);
            _users[UserIndex].setId(_LastUserID);
            _users[UserIndex].setState(true);

            _TelegramBot.sendMessage(_LastUserID, "New user registered");
            _TelegramBot.sendMessage(_LastUserID,"Now you can use commands");

            newUserState = 0;
            return MachineState::sucess; //User registered
        }
    }

    return MachineState::waiting;
}

MachineState SystemControl::configAlarm(){
    static uint8_t state = 0;
    static int8_t alarmIndex;
    static uint8_t hour;
    static uint8_t min;
    static std::string label;

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

            else{ //waiting
                break;
            }
        }
        
        case 2: //security check
        {
            if (alarmIndex == -1) //isn't a number
            {
                Serial.println("isn't a number");
                state = 0;
                return MachineState::erro;
            }

            else if (alarmIndex == 0 || alarmIndex > maxSizeOfAlarmsArray) //array overflow
            {
                Serial.println("wrong input, array overflow");
                state = 0;
                return MachineState::erro;
            }
            
            else{
                state = 3;
                break;
            }
        }

        case 3: //hour
        {
            _TelegramBot.sendMessage(_LastUserID,"Send the hour (24h format):");
            state = 4;
            break;
        }

        case 4: //waiting hour
        {
            Serial.println("waiting hour");

            if (_TelegramBot.getUpdates(_TelegramBot.last_message_received + 1)){
                
                Serial.println("hour receive");
                hour = std::atoi(_TelegramBot.messages->text.c_str()); //convert to int
                
                state = 5;
                break;
            }

            else{//waiting
                break;
            }
        }

        case 5: //check hour format
        {
            if (hour >= 0 and hour <=23){
                state = 6;
                break;
            }
            else{ //format error
                state = 0;
                return MachineState::erro;
            }
        }

        case 6: //min
        {
            _TelegramBot.sendMessage(_LastUserID,"Send the minute:");
            state = 7;
            break;
        }

        case 7: //waiting hour
        {
            if (_TelegramBot.getUpdates(_TelegramBot.last_message_received + 1))
            {
                min = std::atoi(_TelegramBot.messages->text.c_str()); //convert to int
                
                state = 8;
                break;
            }

            else{//waiting
                break;
            }
        }

        case 8: //check minute format
        {
            if (min >= 0 and min <= 59){
                state = 9;
                break;
            }
            
            else{
                state = 0;
                return MachineState::erro;
            }
        }

        case 9: //label
        {
            _TelegramBot.sendMessage(_LastUserID,"send a label:");
            state = 10;
            break;
        }

        case 10: //waiting label
        {
            if (_TelegramBot.getUpdates(_TelegramBot.last_message_received + 1)){
                label = _TelegramBot.messages->text.c_str();
                state = 11;
                break;
            }

            else{ //waiting
                break;
            }
        }

        case 11: //configuring alarm
        {
            uint8_t userIndex = findUserId(_LastUserID);
            bool addState = _users[userIndex].addAlarm(alarmIndex-1,hour,min,label.c_str());

            if (addState){
                _TelegramBot.sendMessage(_LastUserID,"alarm added sucessfully");
                state = 0;
                return MachineState::sucess;
            }

            else{
                _TelegramBot.sendMessage(_LastUserID,"error to add Alarm");
                state = 0;
                return MachineState::erro;
            }
            
        }
    }

    return MachineState::waiting;
}

TelegramCommands SystemControl :: getCommand(){
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