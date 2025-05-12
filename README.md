# NeoHome Library

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub release](https://img.shields.io/github/release/yourusername/NeoHome.svg)](https://github.com/yourusername/NeoHome/releases)
[![Arduino Library Manager](https://img.shields.io/badge/Arduino%20Library%20Manager-NeoHome-blue.svg)](https://www.ardu-badge.com/NeoHome)

An advanced MQTT-based IoT control library for ESP32 devices that simplifies communication with MQTT brokers for home automation and IoT projects.

![NeoHome Architecture Diagram](https://via.placeholder.com/800x400.png?text=NeoHome+Architecture+Diagram) *(Consider adding a diagram here)*

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Examples](#examples)
- [Advanced Usage](#advanced-usage)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## Features

- **Easy Setup**: Simple WiFi and MQTT configuration
- **Automatic Reconnection**: Handles network drops gracefully
- **Virtual Pin System**: Flexible control and monitoring interface
- **Message Buffering**: Stores received values for reliable access
- **Topic Management**: Automatic subscription to device topics
- **ESP32 Optimized**: Built specifically for ESP32 capabilities
- **Extensible**: Easy to add custom functionality

## Installation

### Arduino Library Manager
1. Open Arduino IDE
2. Go to **Sketch > Include Library > Manage Libraries...**
3. Search for "NeoHome"
4. Click "Install"

### Manual Installation
1. Download the [latest release](https://github.com/yourusername/NeoHome/releases) ZIP file
2. In Arduino IDE, go to **Sketch > Include Library > Add .ZIP Library...**
3. Select the downloaded file

### Dependencies
- [PubSubClient](https://github.com/knolleary/pubsubclient) (installed automatically)
- WiFi (included with ESP32 board package)

## Quick Start

```cpp
#include <NeoHome.h>

NeoHome neohome;
const char* deviceId = "your_unique_device_id";
const char* wifiSSID = "your_wifi_network";
const char* wifiPassword = "your_wifi_password";

void setup() {
  Serial.begin(115200);
  neohome.begin(wifiSSID, wifiPassword, deviceId);
}

void loop() {
  // Read from virtual pin 1
  String sensorValue = neohome.read(deviceId, 1);
  Serial.println("Sensor value: " + sensorValue);
  
  // Write to virtual pin 2
  neohome.write(deviceId, 2, "Hello from ESP32!");
  
  delay(1000);
}