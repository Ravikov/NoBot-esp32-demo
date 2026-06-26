#ifndef WEB_SOCKET_MANAGER
#define WEB_SOCKET_MANAGER

#include <WebSocketsClient.h>
#include <Arduino.h>

extern String action_table;
extern String action_table_msg;

class WsCls{

public:
    WsCls();
    void webSocketRun();

private:
    WebSocketsClient _ws_obj;
    const char* _uri;
    const char* _web_path;
    bool _is_connecting;
    String _fragment_cache;
    int _port;

    void _webSocketEvent(WStype_t type, uint8_t* payload, size_t length);
};

#endif