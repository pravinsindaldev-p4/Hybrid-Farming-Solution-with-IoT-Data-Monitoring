<!-- ------------------------------------------------------------- -->
<!-- 🌾 HYBRID FARMING SOLUTION — README (STC CREATIVE CLUB STYLE) -->
<!-- Author: PRAVIN SINH RANA | Instagram: @S.T.C_CREATIVE_CLUB -->
<!-- ------------------------------------------------------------- -->

<h1 align="center">🌾 Hybrid Farming Solution with IoT Data Monitoring</h1>

<p align="center">
  <img src="https://img.shields.io/badge/IoT%20Project-Smart%20Agriculture-green?style=for-the-badge"/>
  <img src="https://img.shields.io/badge/ESP8266-NodeMCU-blue?style=for-the-badge"/>
  <img src="https://img.shields.io/badge/Dashboard-Live%20Monitoring-orange?style=for-the-badge"/>
</p>

<p align="center">
  <i>A fully automated smart-agriculture system featuring irrigation, UV hygiene,  
  real-time monitoring, SoftAP dashboard and live JSON API.</i>
</p>

---

## 📸 Project Preview

<p align="center">
  <img src="/assets/project_main_photo.png" width="650" alt="Main Project Photo"/>
</p>

<p align="center">
  <img src="/assets/project_overview.png" width="420"/>
  <img src="/assets/project_dashboard.png" width="420"/>
</p>

---

## 🔄 System Flow — How It Works

<p align="center">
  <img src="/assets/flow_diagram.png" width="720" alt="Flow Diagram"/>
</p>

---

## 🚀 Overview

The **Hybrid Farming Solution with IoT Data Monitoring** is a modern smart-agriculture automation system powered by **NodeMCU ESP8266**.  
It intelligently controls irrigation, monitors temperature, humidity, soil condition, and manages **UV hygiene** based on ambient light.

The device creates its own **Wi-Fi Hotspot** and hosts a **beautiful animated dashboard** at:

👉 **http://192.168.4.1**

---

## 🧩 Core Features

### 🌡️ Temperature & Humidity (DHT11)
Real-time values shown on LCD + Dashboard.

### 🌞 Ambient Light Detection (LDR)
Auto UV sterilization when darkness is detected.

### 🌱 Soil Moisture (DO Sensor)
Dry → Pump ON  
Wet → Pump OFF  

### 💧 Auto Water Pump Control
Fully automated based on soil condition.

### 🦠 UV Hygiene Mode
Dark environment → UV Relay ON

### 📟 16x2 I2C LCD Display
Live status + sensor updates.

### 📡 ESP8266 SoftAP + Local Dashboard
No internet required.

### ⚡ REST JSON API
Endpoint: `/data`

### 🎨 Modern Web UI
Animated icons • Responsive • Colorful • Clean layout

---

## 📦 Component List

| No. | Component Name |
|-----|----------------|
| 1 | NodeMCU ESP8266 |
| 2 | DHT11 Sensor |
| 3 | LDR Light Sensor |
| 4 | Soil Moisture Sensor (DO) |
| 5 | 16x2 I2C LCD Display |
| 6 | 2-Channel Relay Module |
| 7 | UV Light Module |
| 8 | Mini Water Pump |
| 9 | 5V / 12V Power Supply |
| 10 | Jumper Wires |

---

## 🛠️ Working Principle

### 🌑 1. Darkness → UV Light ON
- LDR detects low light  
- UV Relay ON  
- UV stops when light is detected again

### 🌱 2. Soil Moisture → Pump Automation
- DO HIGH → Dry soil → Pump ON  
- DO LOW → Wet soil → Pump OFF

### 🌡️ 3. DHT11 Monitoring
- Values appear on LCD  
- Visible on dashboard  
- Available via API

### 📡 4. SoftAP + JSON API
- **SSID:** Garden_AP  
- **Password:** 12345678  

**API:**  

/data → { t, h, l, s, p, u }
---

## 🌍 Applications

- 🚜 Smart Farming / Polyhouse  
- 🌿 Indoor Greenhouse  
- 💧 Smart Irrigation  
- 🦠 UV Agricultural Hygiene  
- 📈 IoT Monitoring / Data Logging  
- 🧪 Soil & Environmental Study  

---

## 🎓 Educational Use Cases

### 🏫 STEM & School Projects
Ideal for teaching IoT, sensors, automation and coding.

### 📚 Courses & Workshops
Focus on JSON API, web dashboards, and conditional logic.

### 🧑‍💻 Student Extensions
- Cloud logging  
- Extra sensors  
- Charts  
- Manual controls  

---

## 🧠 Concepts Covered

- Digital + Analog Sensing  
- Local Web Server  
- JSON API Development  
- Smart Automation Logic  
- Wireless Networking  
- IoT Agriculture System Design  

---


---

## 📚 Libraries Used

ESP8266WiFi.h
ESP8266WebServer.h
SimpleDHT.h
Wire.h
LiquidCrystal_I2C.h


---

## 🧭 Upload Instructions

### 1️⃣ Open  
`Hybrid_Farming_IoT.ino`

### 2️⃣ Verify & Upload  
Click ✔ Verify → ⬆ Upload

### 3️⃣ Connect to Hotspot  
SSID: **Garden_AP**  
Password: **12345678**

### 4️⃣ Open Dashboard  
👉 http://192.168.4.1

---

## 🔮 Future Upgrades

- ☁️ Cloud Logging (Firebase / MQTT)  
- 📱 Manual ON/OFF UI  
- 📊 Real-time Charts  
- 🧪 Advanced Sensors (BME280 / DHT22)  
- 🔋 Battery + Power Monitoring  

---

## 📜 License — MIT License
Free to use, modify and share.  
Commercial use allowed.

---

## ⭐ Support the Project

If this project helped you —  
### 👉 *Please give this repository a ⭐ on GitHub!*  
Your support motivates more high-quality IoT projects.

---

<p align="center">
Made with ❤️ by <b>Pravin Sinh Rana</b><br/>
Founder — <b>STC Creative Club</b><br/>
📞 9313057803 • 📸 Instagram: <b>@S.T.C_CREATIVE_CLUB</b>
</p>




