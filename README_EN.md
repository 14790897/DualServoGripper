# Dual Servo Gripper

ESP32C3-based dual servo gripper control system with WiFi remote control capabilities.

![Build Status](https://github.com/your-username/DualServoGripper/workflows/Build%20Firmware/badge.svg)

> 📖 **中文文档**: [README.md](README.md)

## 📹 Demo Video

🎬 **Project Demo Video**: [Watch on Bilibili - ESP32 Dual Servo Gripper Demo](https://www.bilibili.com/video/BV1wfNXzHEoZ/)

Video content includes:
- Web interface operation
- Real gripper action demonstration

## Features

- 🤏 **Dual Servo Control**: Controls two servos using IO9 and IO19 for gripping functionality
- 📶 **WiFi Control**: Remote control via web interface
- 🎭 **Demo Mode**: Automatic demonstration of complete grip-release sequence
- ⚙️ **Dynamic Angle Settings**: Real-time servo angle adjustment through web interface
- 📊 **Status Monitoring**: Real-time servo angle and device status monitoring
- 🌐 **Web Interface**: User-friendly control interface supporting mobile and desktop access

## Hardware Connections

```
ESP32C3 Development Board
├── IO9  → Servo1 signal wire (Left claw)
├── IO19 → Servo2 signal wire (Right claw)
├── 5V   → Servo power positive (Red wire)
└── GND  → Servo power negative (Black/Brown wire)
```

### Servo Wiring Instructions

Each servo has three wires:
- **Red wire (5V)**: Connect to ESP32C3's 5V pin
- **Black wire (GND)**: Connect to ESP32C3's GND pin  
- **Yellow wire (Signal)**: Connect to ESP32C3's IO9 and IO19

⚠️ **Important Notes**:
- Ensure adequate power supply (servos draw significant current)
- For high-power servos, consider using external 5V power supply
- Common ground connection: External power GND must be connected to ESP32C3 GND

## Software Configuration

### 1. WiFi Settings

First, copy the WiFi configuration template:

```bash
# Copy configuration template
cp include/secrets.example include/secrets.h
```

Then modify the WiFi configuration in `include/secrets.h`:

```cpp
const char* WIFI_SSID = "YourWiFiName";        // Change to your WiFi name
const char* WIFI_PASSWORD = "YourWiFiPassword"; // Change to your WiFi password
```

### 2. Upload Filesystem

```bash
# Upload frontend files to device
pio run --target uploadfs

# Compile project
pio run

# Upload to device
pio run --target upload

# Monitor serial output
pio device monitor
```

## 🚀 Automated Build

This project supports GitHub Actions for automated firmware building:

### Build Triggers
- Push to `main` or `master` branch
- Create Pull Request
- Publish Release

### Build Artifacts
Each build generates the following files:
- `firmware.bin` - Main program firmware
- `firmware.elf` - Debug ELF file  
- `littlefs.bin` - Filesystem image (contains Web frontend files)
- `build-info.txt` - Build information and version details

💡 **Frontend Files**: `littlefs.bin` contains all Web frontend files (HTML, CSS, JS) from the `data/` directory, automatically packaged into the filesystem image.

### Getting Build Files
1. **Development builds**: Download from GitHub Actions Artifacts
2. **Release builds**: Download pre-compiled firmware from Releases page

### Manual Firmware Flashing

📖 **Detailed Flashing Guide**: [FLASHING_GUIDE.md](FLASHING_GUIDE.md)

```bash
# Flash firmware using esptool
esptool.py --chip esp32c3 --port COM3 write_flash 0x0 firmware.bin

# Flash filesystem (includes frontend files)
esptool.py --chip esp32c3 --port COM3 write_flash 0x200000 littlefs.bin
```

## API Endpoints

After connecting to WiFi, control the device using the following APIs:

### HTTP Endpoints

| Method | Path | Function | Return |
|--------|------|----------|--------|
| GET | `/` | Control interface | HTML page |
| GET | `/status` | View status | JSON status info |
| POST | `/set_angles` | Set angles | JSON confirmation |
| POST | `/grip` | Execute grip | JSON confirmation |
| POST | `/release` | Execute release | JSON confirmation |
| POST | `/demo` | Demo action | JSON confirmation |

### Example Requests

```bash
# Check status
curl http://192.168.1.100/status

# Set angle parameters
curl -X POST http://192.168.1.100/set_angles \
  -d "open_angle=120&close_angle=60"

# Execute grip
curl -X POST http://192.168.1.100/grip

# Execute release
curl -X POST http://192.168.1.100/release

# Demo action
curl -X POST http://192.168.1.100/demo
```

## Servo Angle Settings

Now supports dynamic angle adjustment through web interface:

```cpp
int OPEN_ANGLE = 120;    // Open angle (adjustable via web interface)
int CLOSE_ANGLE = 0;     // Close angle (adjustable via web interface)
```

You can modify default values in code or adjust them real-time through the web interface.

## Web Control Interface

After device startup, access the device IP address in your browser to see the control interface:

- 🔧 **Angle Settings**: Dynamically adjust open and close angles
- 🤏 **Grip Button**: Execute grip action
- ✋ **Release Button**: Release grip
- 🎭 **Demo Button**: Demonstrate complete sequence
- 📊 **Status Button**: View device status

## Troubleshooting

### 1. WiFi Connection Failed

- Check if WiFi name and password are correct
- Confirm WiFi signal strength is adequate
- Check if router supports 2.4GHz band

### 2. Servo Not Moving

- Check servo power supply (5V and GND connections)
- Verify signal wire connections are correct (IO9, IO19)
- Check if servos themselves are working properly
- Adjust servo angle parameters

### 3. Web Interface Inaccessible

- Confirm device is connected to WiFi
- Check serial output for correct IP address
- Verify firewall settings

## Extension Features

Easy to extend with the following features:

- 🎮 **Gamepad Control**: Add Bluetooth gamepad support
- 📹 **Camera Integration**: Add visual feedback
- 🔧 **Multi-level Gripping**: Support different grip strengths
- 📱 **Mobile APP**: Develop dedicated control app
- 🤖 **Automation**: Integrate sensors for automatic gripping

## Tech Stack

- **Hardware**: ESP32C3 + Servos
- **Framework**: Arduino + PlatformIO
- **Networking**: ESPAsyncWebServer
- **Frontend**: HTML5 + CSS3 + JavaScript
- **Communication**: RESTful API + JSON

## 📺 Learning Resources

🎥 **Complete Tutorial Video**: [Bilibili - ESP32 Dual Servo Gripper from Scratch](https://www.bilibili.com/video/BV1wfNXzHEoZ/)

If this project helps you, welcome to:
- 🌟 Like the video for support
- 📺 Follow the creator for more IoT projects
- 💬 Share your work in video comments

## License

This project is open-sourced under the MIT License. Contributions and suggestions are welcome!
