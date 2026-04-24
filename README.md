# e-ink-dashboard
e ink dashboard for me to display weather my room temp calander and date and time and disply my spotify 

Hardware Design

I designed a custom PCB that integrates:
ESP32-S3 microcontroller
BME280 sensor
Power management (voltage regulator + charging IC)
USB-C input
Display connector

Features

Real-time weather display
displays whats spotify playing
Room temperature and humidity using BME280 sensor
Calendar, date, and time display
Low power consumption using e-ink technology

here is my schematics 
<img width="818" height="517" alt="Screenshot 2026-04-18 at 7 24 58 PM" src="https://github.com/user-attachments/assets/fc3147f5-a0a7-4a2d-ab5c-5fbad3047ee2" />
here my pcb
<img width="497" height="487" alt="Screenshot 2026-04-18 at 7 25 20 PM" src="https://github.com/user-attachments/assets/c7ee6942-7630-4664-9411-ba42252690aa" />
here is my case 
<img width="822" height="492" alt="Screenshot 2026-04-18 at 7 25 42 PM" src="https://github.com/user-attachments/assets/ab3bd300-f3e6-4536-af14-91c2dece892c" />
## Bill of Materials

| Name | Purpose | Qty | Total Cost (USD) | Distributor |
|------|---------|-----|-----------------|-------------|
| 7.3" E Ink Spectra 6 Display 800x480 | Display | 1 | $47.34 | Good Display |
| Resistor 4.7kΩ 0402 | Resistor | 100 | $0.08 | LCSC |
| Pin Socket 1×08 2.54mm | Socket | 5 | $0.53 | LCSC |
| USB-C Receptacle 16P HRO TYPE-C-31-M-12 | USB-C | 5 | $0.85 | LCSC |
| Switch 6×6×5mm | Button | 5 | $0.36 | LCSC |
| Resistor 1.2kΩ 0402 | Resistor | 100 | $0.09 | LCSC |
| BME280 | Temp/humidity sensor | 1 | $7.42 | LCSC |
| ME6211C33M5G-N | Voltage regulator | 10 | $0.50 | LCSC |
| TP4056-42-ESOP8 | Battery charger IC | 5 | $0.93 | LCSC |
| ESP32-S3-WROOM-1-N16R8 | MCU | 1 | $5.21 | LCSC |

**Total with shipping: $$78.20**
