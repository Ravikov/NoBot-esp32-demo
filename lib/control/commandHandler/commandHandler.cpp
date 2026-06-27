#include "commandHandler.h"
#include "execute/execute.h"

CommandHandler::CommandHandler() {}

void CommandHandler::handle(const String& jsonString){
    DynamicJsonDocument doc(1024);
    Serial.printf("json解析: ");
    Serial.println(jsonString);
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error) {
        Serial.print("JSON 解析失败: ");
        Serial.println(error.c_str());
        return;
    }
    else{
        Serial.printf("json解析成功");

        // 打印整个 JSON 对象
        Serial.println("=== 完整的 JSON 文档 ===");
        serializeJson(doc, Serial);
        Serial.println();
        Serial.println("========================");

        const char* msg = doc["msg"] | "";
        Serial.println(msg);

        Executer ex(doc["action"], doc["hardware"], doc["msg"] | "");
        ex.run();
    }
}