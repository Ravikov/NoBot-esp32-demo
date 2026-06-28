#ifndef EXECUTE_H
#define EXECUTE_H

#include "config.h"

// 在actionAndHardware.txt内写入action和hardware枚举内容 顺序须一致
enum Action{
    LED_GLOW,
    LED_OUT,
    LED_BLINK
};

// 当前顺序
// RED
// GREEN
// WHITE
// YELLOW
// const int hardware_list[] = {13, 18, 17, 40};
// 硬件列表 各硬件gpio针脚 顺序必须与 data/actionAndHardware 顺序一致(可以记在上面)
const int hardware_list[] = {red_led, green_led, white_led, yellow_led};

class Executer{
public:
    void run();
    Executer(int action, int hardware, const char* msg, const char* show);
private:
    int _action;
    int _hardware;
    const char* _msg;
    const char* _show;
};

#endif