#include "oled.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "fliker.h"

void oledPrint(const char* msg){
    Adafruit_SSD1306 display(oled_screen.width, oled_screen.height, &Wire, -1);
    Wire.begin(oled_screen.sda_pin, oled_screen.scl_pin);
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)){
        Serial.println("失败");
        flicker_many_time(red_led, 300, 5);
        return;
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println(msg);
    display.display();
}