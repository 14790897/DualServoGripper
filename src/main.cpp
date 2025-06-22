#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <LittleFS.h>
#include "secrets.h"
//io13似乎没有pwm? 不对,根据文档,应该有pwm,最多四路
// 舵机配置
#define SERVO1_PIN 9   // 舵机1号 - 左爪
#define SERVO2_PIN 19  // 舵机2号 - 右爪 

// 舵机对象
Servo servo1;
Servo servo2;

// Web服务器
AsyncWebServer server(80);

// 舵机位置定义
int OPEN_ANGLE = 0;           // 张开角度 (可调节)
int CLOSE_ANGLE = 100;           // 抓取角度 (可调节)

// 计算舵机2的相反角度 (180度对称)
int getOppositeAngle(int angle) {
  return 180 - angle;
}

// 当前状态
bool isGripping = false;

// 测试舵机连接
void testServos() {
  Serial.println("测试舵机连接...");
  
  // 测试舵机1
  Serial.println("测试舵机1 (IO9):");
  servo1.write(90);
  delay(1000);
  servo1.write(180);
  delay(1000);
  Serial.print("舵机1当前角度: ");
  Serial.println(servo1.read());
  
  // 测试舵机2
  Serial.println("测试舵机2 (IO19):");
  servo2.write(90);
  delay(1000);
  servo2.write(0);
  delay(1000);
  Serial.print("舵机2当前角度: ");
  Serial.println(servo2.read());
  
  Serial.println("舵机测试完成");
}

// 执行抓取动作
void executeGrip()
{
  if (isGripping)
  {
    Serial.println("已经处于抓取状态");
    return;
  }
  
  Serial.println("开始抓取...");

  // 直接移动到抓取位置 - 两个舵机相向运动
  Serial.print("舵机1: ");
  Serial.print(servo1.read());
  Serial.print("° -> ");
  Serial.print(CLOSE_ANGLE);
  Serial.println("°");
  servo1.write(CLOSE_ANGLE);
  
  delay(200);
  
  Serial.print("舵机2: ");
  Serial.print(servo2.read());
  Serial.print("° -> ");
  Serial.print(getOppositeAngle(CLOSE_ANGLE));
  Serial.println("°");
  servo2.write(getOppositeAngle(CLOSE_ANGLE));

  delay(500); // 给舵机时间移动到位

  isGripping = true;
  Serial.println("抓取完成");
}

// 执行松开动作
void executeRelease()
{
  if (!isGripping)
  {
    Serial.println("已经处于松开状态");
    return;
  }
  
  Serial.println("开始松开...");

  // 直接移动到松开位置 - 两个舵机反向运动
  Serial.print("舵机1: ");
  Serial.print(servo1.read());
  Serial.print("° -> ");
  Serial.print(OPEN_ANGLE);
  Serial.println("°");
  servo1.write(OPEN_ANGLE);
  
  delay(200);
  
  Serial.print("舵机2: ");
  Serial.print(servo2.read());
  Serial.print("° -> ");
  Serial.print(getOppositeAngle(OPEN_ANGLE));
  Serial.println("°");
  servo2.write(getOppositeAngle(OPEN_ANGLE));

  delay(500); // 给舵机时间移动到位

  isGripping = false;
  Serial.println("松开完成");
}

// 演示完整抓取流程
void executeDemo()
{
  Serial.println("开始演示抓取流程...");

  // 1. 确保初始为松开状态
  executeRelease();
  delay(1000);

  // 2. 执行抓取
  executeGrip();
  delay(2000);

  // 3. 松开
  executeRelease();
  delay(1000);
  Serial.println("演示完成");
}

// 配置Web服务器
void setupWebServer() {  // 根页面 - 控制界面
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });
    // 状态查询
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    JsonDocument doc;
    doc["status"] = isGripping ? "gripping" : "released";
    doc["servo1_angle"] = servo1.read();
    doc["servo2_angle"] = servo2.read();
    doc["open_angle"] = OPEN_ANGLE;
    doc["close_angle"] = CLOSE_ANGLE;
    doc["ip"] = WiFi.localIP().toString();
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });
  
  // 设置角度参数
  server.on("/set_angles", HTTP_POST, [](AsyncWebServerRequest *request){
    String message = "角度设置: ";
    bool updated = false;
    
    // 检查并设置张开角度
    if (request->hasParam("open_angle", true)) {
      int newOpenAngle = request->getParam("open_angle", true)->value().toInt();
      if (newOpenAngle >= 0 && newOpenAngle <= 180) {
        OPEN_ANGLE = newOpenAngle;
        message += "张开角度=" + String(OPEN_ANGLE) + "° ";
        updated = true;
      } else {
        message += "张开角度无效(0-180) ";
      }
    }
    
    // 检查并设置抓取角度
    if (request->hasParam("close_angle", true)) {
      int newCloseAngle = request->getParam("close_angle", true)->value().toInt();
      if (newCloseAngle >= 0 && newCloseAngle <= 180) {
        CLOSE_ANGLE = newCloseAngle;
        message += "抓取角度=" + String(CLOSE_ANGLE) + "° ";
        updated = true;
      } else {
        message += "抓取角度无效(0-180) ";
      }
    }
    
    if (updated) {
      Serial.println("角度参数已更新: 张开=" + String(OPEN_ANGLE) + "°, 抓取=" + String(CLOSE_ANGLE) + "°");
      request->send(200, "application/json", "{\"message\":\"" + message + "\",\"open_angle\":" + String(OPEN_ANGLE) + ",\"close_angle\":" + String(CLOSE_ANGLE) + "}");
    } else {
      request->send(400, "application/json", "{\"message\":\"无有效参数\"}");
    }
  });
  
  // 执行抓取
  server.on("/grip", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.println("收到抓取指令");
    executeGrip();
    request->send(200, "application/json", "{\"message\":\"抓取完成\",\"status\":\"gripping\"}");
  });
  
  // 执行松开
  server.on("/release", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.println("收到松开指令");
    executeRelease();
    request->send(200, "application/json", "{\"message\":\"松开完成\",\"status\":\"released\"}");
  });
  
  // 演示动作
  server.on("/demo", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.println("执行演示动作");
    executeDemo();
    request->send(200, "application/json", "{\"message\":\"演示完成\",\"status\":\"demo_finished\"}");
  });
}

void setup() {
  Serial.begin(115200);
  Serial.println("双舵机抓取器启动中...");
    // 初始化LittleFS
  if(!LittleFS.begin(true)){
    Serial.println("LittleFS挂载失败");
    return;
  }
  Serial.println("LittleFS初始化成功");  // 初始化舵机
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  
  // 执行舵机测试
  testServos();
  
  // 初始位置 - 张开状态
  Serial.println("设置初始位置...");
  servo1.write(OPEN_ANGLE);
  servo2.write(getOppositeAngle(OPEN_ANGLE));
  delay(1000);
    // 连接WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi连接成功!");
  Serial.print("IP地址: http://");
  Serial.println(WiFi.localIP());
  
  // 配置Web服务器路由
  setupWebServer();
  
  // 启动服务器
  server.begin();  Serial.println("Web服务器已启动");
  Serial.println("可用指令:");
  Serial.println("  GET  /status - 查看当前状态");
  Serial.println("  POST /set_angles - 设置角度参数");
  Serial.println("  POST /grip   - 执行抓取");
  Serial.println("  POST /release - 执行松开");
  Serial.println("  POST /demo   - 演示抓取动作");
}

void loop() {
  // 主循环保持空闲，所有操作通过Web服务器处理
  delay(100);
}

