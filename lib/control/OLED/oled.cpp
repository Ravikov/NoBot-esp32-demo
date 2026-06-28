#define SSD1306_NO_SPLASH
#include "oled.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "fliker.h"

Adafruit_SSD1306 display(oled_screen.width, oled_screen.height, &Wire, -1);

void oledInit(){
    Wire.begin(oled_screen.sda_pin, oled_screen.scl_pin);
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)){
        Serial.println("失败");
        flicker_many_time(red_led, 300, 5);
        return;
    }
}

void oledPrint(const char* msg, int size, int x, int y, bool clear, bool clear_self){
    
    if (clear){
        display.clearDisplay();
    }
    else{
        display.fillRect(x,y,oled_screen.width,12*size,SSD1306_BLACK);
    }
    display.setTextSize(size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x,y);
    display.println(msg);
    display.display();
    if (!clear && clear_self){
        display.fillRect(x,y,oled_screen.width,8*size,SSD1306_BLACK);
    }
}

void oledClear(){
    display.clearDisplay();
    display.display();
}

void oledPrintTip(const char* msg,bool clear_self){
    oledPrint(msg,1,0,56,false,clear_self);
}