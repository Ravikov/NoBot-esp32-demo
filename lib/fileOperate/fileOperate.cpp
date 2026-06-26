#include <Arduino.h>
#include <SPIFFS.h>
#include <fileOperate.h>

FileOperate::FileOperate(const char* path)
    : _path(path){}

void FileOperate::setUp(){
    if (!SPIFFS.begin(true)){
        Serial.println("文件系统挂载失败");
        return;
    }
    else{
        Serial.println("SPIFFS文件系统挂载成功");
    }
}

String FileOperate::read(){
    File f = SPIFFS.open(_path, FILE_READ);
    
    if (f){
        String content = f.readString();
        Serial.println("文件读取成功");
        Serial.println(content);
        return content;
    }
    Serial.println("文件读取失败");
    return "";
}
