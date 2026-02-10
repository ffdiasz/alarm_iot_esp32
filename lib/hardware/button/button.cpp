#include "button.h"

//inicialization of static members
volatile bool button::_state = false;

//buttonPin: digital pin to attach the interrupt
button :: button(uint8_t buttonPin):_buttonPin(buttonPin){}

/*
pullResistor: select the pull resistor type using TRUE to pull_up or FALSE to pull_down*

mode: select the sinal edge, CHANGE,RISING or FALLING 
*/
void button::begin(bool pullResistorType, int mode){
    //if mode is wrong break system
    if (mode != RISING && mode != FALLING && mode != CHANGE){
        Serial.println("ERROR: Mode is wrong, expected 'RISING', 'FALLING' or 'CHANGE'");

        while(1){delay(100);} //watchdog time will reset system here
    }

    pinMode(_buttonPin, (pullResistorType ? INPUT_PULLUP : INPUT_PULLDOWN));
    attachInterrupt(digitalPinToInterrupt(_buttonPin),ISR, mode); 
}

bool button:: wasClicked(){
    if(_state){
        _state = false;
        return true;
    }
    
    return false;
}

void IRAM_ATTR button:: ISR(){
    _state = true;
}