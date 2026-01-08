#include <alarm_manager.h>
#include <iostream>

Alarm meusAlarmes[5];

int main(){
    std::string label = "trabaia ne?";
    meusAlarmes[1].setLabel(label);
    meusAlarmes[1].setTime(23,04);
    meusAlarmes[1].setState(true);

    time_t time0;
    struct tm* timeTM;

    for (const auto& alarm : meusAlarmes){
        std::cout << alarm.getStatus() << std::endl;
        time0 = time(NULL);
        timeTM = localtime(&time0);
        bool state = alarm.getState();
        bool check = alarm.checkTime(*timeTM);

        if (state && check){
            std::cout << "ALARME!" <<std::endl;
        }

        std::cout << "--------------------------------------" << std::endl;
    }
}