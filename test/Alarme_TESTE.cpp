#include "../include/alarm_manager.h"

Alarm meusAlarmes[5];

int main(){
    for (const auto& alarme : meusAlarmes){
        std::cout << alarme.getStatus() << std::endl;
    }
}