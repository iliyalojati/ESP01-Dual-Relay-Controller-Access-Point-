# ESP-01 Dual Relay Controller (Glassmorphism UI)

A professional-grade, wireless dual-relay control system based on the ESP8266 (ESP-01) module, featuring a modern **Glassmorphism** web interface.

## 🌟 Features
- **Wireless Control:** Dual-channel relay management via a web-based dashboard.
- **Modern UI:** High-end Glassmorphism design for a sleek and responsive user experience.
- **High Stability:** Optimized boot sequence using `digitalWrite` before `pinMode` to prevent relay flickering during power-up.
- **Resource Management:** Efficient memory handling to prevent Stack Overflow and ensure long-term uptime.

## 🌐 Network Configuration
The device operates in **Access Point (AP) mode** for easy standalone setup.
- **SSID:** `ESP01_Control`
- **IP Address:** `192.168.4.1`

## 🛠 Hardware Specifications & Wiring

### Pin Mapping
| ESP-01 Pin | Connection | Description |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Power Supply |
| **GND** | GND | Ground |
| **CH_PD** | 3.3V | Chip Enable (Must be pulled HIGH) |
| **GPIO0** | Boot/GND | Set to GND for Flashing Mode |
| **GPIO2** | Relay 1/2 | Digital Output |

### Power Management & Noise Reduction
To ensure stability and prevent unexpected resets, the following components are highly recommended:
*   **Voltage Regulator:** AMS1117-3.3V.
*   **Decoupling Capacitors:** 
    *   **470µF** (Electrolytic) for bulk power stability.
    *   **100nF** (Ceramic) for high-frequency noise suppression.

## 🚀 Flashing Instructions
1. Connect **GPIO0** to **GND**.
2. Reset the ESP-01 or power it on to enter **Bootloader Mode**.
3. Upload the firmware using your preferred Arduino IDE or ESP8266 flashing tool.
4. Disconnect GPIO0 from GND and restart the device.

## 🔍 Troubleshooting

| Problem | Possible Cause | Solution |
| :--- | :--- | :--- |
| **Relay flickers on boot** | Missing `digitalWrite` before `pinMode` | Ensure code sets pin state BEFORE initializing `pinMode`. |
| **Device keeps resetting** | Power instability / Voltage drops | Add 470µF and 100nF capacitors near the ESP-01. |
| **Cannot connect to WiFi** | Insufficient power or wrong CH_PD state | Verify 3.3V supply and ensure CH_PD is pulled HIGH. |
| **Cannot flash firmware** | Not in Bootloader mode | Verify GPIO0 is connected to GND during startup. |

## 🛠 Software Stack
- **Language:** C++ (Arduino Framework)
- **UI/UX:** HTML5, CSS3 (Glassmorphism), JavaScript
- **Communication:** WebServer (HTTP)

---
*Developed for high-reliability IoT applications.*
