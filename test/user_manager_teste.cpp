#include "../include/user_manager.h"
#include "../include/secure.h"

user usuarios[4];

int main(){

    usuarios[0].setName("Felipe");
    
    for(const auto& usuario : usuarios){
        std::cout << usuario.getName() << " " << usuario.getId()<< std::endl;
    }

    std::cout << usuarios[0].getAlarms() << std::endl;
    return 0;
}