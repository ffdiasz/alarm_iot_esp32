#include "user_manager.h"
#include <iostream>
#include <time.h>

std::string name = "Felipe";
std::string id = "27032006DIAS";

//test time
struct tm timeNow;

user usuarios[4];


int main(){
    //make a test time
    timeNow.tm_hour = 0;
    timeNow.tm_min = 0;

    usuarios[0].setName(name);
    usuarios[0].setId(id);
    usuarios[0].setState(true);

    //testing the getters
    for (const auto& user : usuarios){

        //pass inacive users
        if (!user.isActive()){
            continue;
        }

        std::string exibir;
        exibir = "name: " + user.getName() + "\n";
        exibir += "id: " + user.getId() + "\n";
        exibir += user.getAlarms();

        std::cout << exibir << std::endl;
    }

    //testing if alarms will be triggered
    //its necessary change state to TRUE in alarm_manager constructor
    if (usuarios[0].checkAlarms(timeNow)){
        std::cout << "ALARME!" << std::endl;
    }
    else{
        std::cout << "Everthing Normal" << std::endl;
    }

}