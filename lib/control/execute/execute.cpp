#include <Arduino.h>
#include "execute.h"
#include "fliker.h"

Executer::Executer(int action, int hardware)
    : _action(action),_hardware(hardware){}

void Executer::run(){
    Serial.printf("收到动作指令,动作: ");
    Serial.println(_action);
    Serial.printf("硬件: ");
    Serial.println(_hardware);
    switch (_action)
    {
    case LED_GLOW:
        pinMode(hardware_list[_hardware], OUTPUT);
        digitalWrite(hardware_list[_hardware], HIGH);
        break;
    case LED_OUT:
        pinMode(hardware_list[_hardware], OUTPUT);
        digitalWrite(hardware_list[_hardware], LOW);
        break;
    case LED_BLINK:
        flicker_many_time(hardware_list[_hardware], 300, 5);
        break;
    
    default:
        break;
    }
}