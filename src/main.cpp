#include <Arduino.h>
#include "wifiConnect.h"
#include "fliker.h"
#include "webSocketManager.h"
#include "../lib/config.h"
#include "fileOperate.h"
#include "OLED/oled.h"

#include <SPIFFS.h>

WsCls wsRunner;
boolean is_ready = false;

void setup() {
  // 初始化串口
  Serial.begin(115200);

  oledPrint("starting...", 1);

  // 初始化文件系统
  FileOperate* f = new FileOperate("");
  f->setUp();
  delete f;
  
  flicker_once(white_led, 1000);
  flicker_once(red_led, 1000);
  flicker_once(green_led, 1000);
  flicker_once(yellow_led, 1000);

  Serial.println("=== 文件系统诊断 ===");

  if (!SPIFFS.begin(true)) {
      Serial.println("SPIFFS 挂载失败");
      return;
  }

  File root = SPIFFS.open("/");
  Serial.println("根目录文件列表:");
  File file = root.openNextFile();
  while (file) {
      Serial.printf("  %s (大小: %d 字节)\n", file.name(), file.size());
      file = root.openNextFile();
  }
  root.close();

  Serial.println("=== 诊断结束 ===");

  Serial.println("starting...");

  pinMode(48, OUTPUT);
  digitalWrite(48, LOW);

  oledPrint("Connecting WiFi...");
  connectWifi(wifi_ssid, wifi_password, green_led);
  delay(300);

  oledPrint("Connecting websocket...");
}

void loop() {
  wsRunner.webSocketRun();
  if (!is_ready){
    oledPrint("NoBot-Esp32 now ready");
    delay(2000);
    oledPrint("NoBot!", 3, 16, 18);
    is_ready = true;
  }
  delay(10);  // 仅延迟10ms，确保 loop() 高频调用
}
