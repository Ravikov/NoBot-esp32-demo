#ifndef FILE_OPERATE_H
#define FILE_OPERATE_H

#include <Arduino.h>

class FileOperate{

private:
    const char* _path;
    
public:
    void setUp();
    FileOperate(const char* path);
    String read();
};

#endif