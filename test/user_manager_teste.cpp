#include "user_manager.h"
#include "secure.h"
#include <iostream>

user usuarios[4];

int main(){

    usuarios[0].setName("Felipe");
    
    for(const auto& usuario : usuarios){
        std::cout << usuario.getName() << " " << usuario.getId()<< std::endl;
    }

    std::cout << usuarios[0].getAlarms() << std::endl;
    return 0;
}