#include <Arduino.h>
#include "execute.h"
#include "fliker.h"
#include "OLED/oled.h"

Executer::Executer(int action, int hardware, const char* msg, const char* show)
    : _action(action),_hardware(hardware),_msg(msg),_show(show){}

void Executer::run(){
    Serial.printf("收到动作指令,动作: ");
    Serial.println(_action);
    Serial.printf("硬件: ");
    Serial.println(_hardware);
    if (_show != ""){
        if (strcmp(_show, "off") == 0){
            oledClear();
        }
        else{
            oledPrint(_show,1,0,48,false,true);
        }
    }
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