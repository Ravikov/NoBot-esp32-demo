#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <ArduinoJson.h>
#include <Arduino.h>

class CommandHandler {
public:
    // 构造函数：可传入 Motor 对象的引用
    CommandHandler();

    // 处理 JSON 指令
    void handle(const String& jsonString);
};

#endif