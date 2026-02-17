#include "buzzer.hpp"

//constructor
//parameters pin, freq, channel = 0 , resolution(bits) = 8
buzzer::buzzer(uint8_t pin, uint16_t freq, uint8_t channel, uint8_t resolution)
:_buzzer_pin(pin), _state(false),_freq(freq),_channel(channel),_resolution(resolution)
{
    //config LedC
    ledcSetup(_channel,_freq,_resolution);

    //init
    ledcAttachPin(_buzzer_pin,channel);
}

//Pulse Buzzer
//paramenter: PulseTime and dutyCicle
void buzzer::pulse(uint32_t pulseTime, uint16_t dutyCicle){
    if (millis() - _previousPulseTime >= pulseTime)
    {
        if (!_state){
            ledcWrite(_channel, dutyCicle); //on buzzer
            _state = true;   
        }

        else{
            ledcWrite(_channel, 0); //off buzzer
            _state = false;
        }

        _previousPulseTime = millis();
    }
}

//ON Buzzer
void buzzer::on(uint16_t dutyCicle){
    ledcWrite(_channel,dutyCicle);
    _state = true;
}

//OFF Buzzer
void buzzer::off(){
    ledcWrite(_channel,0);
    _state= false;
}