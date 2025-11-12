/*
  tinyCore (ESP32-S3) - Deep Sleep with Button Wake + Gyroscope
  
  This code demonstrates deep sleep with external interrupt wake:
  - Wakes up and turns LED on for 2 seconds
  - Reads gyroscope (rotation) every 100ms while awake
  - Goes to DEEP SLEEP (LED off) until button is pressed
  - Button on GPIO 2 wakes the board
  - Repeats indefinitely
  
  Hardware: 
  - tinyCore by MR.INDUSTRIES (ESP32-S3)
  - Button connected to GPIO 2 and GND (internal pullup used)
  - Onboard LSM6DSO 6-axis IMU (using gyroscope only)
  
  Libraries Required (install via Library Manager):
  - Adafruit LSM6DS (or Adafruit_LSM6DSO)
  
  IMPORTANT ESP32 NOTE:
  =====================
  Deep sleep on ESP32 COMPLETELY RESETS the microcontroller!
  After wake, code starts from setup() again, NOT from where it left off.
  This is different from Arduino Nano 33 BLE behavior.
  
  DEMO TEACHING POINT:
  =====================
  Try this: Rotate the board during sleep - nothing happens!
            Press button to wake, then rotate - gyroscope responds!
*/

#include "esp_sleep.h"
#include <Adafruit_LSM6DSO.h>

// LED pin (adjust based on your tinyCore - typically GPIO 48)
#define LED_PIN 48

// Button pin
#define BUTTON_PIN 2

// Timing constants
#define AWAKE_TIME 2000    // 2 seconds awake
#define SAMPLE_RATE 100    // Read sensor every 100ms

// Sensor object
Adafruit_LSM6DSO lsm6dso;

// Boot count tracking (stored in RTC memory, persists through deep sleep)
RTC_DATA_ATTR int bootCount = 0;

void setup() {
  // Increment boot count
  bootCount++;
  
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000); // Give serial time to initialize
  
  // Configure LED pin as output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Configure button pin with internal pullup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Initialize gyroscope sensor
  if (!lsm6dso.begin_I2C()) {
    Serial.println("Error initializing LSM6DSO sensor!");
    Serial.println("Check I2C connections!");
    while (1) delay(10); // Halt if sensor fails
  }
  
  Serial.println("===============================================");
  Serial.println("Deep Sleep Button Wake + Gyroscope Demo");
  Serial.println("tinyCore ESP32-S3 (mbed native)");
  Serial.printf("Boot number: %d\n", bootCount);
  Serial.println("Press button on GPIO 2 to wake from sleep");
  Serial.println("Open Serial Plotter to see gyroscope data");
  Serial.println("===============================================");
  delay(1000);
  
  // Run the awake period
  runAwakePeriod();
  
  // Prepare for deep sleep
  goToSleep();
}

void loop() {
  // Loop never runs - deep sleep resets and restarts from setup()
  // This is kept here for code structure clarity
}

void runAwakePeriod() {
  Serial.println("Awake");
  
  // Turn LED on
  digitalWrite(LED_PIN, HIGH);
  
  // Create sensor event objects
  sensors_event_t accel, gyro, temp;
  
  // Read gyroscope multiple times while awake
  unsigned long awakeStart = millis();
  while (millis() - awakeStart < AWAKE_TIME) {
    
    // Get sensor readings
    lsm6dso.getEvent(&accel, &gyro, &temp);
    
    // Format for Serial Plotter (gyroscope X, Y, Z in rad/s)
    Serial.print("GyroX:");
    Serial.print(gyro.gyro.x);
    Serial.print(",GyroY:");
    Serial.print(gyro.gyro.y);
    Serial.print(",GyroZ:");
    Serial.println(gyro.gyro.z);
    
    delay(SAMPLE_RATE);
  }
}

void goToSleep() {
  Serial.println("Going to sleep - sensor will NOT respond!");
  Serial.println("Press button to wake...");
  Serial.flush(); // Ensure message is sent before sleeping
  
  // Turn LED off
  digitalWrite(LED_PIN, LOW);
  
  // Configure external wake on button press (GPIO 2 goes LOW)
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, LOW);
  
  // Enter deep sleep (board will reset on wake)
  Serial.println("Entering deep sleep NOW");
  Serial.flush();
  esp_deep_sleep_start();
  
  // Code never reaches here - deep sleep resets the ESP32
}
