#pragma once
#include <Arduino.h>
#include <cstdint>

class button{
private:
    const uint8_t _buttonPin;
    static volatile bool _state;

    static void IRAM_ATTR ISR();
    
public:
    /*Note: pullResistor functions is select the pull resistor type
    use TRUE to pull_up and FALSE to pull_down*/
    button(uint8_t buttonPin);

    void begin(bool pullResistorType, int mode);
    bool wasClicked();
};