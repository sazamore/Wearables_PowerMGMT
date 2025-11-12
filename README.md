# Low Power Sleep Templates for Arduino Boards

This repository contains 6 template scripts demonstrating low-power sleep modes across different Arduino platforms. These templates are designed for students and educators to learn about power management in embedded systems.

---

## 📁 Repository Contents

### Arduino Nano 33 BLE (nRF52840 - mbed OS)
1. **`lowpower_33BLE.ino`** - Light sleep with 5-second timer wake
2. **`externalWake_33BLE.ino`** - Deep sleep with button wake + temperature/humidity sensor

### tinyCore (ESP32-S3)
3. **`lowpower_tinyCore.ino`** - Light sleep with 5-second timer wake
4. **`externalWake_tinyCore.ino`** - Deep sleep with button wake + gyroscope sensor

### Arduino Nano / Uno (ATmega328P - AVR)
5. **`lowpower_nano.ino`** - Power-down sleep with watchdog timer wake
6. **`extternalWake_nano.ino`** - Power-down sleep with button interrupt wake

---

## 🎯 How to Use These Templates

### Option 1: Direct Use
1. **Select the appropriate template** for your board
2. **Open in Arduino IDE**
3. **Install required libraries** (listed in each script's header)
4. **Upload to your board**
5. **Modify timing constants** at the top of the script as needed

### Option 2: Build Custom Code with AI Assistance

These templates are designed to work seamlessly with Claude (or other AI assistants) to generate custom code for your specific needs.

#### Step-by-Step Process:

1. **Choose your base template** based on your hardware platform

2. **Copy the relevant template** into your conversation with Claude

3. **Describe your modifications** clearly. Examples:
   ```
   "Modify this script to wake every 30 seconds instead of 5"
   
   "Add a second button on D3 that also wakes the board"
   
   "Change the sensor reading from temperature to accelerometer"
   
   "Add code to log data to SD card while awake"
   
   "Make the LED blink 3 times before going to sleep"
   ```

4. **Provide context** about your project:
   ```
   "I'm building a battery-powered weather station that needs to 
   last 6 months on a coin cell battery. I need to read temperature 
   every hour and deep sleep between readings."
   ```

5. **Ask for explanations** if you want to learn:
   ```
   "Can you explain why we disable Timer0 in AVR sleep mode?"
   
   "What's the difference between light sleep and deep sleep?"
   
   "How much power does each sleep mode consume?"
   ```

#### Example Prompts:

**For beginners:**
```
I have the lowpower_nano.ino template. Can you modify it to:
- Wake every 10 seconds instead of 5
- Blink the LED 5 times instead of solid on
- Add a counter that prints how many times it has woken up
```

**For intermediate users:**
```
Using the externalWake_tinyCore.ino template, help me:
- Add WiFi connection during wake period
- Send gyroscope data to a web server
- Only go to sleep if WiFi send is successful
```

**For advanced users:**
```
Take the externalWake_nano.ino and integrate it with:
- An external DS3231 RTC for accurate time keeping during sleep
- EEPROM storage to log wake times
- Multiple sleep durations based on time of day
```

---

## 🔧 Platform-Specific Notes

### Arduino Nano 33 BLE (mbed OS)
- **Sleep Library:** Built-in mbed functions (no external library needed)
- **Wake Sources:** Any interrupt pin, timer (via delay)
- **Memory:** RAM retained during sleep
- **Onboard Sensors:** IMU, Temp/Humidity, Proximity, Pressure
- **Key Command:** `sleep()` for light sleep

### tinyCore ESP32-S3
- **Sleep Library:** `esp_sleep.h` (built-in)
- **Wake Sources:** Timer, GPIO, touch sensor
- **Memory:** Deep sleep **RESETS** the board, use `RTC_DATA_ATTR` for persistence
- **Onboard Sensors:** LSM6DSO 6-axis IMU (accelerometer + gyroscope)
- **Key Commands:** 
  - `esp_light_sleep_start()` - RAM retained
  - `esp_deep_sleep_start()` - full reset, max power savings

### Arduino Nano/Uno (AVR)
- **Sleep Library:** `avr/sleep.h` (built-in)
- **Wake Sources:** D2 (INT0), D3 (INT1), watchdog timer
- **Memory:** RAM retained during sleep
- **Onboard Sensors:** None (add external sensors)
- **Key Command:** `sleep_mode()` with `SLEEP_MODE_PWR_DOWN`

---

## 📚 Common Modifications

### Change Wake Timing
```cpp
// For timer-based wake, modify these constants:
#define AWAKE_TIME 2000    // Time awake in milliseconds
#define SLEEP_TIME 5000    // Time asleep in milliseconds (or microseconds for ESP32)
```

### Add Additional Sensors
1. Include the sensor library at the top
2. Initialize sensor in `setup()`
3. Read sensor during awake period
4. Format output for Serial Plotter: `Serial.print("Label:"); Serial.println(value);`

### Change LED Behavior
```cpp
// Instead of solid on:
digitalWrite(LED_PIN, HIGH);

// Blink pattern:
for (int i = 0; i < 3; i++) {
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(100);
}
```

### Add Data Logging
Consider where to store data:
- **AVR:** EEPROM (limited writes) or SD card
- **Nano 33 BLE:** SD card or BLE transmission
- **ESP32:** SPIFFS, SD card, or WiFi transmission

---

## 🤖 Working with AI Assistants

### Best Practices:

1. **Start with a template** - Don't ask AI to write from scratch, modify existing working code
2. **Be specific** - "Add a temperature sensor" vs "Add the HTS221 sensor and read every 200ms"
3. **Ask for explanations** - Understanding the code helps you debug and modify further
4. **Test incrementally** - Make one change at a time and verify it works
5. **Share error messages** - If code doesn't compile, paste the full error message

### Sample Conversation Flow:

```
You: "I have the nano_ble_deep_sleep_button_temp_humidity.ino script. 
     Can you modify it to use the accelerometer instead of temp/humidity?"

Claude: "I'll modify the script to use the LSM9DS1 accelerometer. 
        Here are the changes..."

You: "Great! Now can you make it only wake if the board is moved 
     significantly? Like a motion-activated camera."

Claude: "I'll add motion detection logic using acceleration magnitude..."

You: "Can you explain how the motion detection algorithm works?"

Claude: "Sure! The motion detection calculates..."
```

---

## 🐛 Troubleshooting

### Script Won't Compile
1. Check you have the correct **board selected** in Arduino IDE
2. Install all **required libraries** listed at the top of each script
3. Copy the **full error message** and ask Claude for help

### Board Won't Wake from Sleep
1. **AVR:** Ensure button is on D2 or D3 (only pins that support wake)
2. **ESP32:** Check GPIO pin number matches your board's button
3. **All platforms:** Verify button wiring (button to pin, other side to GND)

### Sensor Not Working
1. Check sensor library is installed
2. Verify I2C connections (some sensors need external power)
3. Add `Serial.println()` debug statements to see where code fails
4. Ask Claude: "My sensor initialization fails, here's the error: [paste error]"

### Serial Monitor Shows Nothing After Wake
- **AVR:** This is normal - Serial is disabled during power-down sleep
- **ESP32 Deep Sleep:** Board resets, Serial needs time to reconnect
- **Add delays** after wake: `delay(1000);` before first `Serial.println()`

---

## 📊 Power Consumption Reference

| Platform | Active | Light Sleep | Deep Sleep |
|----------|--------|-------------|------------|
| AVR (Nano/Uno) | ~20mA | ~15mA (IDLE) | ~0.1mA (POWER_DOWN) |
| Nano 33 BLE | ~10mA | ~5µA | ~2µA |
| ESP32-S3 | 40-80mA | ~1-2mA | ~10µA |

*Note: Actual consumption varies with clock speed, peripherals, and code efficiency*

---

## 🎓 Learning Objectives

These templates teach:
- **Power management** fundamentals for battery-operated devices
- **Interrupt-driven programming** vs polling
- **Trade-offs** between power consumption and responsiveness
- **Platform differences** in sleep implementations
- **Real-time constraints** in embedded systems

---

## 🤝 Contributing

If you create a useful modification:
1. Test it thoroughly on your hardware
2. Add clear comments explaining what it does
3. Document any additional libraries needed
4. Share back to help others learn!

---

## 📖 Additional Resources

- [Arduino Low Power Documentation](https://docs.arduino.cc/learn/electronics/low-power)
- [ESP32 Sleep Modes Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html)
- [AVR Sleep Modes Tutorial](https://www.gammon.com.au/power)
- [Adafruit Low Power Guide](https://learn.adafruit.com/low-power-wifi-datalogging)

---

## 📝 License

These templates are provided for educational use. Modify and share freely!

---

**Questions?** Open an issue or ask Claude to help modify these templates for your specific project needs!
