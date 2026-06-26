#include <Arduino.h>
#include "fliker.h"

void flicker_once(int pin, int delay_time){

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delay(delay_time);
    digitalWrite(pin, LOW);
    delay(delay_time);

}

void flicker_many_time(int pin, int delay_time, int flicker_time){

    for (int i=flicker_time;i>0;i--){
        flicker_once(pin, delay_time);
    }

}