#include <Arduino.h>
#include <WiFi.h>
#include "wifiConnect.h"
#include "fliker.h"
#include "config.h"
#include "OLED/oled.h"

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
            flicker_many_time(yellow_led, 200, 20);
            return;
        }
        Serial.printf(".");
        delay(500);
    }
    char w[64];
    snprintf(w,64,"WiFi:%s",WiFi.localIP().toString());
    oledPrint(w,1,0,0,false);
    flicker_many_time(led_pin, 1000, 2);
    flicker_many_time(led_pin, 200, 3);
    Serial.println("");
    Serial.printf("连接成功,本机IP: ");
    Serial.println(WiFi.localIP());
}