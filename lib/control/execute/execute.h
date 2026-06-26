#ifndef EXECUTE_H
#define EXECUTE_H


// 在actionAndHardware.txt内写入action和hardware枚举内容 顺序须一致
enum Action{
    LED_GLOW,
    LED_OUT,
    LED_BLINK
};
enum Hardware{
    RED_LED,
    GREEN_LED,
    WHITE_LED
};

// 硬件列表 各硬件gpio针脚 顺序必须与上hardware枚举顺序一致
const int hardware_list[] = {13, 18, 17};

class Executer{
public:
    void run();
    Executer(int action, int hardware);
private:
    int _action;
    int _hardware;
};

#endif