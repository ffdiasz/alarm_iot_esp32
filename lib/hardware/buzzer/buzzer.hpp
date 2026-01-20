#pragma once
#include "Arduino.h"
#include "cstdint"

class buzzer {
private:
//configs
uint8_t _buzzer_pin;
uint16_t _freq;
uint8_t _channel; // 0 default
uint8_t _resolution; // 8bits default
bool _state;

//previous time
uint32_t _previousPulseTime;

public:
//constructor
buzzer(uint8_t pin,uint16_t freq,uint8_t channel = 0, uint8_t resolution = 8);

void pulse(uint32_t pulseTime, uint16_t dutyCicle);

//on buzzer
void on(uint16_t dutyCicle);

//off buzzer
void off();
};