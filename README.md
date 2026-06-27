# NoBot-Body — ESP32-S3 躯体控制固件

基于 PlatformIO + Arduino 框架的 ESP32-S3 固件，通过 WebSocket 接收远程指令，控制GPIO设备执行动作。

> 本项目可选择配合 [github/Ravikov/NoBot](https://github.com/Ravikov/NoBot) 使用，实现更便捷的控制。

## 硬件需求(可自行调整)

- **开发板**: ESP32-S3-DevKitC-1 (N8, 8MB Flash)
- **外设**: 3 个 LED（红、绿、白），通过 GPIO 控制

> 引脚定义在 `src/config.cpp` 中，可按需修改。

## 快速开始

### 1. 安装 PlatformIO

```bash
pip install platformio
```

### 2. 克隆本仓库

```bash
git clone https://github.com/Ravikov/NoBot-esp32-demo
cd NoBot-esp32-demo
```

### 3. 配置 WiFi 和 WebSocket

> `config.cpp` 包含 WiFi 密码等敏感信息，已加入 `.gitignore`，不会被提交到仓库。

创建 `src/config.cpp`，填入你的配置信息：

```cpp
#include "config.h"

// 引脚定义(可自行配置)
const int red_led = 13;
const int green_led = 18;
const int white_led = 17;
const int yellow_led = 40;

// WiFi 配置
const char* wifi_ssid = "360WiFi-2CFF";
const char* wifi_password = "123456789";

// WebSocket 配置
const char* ws_uri = "192.168.0.xxx";
const int ws_port = 8888;
const char* ws_path = "/";

// 初始化 OLED 配置(依次是sda引脚,scl引脚,宽度,高度)
oledConfig oled_screen = { 12, 46, 128, 64 };
// ADDR请于lib/config.h配置,默认0x3c,兼容一般的esp32
```

### 4. 编译并烧录

```bash
pio run --target upload
```

首次编译会自动下载依赖（ArduinoJson、WebSocketsClient 等）。

### 5. 上传动作表文件系统数据

```bash
pio run -t uploadfs
```

`data/actionAndHardware.txt` 定义了动作和硬件的枚举映射，固件启动时会挂载 SPIFFS 读取,并发送给ws服务端,服务端便可得到动作与硬件的配置信息,**务必要上传,否则服务端无法获取可用动作与硬件**.

## 备注
1. 由于我所用的劣质开发板板载LED(GPIO48)不太稳定,所以我在`src/main.cpp`写了上电默认拉低,各位可自行调整
2. 项目自带动作有: 四色LED亮起,熄灭闪烁. OLED屏幕(128*64)显示,`lib/execute.cpp`内可自行编写更多动作

## 工作原理


```
远程服务器 (WebSocket)
        ↕  JSON 指令
   ┌───────────────┐
   │  ESP32-S3     │
   │  NoBot-esp32  │
   │               │
   │  WiFi ←→ 指令 │
   │  解析 → 执行   │
   └───────┬───────┘
           ↓
        各类执行器
```

1. 上电后依次闪烁 LED 自检
2. 自动连接 WiFi（绿色 LED 闪烁表示联网中, OLED屏幕也会显示connecting）
3. 连接 WebSocket 服务器, 发送动作表, 等待指令
4. 收到 JSON 指令后解析 `action` + `hardware`，驱动对应 LED

## WebSocket 协议

### 指令格式 (JSON)
包含硬件与动作的编号,具体枚举信息在`lib/control/execute/execute.h`内

```json
{
  "hardware": 0,
  "action": 0
}
```

### 动作 (Action)

| 枚举值 | 效果 |
|---|---|
| `LED_GLOW` | 点亮 |
| `LED_OUT` | 熄灭 |
| `LED_BLINK` | 闪烁 |

### 硬件 (Hardware)

| 枚举值 | 对应引脚(示例) |
|---|---|
| `RED_LED` | GPIO 13 |
| `GREEN_LED` | GPIO 18 |
| `WHITE_LED` | GPIO 17 |
| `YELLOW_LED` | GPIO 40 |

> 枚举定义在 `lib/control/execute/execute.h`，与 `data/actionAndHardware.txt` 中的顺序需保持一致。

## 项目结构

```
├── data/
│   └── actionAndHardware.txt   # 动作/硬件枚举映射表,每次与服务端建立连接时向服务端发送
├── lib/
│   ├── config.h                # 全局配置头文件声明
│   ├── control/
│   │   ├── commandHandler/     # JSON 指令解析
│   │   └── execute/           # 动作执行（Action/Hardware 枚举）
│   ├── fileOperate/           # SPIFFS 文件读写
│   ├── flicker/               # LED 闪烁工具函数
│   ├── webSocket/             # WebSocket 客户端管理
│   └── WiFi/                  # WiFi 连接管理
├── src/
│   ├── main.cpp               # 入口（setup/loop）
│   └── config.cpp             # 引脚、WiFi、WebSocket 配置值
├── platformio.ini             # PlatformIO 项目配置
└── README.md
```

## 自定义扩展

本项目设计为高可调性固件，各模块均可按需修改，不局限于现有的 LED 与 OLED屏幕 。

### 增加更多 LED / 硬件

1. 在 `hardware_list[]` 数组中按**相同顺序**填写对应 GPIO 引脚：
   ```cpp
   const int hardware_list[] = {13, 18, 17, 19, 27};
   ```
2. 在 `data/actionAndHardware.txt` 中添加对应条目：
   ```
   hardware:
       ...
       BLUE_LED   # 蓝色led
       BUZZER     # 蜂鸣器
   ```

> 只要保持枚举 / `hardware_list[]` / `data/actionAndHardware.txt` 三者顺序一致，一般任意数量的硬件均可正常工作。

### 增加新的动作类型

在 `lib/control/execute/execute.h` 的 `Action` 枚举中添加：
```cpp
enum Action {
    LED_GLOW,
    LED_OUT,
    LED_BLINK,
    LED_BREATHING,   // 新增：呼吸灯
    MOTOR_RUN        // 新增：电机转动
};
```
然后在 `lib/control/execute/execute.cpp` 中实现对应逻辑。

>硬件与动作添加新项目后都需要更新`data/actionAndHardware.txt`并**再次烧录文件**:
```bash
pio run -t uploadfs
```

### 自定义 WebSocket 指令协议

**此项有点吃技术**

在 `lib/control/commandHandler/commandHandler.cpp` 中修改 `handle()` 方法，可以：
- 增加新字段（如 `duration`、`brightness`）
- 增加新指令类型（如 `"motor": {"speed": 255}`）
- 增加鉴权/校验逻辑

示例扩展指令：
```json
{
  "hardware": "BLUE_LED",
  "action": "LED_BLINK",
  "duration": 5000,
  "repeat": 3
}
```

请确保服务端发送协议与之相符,一般建议服务端的发送方式是 websocket.send(列表) 实现分块发送,以防一些暂时没有发现的错误

### 替换 / 增加通信方式

**此项也有点吃技术**

`lib/webSocket/` 模块可以替换为其他通信方式：
- HTTP REST（替换为 `AsyncWebServer`）
- MQTT（替换为 `PubSubClient`）
- BLE（替换为 `NimBLE`）
- 串口直连（屏蔽 WiFi，通过 `Serial` 接收指令）

只需保持外部接口一致，核心的指令解析和执行模块无需改动。

### 其他可调参数

| 参数 | 位置 | 说明 |
|---|---|---|
| LED 闪烁时长 | `lib/flicker/fliker.h` | 调用时传入的 `delay_time` |
| WiFi 连接指示灯 | `src/config.cpp` / `src/main.cpp` | `connectWifi(ssid, pswd, green_led)` 可指定任意 GPIO |
| 串口波特率 | `src/main.cpp` `Serial.begin(115200)` | 按需调整 |
| SPIFFS 分区文件 | `data/actionAndHardware.txt` | 可扩展为 JSON 格式，存储更多配置 |

## 依赖

- [ArduinoJson](https://arduinojson.org/) — JSON 解析
- [WebSocketsClient](https://github.com/Links2004/arduinoWebSockets) — WebSocket 客户端
- [arduino-esp32](https://github.com/espressif/arduino-esp32) — ESP32 Arduino 核心
