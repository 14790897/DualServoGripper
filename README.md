# 双舵机抓取器 (Dual Servo Gripper)

基于ESP32C3的双舵机抓取器控制系统，支持WiFi远程控制。

## 📹 演示视频

🎬 **项目演示视频**: [B站观看 - ESP32双舵机抓取器实战演示](https://www.bilibili.com/video/BV1wfNXzHEoZ/)

视频内容包括：
- Web界面操作
- 实际抓取效果展示

## 功能特性

- 🤏 **双舵机控制**: 使用IO9和IO19控制两个舵机实现抓取功能
- 📶 **WiFi控制**: 通过Web界面远程控制抓取器
- 🎭 **演示模式**: 自动演示完整的抓取-松开流程
- ⚙️ **动态角度设置**: 支持通过Web界面实时调整舵机角度
- 📊 **状态监控**: 实时查看舵机角度和设备状态
- 🌐 **Web界面**: 友好的控制界面，支持手机和电脑访问

## 硬件连接

```
ESP32C3 开发板
├── IO9  → 舵机1信号线 (左爪)
├── IO19 → 舵机2信号线 (右爪)
├── 5V   → 舵机电源正极 (红线)
└── GND  → 舵机电源负极 (黑/棕线)
```

### 舵机接线说明

每个舵机有三根线：
- **红线 (5V)**: 连接到ESP32C3的5V引脚
- **黑线 (GND)**: 连接到ESP32C3的GND引脚  
- **黄线 (信号)**: 连接到ESP32C3的IO9和IO19

⚠️ **注意事项**:
- 确保电源供应足够 (舵机工作电流较大)
- 如果舵机功率较大，建议使用外部5V电源供电
- 共地连接：外部电源的GND要与ESP32C3的GND相连

## 软件配置

### 1. 修改WiFi设置

首先复制WiFi配置模板：

```bash
# 复制配置模板
cp include/secrets.example include/secrets.h
```

然后在 `include/secrets.h` 中修改WiFi配置：

```cpp
const char* WIFI_SSID = "YourWiFiName";        // 修改为您的WiFi名称
const char* WIFI_PASSWORD = "YourWiFiPassword"; // 修改为您的WiFi密码
```

### 2. 上传文件系统

```bash
# 上传前端文件到设备
pio run --target uploadfs

# 编译项目
pio run

# 上传到设备
pio run --target upload

# 查看串口输出
pio device monitor
```

## API接口

设备连接WiFi后，可通过以下API控制：

### HTTP端点

| 方法 | 路径 | 功能 | 返回 |
|------|------|------|------|
| GET | `/` | 控制界面 | HTML页面 |
| GET | `/status` | 查看状态 | JSON状态信息 |
| POST | `/set_angles` | 设置角度 | JSON确认信息 |
| POST | `/grip` | 执行抓取 | JSON确认信息 |
| POST | `/release` | 执行松开 | JSON确认信息 |
| POST | `/demo` | 演示动作 | JSON确认信息 |

### 示例请求

```bash
# 查看状态
curl http://192.168.1.100/status

# 设置角度参数
curl -X POST http://192.168.1.100/set_angles \
  -d "open_angle=120&close_angle=60"

# 执行抓取
curl -X POST http://192.168.1.100/grip

# 执行松开
curl -X POST http://192.168.1.100/release

# 演示动作
curl -X POST http://192.168.1.100/demo
```

## 舵机角度设置

现在支持通过Web界面动态调整角度参数：

```cpp
int OPEN_ANGLE = 120;    // 张开角度 (可通过Web界面调整)
int CLOSE_ANGLE = 0;     // 抓取角度 (可通过Web界面调整)
```

您也可以在代码中修改默认值，或通过Web界面实时调整。

## Web控制界面

设备启动后，在浏览器中访问设备IP地址即可看到控制界面：

- 🔧 **角度设置**: 动态调整张开和抓取角度
- 🤏 **抓取按钮**: 执行抓取动作
- ✋ **松开按钮**: 松开抓取
- 🎭 **演示按钮**: 演示完整流程
- 📊 **状态按钮**: 查看设备状态

## 故障排除

### 1. WiFi连接失败
- 检查WiFi名称和密码是否正确
- 确认WiFi信号强度足够
- 检查路由器是否支持2.4GHz频段

### 2. 舵机不动作

- 检查舵机电源供应 (5V和GND连接)
- 确认信号线接线是否正确 (IO9, IO19)
- 检查舵机本身是否正常工作
- 调整舵机角度参数

### 3. Web界面无法访问
- 确认设备已连接WiFi
- 检查串口输出获取正确IP地址
- 确认防火墙设置

## 扩展功能

可以轻松扩展以下功能：

- 🎮 **游戏手柄控制**: 添加蓝牙手柄支持
- 📹 **摄像头集成**: 添加视觉反馈
- 🔧 **多级抓取**: 支持不同力度的抓取
- 📱 **移动APP**: 开发专用控制APP
- 🤖 **自动化**: 集成传感器实现自动抓取

## 技术栈

- **硬件**: ESP32C3 + 舵机
- **框架**: Arduino + PlatformIO
- **网络**: ESPAsyncWebServer
- **前端**: HTML5 + CSS3 + JavaScript
- **通信**: RESTful API + JSON

## 📺 学习资源

🎥 **完整教程视频**: [B站 - ESP32双舵机抓取器从零开始](https://www.bilibili.com/video/BV1wfNXzHEoZ/)

如果这个项目对您有帮助，欢迎：
- 🌟 给视频点赞支持
- 📺 关注UP主获取更多IoT项目
- 💬 在视频评论区分享您的作品

## 开源协议

本项目基于MIT协议开源，欢迎贡献代码和提出建议！
