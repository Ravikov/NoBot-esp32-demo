#include <Arduino.h>
#include <WiFi.h>
#include "wifiConnect.h"
#include "fliker.h"

void connectWifi(const char* ssid,const char*pswd,int led_pin){

    // put your setup code here, to run once:
    pinMode(led_pin, OUTPUT);
    Serial.print("尝试连接: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pswd);
    while (WiFi.status() != WL_CONNECTED){
        if (WiFi.status() == WL_CONNECT_FAILED){
            Serial.println("");
            Serial.println("连接失败");
            return;
        }
        Serial.printf(".");
        delay(500);
    }
    flicker_many_time(led_pin, 1000, 3);
    flicker_many_time(led_pin, 200, 5);
    Serial.println("");
    Serial.printf("连接成功,本机IP: ");
    Serial.println(WiFi.localIP());

}