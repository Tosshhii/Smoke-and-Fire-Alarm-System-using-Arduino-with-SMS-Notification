Smoke and Fire Alarm System with GSM SMS Notification

Summary
- This version is converted from the original Wi-Fi/cloud-SMS design to use a GSM module (for example a SIM800L or SIM900) connected to an Arduino Uno.
- The sketch monitors a flame sensor and an MQ-2 smoke sensor, triggers a buzzer/LED alarm, and sends an SMS through the GSM module.

Required Hardware
- Arduino Uno (or another AVR-based Arduino compatible with SoftwareSerial)
- GSM module such as SIM800L / SIM900
- MQ-2 smoke/gas sensor
- Flame sensor
- 5V active buzzer
- Red LED and Green LED
- 220Ω resistors for the LEDs
- Breadboard and jumper wires
- SIM card with SMS service

Wiring
- Smoke sensor -> digital pin 2
- Flame sensor -> digital pin 6
- Green LED -> digital pin 3 (with resistor to GND)
- Red LED -> digital pin 5 (with resistor to GND)
- Buzzer -> digital pin 4
- GSM module TX -> digital pin 7
- GSM module RX -> digital pin 8
- GSM module VCC/GND -> proper power source (often 4V for SIM800L, check your module)

Software & Configuration
1. Open src/main.cpp and replace the placeholder phone number:
   - phoneNumber = "YOUR_PHONE_NUMBER"
2. Make sure your GSM module is powered correctly and uses the same baud rate as the sketch (9600).
3. Build and upload the sketch.

Notes
- The code uses SoftwareSerial for the GSM module, which is suitable for Arduino Uno.
- If your GSM module is not responding, check its power supply, baud rate, and SIM card status.
- The alarm loop is intentionally blocking so the buzzer keeps sounding until reset.
