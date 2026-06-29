#include "commandHandler.h"
#include "execute/execute.h"
#include <FreeRTOS.h>

CommandHandler::CommandHandler(){}
DynamicJsonDocument doc(1024);
void start();
SemaphoreHandle_t doneSemaphore = xSemaphoreCreateBinary();

void CommandHandler::handle(const String& jsonString){
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

        doc["delay"] = doc.containsKey("delay") ? doc["delay"] : 0;
        Serial.printf("延迟: ");
        Serial.println((const char*)doc["delay"]);
        start();
        if (doc["delay"] != 0){
            if (doc["delay"] == -1){
                xSemaphoreTake(doneSemaphore, portMAX_DELAY);
            }
            else{
                delay(1000*(int)doc["delay"]);
            }
        }
    }
}

void task(void* param){
    Executer ex(doc["action"], doc["hardware"], doc["msg"] | "", doc["show"] | "");
    ex.run();
    xSemaphoreGive(doneSemaphore);
    vTaskDelete(NULL);
}

void start(){
    delay(50);
    xTaskCreate(task,"run",4096,NULL,2,NULL);
}