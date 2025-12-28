#include "../include/user_manager.h"

user usuarios[4];

int main(){

    for(const auto& usuario : usuarios){
        std::cout << usuario.getName() << " " << usuario.getId()<< std::endl;
    }

    std::cout << usuarios[0].getAlarms() << std::endl;

    return 0;
}