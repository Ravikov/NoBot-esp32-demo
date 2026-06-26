#ifndef CONFIG_H
#define CONFIG_H

// 引脚配置（常量，可直接在头文件中定义）
extern const int red_led;
extern const int green_led;
extern const int white_led;

// WiFi 配置（字符串，用 extern 声明）
extern const char* wifi_ssid;
extern const char* wifi_password;

// WebSocket 配置
extern const char* ws_uri;
extern const int ws_port;
extern const char* ws_path;

#endif
