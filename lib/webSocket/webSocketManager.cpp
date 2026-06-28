#include <WebSocketsClient.h>
#include "webSocketManager.h"
#include "fliker.h"
#include "../lib/config.h"
#include "commandHandler/commandHandler.h"
#include "../fileOperate/fileOperate.h"
#include "OLED/oled.h"

String action_table;
String action_table_msg;

WsCls::WsCls()
    : _is_connecting(false), _fragment_cache(""), _last_connect_attempt(0){

        _ws_obj.onEvent([this](WStype_t type, uint8_t* payload, size_t length){
                _webSocketEvent(type, payload, length);
        });
        _uri      = ws_uri;
        _port     = ws_port;
        _web_path = ws_path;
    }
    

void WsCls::_webSocketEvent(WStype_t type, uint8_t* payload, size_t length){
    Serial.printf("事件类型: ");
    Serial.println(type);

    switch (type){
        case WStype_CONNECTED:
        {
            Serial.println("ws连接成功");
            oledPrint("WS:Connected",1,0,8,false,false);
            flicker_many_time(green_led, 500, 3);
            _is_connecting = false;
            // 连接建立后再发消息
            // 先读取文件获取动作表
            FileOperate action_file("/actionAndHardware.txt");
            action_table = action_file.read();
            action_table_msg = "{\"msg\":\"" + action_table + "\",\"type\":100}";
            _ws_obj.sendTXT(action_table_msg);
            _ws_obj.loop();
            oledPrintTip("Ready.",true);
            delay(1500);
            break;
        }
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT:
        {
            Serial.println("分块消息,缓存处理...");
            _fragment_cache.concat((char*)payload);
            Serial.printf("本条消息:");
            Serial.println((char*)payload);
            CommandHandler handler;
            handler.handle(_fragment_cache);
            _fragment_cache = "";
            break;
        }
        case WStype_FRAGMENT_FIN:
        {   
            if ((char*)payload != ""){
                Serial.printf("最后一条分块消息:");
                Serial.println((char*)payload);
                _fragment_cache.concat((char*)payload);
                Serial.printf("缓存长度: ");
                Serial.println(_fragment_cache.length());
                Serial.printf("消息内容: ");
                Serial.println(_fragment_cache);
                // 这里消息处理
                CommandHandler handler;
                handler.handle(_fragment_cache);
            }
            break;
        }
        case WStype_TEXT:
        {
            Serial.printf("收到消息: ");
            Serial.println((char*)payload);
            flicker_once(green_led, 500);
            flicker_once(white_led, 300);
            // 这里可以放消息处理
            CommandHandler handler;
            handler.handle(String((char*)payload));
            break;
        }
        case WStype_DISCONNECTED:
        {
            Serial.println("ws连接断开，尝试重连...");
            oledPrint("WS:DisConnected",1,0,8,false,false);
            oledPrintTip("Waiting for ws.");
            flicker_many_time(red_led, 500, 3);
            _is_connecting = false;  // 允许下次重试
            break;
        }
        case WStype_ERROR:
        {
            Serial.println("ws连接出错");
            flicker_many_time(red_led, 500, 3);
            _is_connecting = false;  // 允许下次重试
            break;
        }
        default:{
            break;}
    }

}

void WsCls::webSocketRun(){

    if (! _ws_obj.isConnected()){
        // 未连接且不在连接中 → 发起连接
        if (!_is_connecting) {
            Serial.println("尝试连接到ws服务端...");
            _is_connecting = true;
            _ws_obj.begin(_uri, _port, _web_path);
        }
    } else {
        // 已连接 → 重置状态
        _is_connecting = false;
    }
    
    // 每次必须频繁调用 loop() 处理协议事件
    _ws_obj.loop();

}
