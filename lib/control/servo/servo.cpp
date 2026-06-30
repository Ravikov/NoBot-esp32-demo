#include <Arduino.h>
#include <ESP32Servo.h>
#include "config.h"

Servo servo_runner;

void servoInit(){
    servo_runner.attach(servo_pin);
    servo_runner.write(0);
}

void servoRun(int angle){
    servo_runner.write(angle);
    Serial.printf("舵机旋转: ");
    Serial.println(angle);
}