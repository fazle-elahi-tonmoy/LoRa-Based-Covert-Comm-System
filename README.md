# LoRa Based Covert Comm System
# LoRa Based Covert Communication System

A secure and covert long-range communication system built using **LoRa**, **ESP32**, GPS modules, and a custom Command Center dashboard.
The system allows multiple field clients to communicate with each other using encrypted LoRa messages while sharing live GPS positions, silent vibration alerts, and display notifications.

---

## 📌 Project Overview

This project was designed as a **covert tactical communication platform** where multiple portable client devices can:

* Exchange encrypted messages over LoRa
* Share live GPS coordinates
* Trigger silent vibration notifications
* Display incoming alerts/messages
* Communicate with a central command station
* Operate over long distances with low power consumption

The Command Center dashboard provides:

* Live GPS tracking on map
* Client status monitoring
* Signal strength visualization
* Serial communication terminal
* Remote command execution
* Broadcast and targeted messaging

---

## ✨ Features

### 🔐 Secure Communication

* Encrypted LoRa message transmission
* Peer-to-peer messaging support
* Broadcast messaging support

### 📍 GPS Tracking

* Real-time client position monitoring
* OpenStreetMap integration
* Live marker updates

### 📶 Signal Monitoring

* RSSI-based signal strength meter
* Online/Offline device detection

### 📳 Silent Notification System

* Vibration alerts for covert communication
* Display notifications without audible sound

### 🖥️ Command Center Dashboard

* Browser-based monitoring interface
* Serial communication through Web Serial API
* Live terminal logs
* Client control buttons

### ⚡ Long Range Operation

* LoRa-based communication suitable for remote deployments
* Low bandwidth and low power design

---

# 🛠 Hardware Used

## Client Device

* ESP32
* LoRa Module (SX1278/SX1276)
* GPS Module (NEO-6M or similar)
* OLED/TFT Display
* Vibration Motor
* Battery Power Supply

## Ground Station

* ESP32
* LoRa Module
* USB Serial Connection

---

# 💻 Software Components

| Component                        | Description             |
| -------------------------------- | ----------------------- |
| `ESP32_Client.ino`               | Client firmware         |
| `ground_station.ino`             | Ground station firmware |
| `command center with debug.html` | Web dashboard           |

---

# 🌐 Command Center Dashboard

The dashboard provides a tactical-style monitoring interface with:

* Interactive map view
* Client cards
* Signal strength indicators
* Live serial terminal
* FIRE command trigger
* Messaging interface

The interface uses:

* HTML/CSS/JavaScript
* Leaflet.js
* Web Serial API
* OpenStreetMap tiles

Example features found in the dashboard include: 

* Real-time GPS marker updates
* Online/offline client status
* Signal strength visualization
* Serial communication monitoring
* Remote command controls

---

# 📡 Communication Protocol

Example command structure:

```text
MSG|CLIENT_ID|MESSAGE
FIRE|CLIENT_ID
READY|CLIENT_ID
```

Broadcast example:

```text
AT+SEND=0,6,FIRE|0
```

---

# 🗺️ Live Map Tracking

The dashboard integrates OpenStreetMap with Leaflet.js to display:

* Real-time GPS positions
* Client movement
* Online/offline markers
* Client labels

Example map initialization from the dashboard: 

---

# 📶 Signal Strength Visualization

RSSI values are converted into graphical signal bars for quick situational awareness.

Example thresholds:

| RSSI           | Signal    |
| -------------- | --------- |
| > -60 dBm      | Excellent |
| > -70 dBm      | Strong    |
| > -85 dBm      | Medium    |
| > -100 dBm     | Weak      |
| Below -100 dBm | Poor      |

---

# 🚀 How To Run

## 1️⃣ Upload Firmware

Upload:

* `ESP32_Client.ino` to client devices
* `ground_station.ino` to the base station ESP32

Using:

* Arduino IDE
* PlatformIO

---

## 2️⃣ Open Dashboard

Open:

```text
command center with debug.html
```

in a Chromium-based browser:

* Google Chrome
* Microsoft Edge

---

## 3️⃣ Connect Serial

* Click `CONNECT`
* Select the ESP32 serial port
* Dashboard will begin receiving live data

---

# 🧠 System Architecture

```text
CLIENT DEVICE <----LoRa----> GROUND STATION <----USB SERIAL----> WEB DASHBOARD
```

Multiple clients can communicate through the LoRa network while the command center monitors all nodes simultaneously.

---

# 📷 Future Improvements

* AES-256 encryption
* Mesh networking
* Offline map caching
* OTA firmware updates
* End-to-end authentication
* Battery telemetry
* Voice communication support
* LoRaWAN compatibility

---

# ⚠ Disclaimer

This project is intended strictly for:

* Educational purposes
* Research
* Emergency communication systems
* Robotics and IoT experimentation

Users are responsible for complying with local radio communication regulations.

---

# 👨‍💻 Author

Developed by **Fazle Elahi Tonmoy**

Passionate about:

* Robotics
* Embedded Systems
* UAV Systems
* Tactical Communication Technologies

---

# 📄 License

This project is licensed under the MIT License.

```text
MIT License © 2026 Fazle Elahi Tonmoy
```
