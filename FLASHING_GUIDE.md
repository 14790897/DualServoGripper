# 固件刷写指南 / Firmware Flashing Guide

## 中文指南

### 📦 下载构建文件

从GitHub获取预编译固件：

1. **开发版本** (最新代码)
   - 进入项目的 [Actions页面](https://github.com/your-username/DualServoGripper/actions)
   - 点击最新的成功构建
   - 下载 "firmware-xxxxx" artifacts
   - 解压得到固件文件

2. **发布版本** (稳定版)
   - 进入项目的 [Releases页面](https://github.com/your-username/DualServoGripper/releases)
   - 下载最新release的固件文件

### 🔧 刷写步骤

#### 方法1: 使用esptool (推荐)

```bash
# 1. 安装esptool
pip install esptool

# 2. 刷写主程序固件
esptool.py --chip esp32c3 --port COM3 --baud 921600 write_flash 0x0 firmware.bin

# 3. 刷写文件系统 (包含Web界面)
esptool.py --chip esp32c3 --port COM3 --baud 921600 write_flash 0x200000 littlefs.bin

# 4. 重启设备
esptool.py --chip esp32c3 --port COM3 reset
```

#### 方法2: 使用PlatformIO

```bash
# 克隆项目并进入目录
git clone https://github.com/your-username/DualServoGripper.git
cd DualServoGripper

# 复制WiFi配置
cp include/secrets.example include/secrets.h
# 编辑 include/secrets.h 填入您的WiFi信息

# 上传固件和文件系统
pio run --target upload
pio run --target uploadfs
```

### 📋 文件说明

| 文件名 | 地址 | 描述 | 必需 |
|--------|------|------|------|
| `firmware.bin` | `0x0` | 主程序代码 | ✅ 是 |
| `littlefs.bin` | `0x200000` | 文件系统镜像<br>(包含HTML/CSS/JS) | ✅ 是 |
| `firmware.elf` | - | 调试文件 | ❌ 否 |
| `build-info.txt` | - | 构建信息 | ❌ 否 |

### ⚠️ 注意事项

1. **端口号**: 将 `COM3` 替换为您的实际串口号
   - Windows: `COM1`, `COM3`, `COM8` 等
   - Linux/Mac: `/dev/ttyUSB0`, `/dev/ttyACM0` 等

2. **文件系统地址**: `0x200000` 是LittleFS的默认地址，请勿修改

3. **波特率**: 921600是推荐速度，如有问题可降低到115200

4. **前端文件**: 不需要单独上传HTML/CSS/JS文件，它们已打包在 `littlefs.bin` 中

---

## English Guide

### 📦 Download Build Files

Get pre-compiled firmware from GitHub:

1. **Development builds** (Latest code)
   - Go to project [Actions page](https://github.com/your-username/DualServoGripper/actions)
   - Click on the latest successful build
   - Download "firmware-xxxxx" artifacts
   - Extract to get firmware files

2. **Release builds** (Stable)
   - Go to project [Releases page](https://github.com/your-username/DualServoGripper/releases)
   - Download firmware files from latest release

### 🔧 Flashing Steps

#### Method 1: Using esptool (Recommended)

```bash
# 1. Install esptool
pip install esptool

# 2. Flash main firmware
esptool.py --chip esp32c3 --port COM3 --baud 921600 write_flash 0x0 firmware.bin

# 3. Flash filesystem (includes Web interface)
esptool.py --chip esp32c3 --port COM3 --baud 921600 write_flash 0x200000 littlefs.bin

# 4. Reset device
esptool.py --chip esp32c3 --port COM3 reset
```

#### Method 2: Using PlatformIO

```bash
# Clone project and enter directory
git clone https://github.com/your-username/DualServoGripper.git
cd DualServoGripper

# Copy WiFi configuration
cp include/secrets.example include/secrets.h
# Edit include/secrets.h to fill in your WiFi info

# Upload firmware and filesystem
pio run --target upload
pio run --target uploadfs
```

### 📋 File Description

| Filename | Address | Description | Required |
|----------|---------|-------------|----------|
| `firmware.bin` | `0x0` | Main program code | ✅ Yes |
| `littlefs.bin` | `0x200000` | Filesystem image<br>(contains HTML/CSS/JS) | ✅ Yes |
| `firmware.elf` | - | Debug file | ❌ No |
| `build-info.txt` | - | Build information | ❌ No |

### ⚠️ Important Notes

1. **Port name**: Replace `COM3` with your actual serial port
   - Windows: `COM1`, `COM3`, `COM8`, etc.
   - Linux/Mac: `/dev/ttyUSB0`, `/dev/ttyACM0`, etc.

2. **Filesystem address**: `0x200000` is the default LittleFS address, do not modify

3. **Baud rate**: 921600 is recommended, reduce to 115200 if problems occur

4. **Frontend files**: No need to upload HTML/CSS/JS files separately, they are packaged in `littlefs.bin`
