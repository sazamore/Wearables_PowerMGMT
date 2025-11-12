/*
  Nano 33 BLE - Low Power Sleep Template (mbed native)
  
  This code demonstrates a basic sleep/wake cycle:
  - Wakes up and turns GREEN LED on for 2 seconds
  - Goes to sleep and turns LED off for 5 seconds
  - Repeats indefinitely
  
  Hardware: Arduino Nano 33 BLE
  Library Required: NONE - uses built-in mbed sleep functions
  
  Note: This version uses native mbed sleep functions instead of
        the ArduinoLowPower library to avoid installation issues.
*/

#include <mbed.h>

// RGB LED pins on Nano 33 BLE (Active LOW - LOW=on, HIGH=off)
#define LED_RED    22
#define LED_GREEN  23
#define LED_BLUE   24

// Timing constants
#define AWAKE_TIME  2000  // 2 seconds awake
#define SLEEP_TIME  5000  // 5 seconds asleep

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial && millis() < 3000); // Wait for serial or timeout after 3s
  
  // Configure LED pins as outputs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  
  // Turn off all LEDs initially (HIGH = off for this board)
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  
  Serial.println("Low Power Sleep Demo Started (mbed native)");
  Serial.println("-------------------------------------------");
}

void loop() {
  // ===== AWAKE PERIOD =====
  Serial.println("Awake");
  
  // Turn GREEN LED on (LOW = on)
  digitalWrite(LED_GREEN, LOW);
  
  // Stay awake for 2 seconds
  delay(AWAKE_TIME);
  
  // ===== SLEEP PERIOD =====
  Serial.println("Going to sleep");
  Serial.flush(); // Ensure message is sent before sleeping
  
  // Turn GREEN LED off (HIGH = off)
  digitalWrite(LED_GREEN, HIGH);
  
  // Use mbed delay function to sleep for 5 seconds
  // This puts the CPU in low power mode automatically
  delay(SLEEP_TIME);
  
  // After sleep, the code continues here and loops back
}